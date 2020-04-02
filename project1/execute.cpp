#include "execute.h"

ExecutionEnvironment::ExecutionEnvironment() {
    env = Environment();
    job_handler = JobHandler();
}

ExecutionEnvironment::~ExecutionEnvironment() {}

void ExecutionEnvironment::prefixCommandPaths(std::vector<Command>& seq) {
    std::string* s_ptr = nullptr;
    for(Command& com : seq) {
        if(com[0].front() != '/') {
            s_ptr = env.inPath(com[0]);
            if(s_ptr != nullptr) {
                com[0] = *s_ptr;
                delete s_ptr;
                s_ptr = nullptr;
            } else { 
                // try to parse it as a relative path
                // Must start with "./" or "../"
                if(com[0].front() == '.' && (com[0].at(1) == '/' || ((com[0].at(1) == '.') && (com[0].at(2) == '/')))) {
                    com[0] = resolvePath(com[0], env.getCwd());
                }
            }
        }
        if(!isExecutable(com[0])) {
            throw(std::runtime_error("The path '" + com[0] + "' is not a valid executable."));
        }
    }
}

void ExecutionEnvironment::execute(ParseStruct& ps) {
    if(ps.parse_type == BUILTIN) {
        executeBuiltin(*ps.builtin);
    } else if(ps.parse_type == PIPELINE) {
        prefixCommandPaths(ps.pipeline->command_sequence);
        executePipeline(*ps.pipeline);
    }
}

void ExecutionEnvironment::executePipeline(Pipeline& p) {
    if(p.is_background) {
        pid_t pid;
        if((pid = fork()) == 0) {
            // in child process
            executePipelineCore(p);
            exit(0);
        } else if(pid > 0) {
            // in parent process
            job_handler.add(pid, p.toStr());
        } else {
            throw(std::runtime_error("Fork failed."));
        }
    } else {
        executePipelineCore(p);
    }
}

void ExecutionEnvironment::executePipelineCore(Pipeline& p) {
    int fd_stdin_backup = dup(0);
    int fd_stdout_backup = dup(1);

    // Set input file descriptor
    int fd_in, fd_out;
    if(p.redir_in != nullptr) fd_in = open(p.redir_in->c_str(), O_RDONLY);
    else fd_in = dup(fd_stdin_backup);

    pid_t pid;
    // Iterate through the command chain
    for(int i = 0; i < p.command_sequence.size(); i++) {
        std::cout << "Command to execute: " << vectToStr(p.command_sequence[i]) << '\n';
        // Set file descriptor 0 to fd_in
        // This "mimicks" the stdin
        dup2(fd_in, 0);
        close(fd_in);

        // Check if last command
        if(i == p.command_sequence.size() - 1) {
            if(p.redir_out != nullptr) fd_out = open(p.redir_out->c_str(), O_WRONLY);
            else fd_out = dup(fd_stdout_backup);
        } else {
            int pipefds[2];
            pipe(pipefds);
            fd_in = pipefds[0]; // sets default stdin on next iteration
            fd_out = pipefds[1];
        }

        // Set file descriptor 1 to fd_out
        // This "mimicks" stdout
        dup2(fd_out, 1);
        close(fd_out);

        // Fork the process & run the command
        if((pid = fork()) == 0) { // In child process

            // Convert command vector into cstring array
            int size = p.command_sequence[i].size();
            char* arr[size + 1];
            for(int j = 0; j < size; i++) {
                strcpy(arr[j], p.command_sequence[i][j].c_str());
            }
            arr[size] = NULL;

            // Execute the command using execv            
            execv(arr[0], arr);
            perror("exec failed.");
            exit(1);
        }
    }
    // Done iterating, restore default file descriptors
    dup2(fd_stdin_backup, 0);
    close(fd_stdin_backup);
    dup2(fd_stdout_backup, 1);
    close(fd_stdin_backup);

    // wait for last child process to finish
    int status;
    waitpid(pid, &status, 0);
}

void ExecutionEnvironment::executeBuiltin(Builtin& b) {
    int size = b.command.size();
    if(b.command[0] == "cd") {
        if(size > 2) throw(std::runtime_error("Too many arguments provided for command 'cd'."));
        if(!env.setCwd(size == 1 ? env.getHome() : b.command[1])) throw(std::runtime_error("Invalid path provided for setting working directory."));
    } else if(b.command[0] == "exit" || b.command[0] == "quit") {
        if(size > 1) throw(std::runtime_error("Too many arguments provided for command 'exit/quit'."));
        exit(0);
    } else if(b.command[0] == "set") {
        if(size != 2) throw(std::runtime_error("Invalid argument count for command 'set'."));
        std::regex r("^\\s*([^=]*)=([^=]*)\\s*$");
        std::smatch m;
        if(std::regex_match(b.command[1], m, r)) {
            if(m[1] == "HOME") env.setHome(m[2]);
            else if(m[2] == "PATH") env.setPath(m[2]);
            else std::cout << "Only HOME and PATH are supported by the Quash Shell.\n";
        } else throw(std::runtime_error("Invalid argument structure for command 'set'."));
    } else if(b.command[0] == "jobs") {
        if(size > 1) throw(std::runtime_error("Too many arguments to command 'jobs'."));

        std::cout << std::left << std::setw(WIDTH) << std::setfill(' ') << "JOBID";
        std::cout << std::left << std::setw(WIDTH) << std::setfill(' ') << "PID";
        std::cout << "COMMAND" << '\n';
        std::cout << std::left << std::setw(40) << std::setfill('_') << std::endl;
        for(Job j : job_handler.getJobs()) {
            std::cout << std::left << std::setw(WIDTH) << std::setfill(' ') << j.job_id;
            std::cout << std::left << std::setw(WIDTH) << std::setfill(' ') << j.process_id;
            std::cout << j.command_name << '\n';
        }
    } else if(b.command[0] == "kill") {
        if(size != 3) throw(std::runtime_error("Invalid argument count for command 'kill'."));
        pid_t pid = (pid_t)std::stoi(b.command[1]);
        int sig = std::stoi(b.command[2]);
        int status = kill(pid, sig);
        if(status == -1) std::cout << "Error on command 'kill'. The signal was not sent.\n";
        else std::cout << "Signal " << sig << " sent to Process ID " << pid << ".\n";
    } else if(b.command[0] == "printvar") {
        if(size > 2) throw(std::runtime_error("Too many arguments provided for command 'printvar'."));
        std::string w = "";
        if(b.command[1] == "PATH") {
            for(std::string dir : env.getPath()) {
                w = w + dir + ":";
            }
            w.pop_back();
            std::cout << "PATH=" << w << '\n';
        } else if(b.command[1] == "HOME") {
            std::cout << "HOME=" << env.getHome() << '\n';
        } else {
            std::cout << "The environment variable '" << b.command[1] << "' is not supported by Quash Shell.\n";
        }
    } else throw(std::runtime_error("Error running builtin command."));
}
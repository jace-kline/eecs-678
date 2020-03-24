#include "parse.h"

ParseError::ParseError(std::string m) {
    msg = "Parse Error: " + m;
}

ParseError::~ParseError() {}

std::string ParseError::what() {return msg;}

std::vector<return_w_msgs<Result*>> parseLines(std::string s) {
    std::vector<return_w_msgs<Result*>> line_returns;
    std::stringstream ss = std::stringstream(s);
    for (std::string l; std::getline(ss, l, '\n');) {
        line_returns.push_back(parse_Result(l));
    }
    return line_returns;
}

Result::Result(RedirStdin *s, bool bg) : chain(s), background(bg) {}

Result::~Result() {
    if(chain != nullptr) delete chain;
}

Result* parse_Result(std::string s) {
    std::regex r("^(.*)\\s+&$");
    std::smatch m;
    bool bg = std::regex_match(s, m, r);
    std::string sub = bg ? m[1] : s;
    try {
        return(new Result(parse_RedirStdin(sub), bg));
    }
    catch(ParseError& e) {
        throw(e);
    }
}

RedirStdin::RedirStdin(ComWArgs* cwa, FilePath* fp, PipeSequence* seq) : com_w_args(cwa), in_path(fp), pipe_sequence(seq) {}

RedirStdin::~RedirStdin() {
    if(com_w_args != nullptr) delete com_w_args;
    if(in_path != nullptr) delete in_path;
    if(pipe_sequence != nullptr) delete pipe_sequence;
}

bool RedirStdin::isRedir() const {
    return(in_path != nullptr);
}

RedirStdin* parse_RedirStdin(std::string s) {
    std::regex r("^(.*)\\s+<\\s+([^[:space:]]*)(\\s+.*)?$");
    std::smatch m;
    bool redir = std::regex_match(s, m, r);
    ComWArgs* cwa = nullptr;
    FilePath* in = nullptr;
    PipeSequence* seq = nullptr;
    try {
        if(redir) {
            cwa = parse_ComWArgs(m[1]);
            in = parse_FilePath(m[2]);
            seq = parse_PipeSequence(m[3]);
        } else {
            seq = parse_PipeSequence(s);
        }

        if(cwa == nullptr && in == nullptr && seq == nullptr) {
            throw(ParseError("Invalid Pipe or Redirect Sequence."));
        }
        return(new RedirStdin(cwa, in, seq));
    }
    catch(ParseError& e) {
        throw(e);
    }
}

PipeSequence::PipeSequence(ComWArgs* cwa, PipeSequence* seq, RedirStdout* out) : com_w_args(cwa), next(seq), redir_stdout(out) {}

PipeSequence::~PipeSequence() {
    if(com_w_args != nullptr) delete com_w_args;
    if(next != nullptr) delete next;
    if(redir_stdout != nullptr) delete redir_stdout;
}

bool PipeSequence::isBridge() const {
    return(com_w_args == nullptr && next == nullptr && redir_stdout != nullptr);
}

bool PipeSequence::isCap() const {
    return(com_w_args == nullptr && next == nullptr && redir_stdout == nullptr);
}

PipeSequence* parse_PipeSequence(std::string s) {
    std::regex r("^\\s*(.*)\\s+\\|\\s+(.*)$");

}

PipeSequence* parse_PipeSequenceHelper(std::string s) {
    std::regex r("^\\s*(.*)\\s+\\|\\s+(.*)$");
    std::smatch m;
    if(std::regex_match(s, m, r)) {
        PipeSequence* prev = parse_PipeSequenceHelper(m[1]);
        ComWArgs* cwa = parse_ComWArgs(m[2]);
    }
    
}

RedirStdout::RedirStdout(ComWArgs* cwa, FilePath* fp) : com_w_args(cwa), out_path(fp) {}

RedirStdout::~RedirStdout() {
    if(com_w_args != nullptr) delete com_w_args;
    if(out_path != nullptr) delete out_path;
}

bool RedirStdout::isRedir() const {
    return(out_path != nullptr);
}

ComWArgs::ComWArgs(Command* c, ArgList* args) : command(c), arg_list(args) {}

ComWArgs::~ComWArgs() {
    if(command != nullptr) delete command;
    if(arg_list != nullptr) delete arg_list;
}

bool ComWArgs::valid() const {
    return (command != nullptr);
}

Command::Command(std::string s) : str(s), exec_path(nullptr) {
    builtin = (str == "cd" || str == "exit" || str == "quit" || str == "set");
}

Command::Command(FilePath* fp) : exec_path(fp), builtin(false) {}

Command::~Command() {
    if(exec_path != nullptr) delete exec_path;
}

bool Command::valid() const {
    return(!(exec_path == nullptr && builtin == false) && exec_path->isExecutable());
}

ArgList::ArgList(std::vector<std::string> vect) : args(vect) {}

ArgList::~ArgList() {}

bool ArgList::isEmpty() const {
    return args.empty();
}

std::string ArgList::toStr() const {
    std::string s = "";
    for(int i = 0; i < args.size(); i++) {
        s = s + (i > 0 ? " " : "") + args.at(i);
    }
}

FilePath::FilePath(std::string s) : str(s) {}

FilePath::~FilePath() {}

bool FilePath::isRegFile() const {
    struct stat s;
    if(stat(str.c_str(), &s) == 0) {
        return(s.st_mode & S_IFREG);
    }
    return false;
}

bool FilePath::isDir() const {
    struct stat s;
    if(stat(str.c_str(), &s) == 0) {
        return(s.st_mode & S_IFDIR);
    }
    return false;
}

bool FilePath::isExecutable() const {
    struct stat s;
    if(stat(str.c_str(), &s) == 0) {
        return(s.st_mode & S_IXUSR);
    }
    return false;
}
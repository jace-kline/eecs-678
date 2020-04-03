#include "parse.h"

template <typename T>
bool inVector(const T& element, const std::vector<T>& v) {
    return(std::find(v.begin(), v.end(), element) != v.end());
}

ParseStruct* constructParseStruct(const std::string& s) {
    // Get first word/command
    std::string w = getFirstWord(s);
    Pipeline* p;
    Builtin* b;
    // Check whether command is a builtin
    try{
        if(inVector(w, builtins)) {
            b = constructBuiltin(s);
            p = nullptr;
        } else {
            b = nullptr;
            p = constructPipeline(s);
        }
    }
    catch(std::runtime_error& e) {
        throw(e);
    }
    return (new ParseStruct(b, p));
}

Builtin* constructBuiltin(const std::string& s) {
    Command c;
    std::string w;
    std::stringstream ss(s);
    while(ss >> w) {
        if(w == "|" || w == "<" || w == ">" || w == "&") {
            throw(std::runtime_error("Operator '" + w + "' not permitted with builtin Quash commands."));
        } else c.push_back(w);
    }
    Builtin* b = new Builtin(c);
    return b;
}

Pipeline* constructPipeline(const std::string& s) {
    // Create a regular expression for determining
    // if should run as a background process
    std::regex r_bg("^\\s*(.*)\\s+&\\s*$");
    std::smatch m;
    bool bg = std::regex_match(s, m, r_bg);
    std::string str = bg ? m[1] : s;

    // Split the pipeline into vector chunks, 
    // delimitted by '|'
    // Make each argument surrounded by quotes " into a single
    // string instead of delimitted by whitespace
    std::vector<Command> seq;
    Command pipe_chunk;
    std::stringstream ss(str);
    std::string w;
    while(!ss.eof()) {
        pipe_chunk.clear();
        while(ss >> w) {
            if(w == "|") break;
            else if(w.front() == '"') {
                w = w.substr(1);
                std::string y;
                bool end = false;
                while(ss >> y) {
                    if(y.back() == '"') {
                        y.pop_back();
                        end = true;
                    }
                    w = w + ' ' + y;
                    if(end) break;
                    if(ss.eof()) throw(std::runtime_error("No closing quotation to corresponding open quotation in given command."));
                }
            }
            pipe_chunk.push_back(w);
        }
        if(pipe_chunk.empty() && !ss.eof()) throw(std::runtime_error("Parse error when reading command pipeline."));
        else seq.push_back(pipe_chunk);
    }

    FilePath* redir_out = parseRedir(OUT, seq[seq.size() - 1]);
    FilePath* redir_in = parseRedir(IN, seq[0]);

    for(Command chunk : seq) {
        for(std::string str : chunk) {
            if(str == "<" || str == ">" || str == "&") {
                throw(std::runtime_error("Invalid special character '" + str + "' present in text chunk '" + vectToStr(chunk) + "', found while parsing command pipeline."));
            }
        }
    }

    Pipeline* p = new Pipeline(seq, redir_in, redir_out, bg);
    return(p);
}

std::string trimLeadingWhitespace(const std::string& s) {
    size_t track = s.find_first_not_of(WHITESPACE);
	return (track == std::string::npos) ? "" : s.substr(track);
}

std::string trimTrailingWhitespace(const std::string& s) {
    size_t track = s.find_last_not_of(WHITESPACE);
	return (track == std::string::npos) ? "" : s.substr(0, track + 1);
}

std::string getFirstWord(const std::string& s) {
    std::stringstream ss(s);
    std::string w;
    ss >> w;
    return(w);
}

std::string vectToStr(const std::vector<std::string>& v) {
    std::string str = "";
    for(std::string s : v) {
        str = str + s + " ";
    }
    if(!str.empty()) str.pop_back();
    return str;
}

FilePath* parseRedir(RedirType t, Command& c) {
    int com_size = c.size();
    FilePath* fp = nullptr;
    if(com_size >= 3 && c[com_size - 2] == (t == IN ? "<" : ">")) {
        fp = new std::string(c.back());
        c.pop_back();
        c.pop_back();
    } 
    return(fp);
}

// void addPathPrefixes(std::vector<Command>& seq, const Environment& env) {
//     std::string* s_ptr = nullptr;
//     for(Command& com : seq) {
//         if(com[0].front() != '/') {
//             s_ptr = env.inPath(com[0]);
//             if(s_ptr != nullptr) {
//                 com[0] = *s_ptr;
//                 delete s_ptr;
//                 s_ptr = nullptr;
//             } else {
//                 delete s_ptr;
//                 throw(std::runtime_error("Executable file '" + com[0] + "' is not located in the PATH."));
//             }
//         }
//         if(!isExecutable(com[0])) {
//             throw(std::runtime_error("The file '" + com[0] + "' is not a valid executable."));
//         }
//     }
// }

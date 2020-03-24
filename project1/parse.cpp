#include "parse.h"

template <typename T>
bool inVector(const T& element, const std::vector<T>& v) {
    std::vector<T>::iterator it;
    it = std::find(v.begin(), v.end(), element);
    return(it != v.end());
}

Executable* constructExecutablePtr(const std::string& s) {
    // Get first word/command
    std::string w = getFirstWord(s);

    // Check whether command is a builtin
    try{
        if(inVector(w, builtins)) {
            return(constructBuiltin(s));
        } else return(constructPipeline(s));
    }
    catch(std::runtime_error& e) {
        throw(e);
    }
    return nullptr;
}

Builtin* constructBuiltin(const std::string& s) {
    Command c;
    std::string w;
    std::stringstream ss(s);
    while(ss >> w) {
        if(w == "|" || w == "<" || w == ">" || w == "&") {
            throw(std::runtime_error("Redirection and pipe operators not permitted with builtin Quash commands."));
        } else c.push_back(w);
    }
    Builtin* b = new Builtin(c);
    return b;
}

Pipeline* constructPipeline(const std::string& s) {
    std::vector<Command> seq;
    std::vector<std::string> pipe_chunk;
    std::stringstream ss(s);
    std::string w;

    // Create a regular expression for determining
    // if should run as a background process
    std::regex r_bg("^\\s*(.*)\\s+&\\s*$");
    std::smatch m;
    bool bg = std::regex_match(s, m, r_bg);
    w = bg ? m[1] : s;

    // Split the pipeline into vector chunks, 
    // delimitted by '|'
    while(!ss.eof()) {
        while(ss >> w) {
            if(w == "|") break;
            else pipe_chunk.push_back(w);
        }
        if(pipe_chunk.empty() && !ss.eof()) throw(std::runtime_error("Parse error when reading command pipeline."));
        else seq.push_back(pipe_chunk);
    }

    Maybe<FilePath> m_redir_out = parseRedir(OUT, seq[seq.size() - 1]);
    Maybe<FilePath> m_redir_in = parseRedir(IN, seq[0]);

    std::regex r_valid_text("");
    for(Command chunk : seq) {
        for(std::string str : chunk) {
            if(!std::regex_match(str, m, r_valid_text)) {
                throw(std::runtime_error("Invalid character(s) present in text chunk '" + str + "', found while parsing command pipeline."));
            }
        }
    }
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

Maybe<FilePath> parseRedir(RedirType t, Command& c) {
    int com_size = c.size();
    if(com_size >= 3 && c[com_size - 2] == (t == IN ? "<" : ">")) {
        Maybe<FilePath> m_fp(c.back());
        c.pop_back();
        c.pop_back();
        return(m_fp);
    } else return Maybe<FilePath>();
}

#include "parse.h"

template <typename T>
return_w_msgs<T>::return_w_msgs(const T& obj, const std::vector<std::string>& msg_vect) {
    item = obj;
    msgs = msg_vect;
}

template <typename T>
return_w_msgs<T>::~return_w_msgs() {}

template <typename T>
bool return_w_msgs<T>::anyMessages() const {
    return(!msgs.empty());
}

Result::Result(Sequence *s, bool bg) : seq(s), background(bg) {}

Result::~Result() {
    if(seq != nullptr) delete seq;
}

bool Result::valid() const {
    return (seq != nullptr);
}

return_w_msgs<Result*> parse_Result(std::string s) {

}

Sequence::Sequence(ComWArgs* cwa) : com_w_args(cwa), next(nullptr), path(nullptr), redir_stdout(false) {}

Sequence::Sequence(ComWArgs* cwa, Sequence* s) : com_w_args(cwa), next(s), path(nullptr), redir_stdout(false) {}

Sequence::Sequence(ComWArgs* cwa, FilePath* fp, bool stdout) : com_w_args(cwa), next(nullptr), path(fp), redir_stdout(stdout) {}

Sequence::~Sequence() {
    if(com_w_args != nullptr) delete com_w_args;
    if(next != nullptr) delete next;
    if(path != nullptr) delete path;
}

bool Sequence::valid() const {
    return (com_w_args != nullptr);
}

bool Sequence::isEnd() const {
    return (next == nullptr);
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
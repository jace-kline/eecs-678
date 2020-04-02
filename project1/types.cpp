#include "types.h"

Builtin::Builtin(const Command& c) {
    command = c;
}

Builtin::~Builtin() {}

Pipeline::Pipeline(const std::vector<Command>& seq, FilePath* in, FilePath* out, bool bg) {
    command_sequence = seq;
    redir_in = in;
    redir_out = out;
    is_background = bg;
}

Pipeline::~Pipeline() {
    if(redir_in != nullptr) delete redir_in;
    if(redir_out != nullptr) delete redir_out;
}

std::string Pipeline::toStr() const {
    std::string w = "";
    int i = 0;
    for(Command com : command_sequence) {
        for(std::string s : com) {
            w = w + s + ' ';
        }
        if(i == 0 && redir_in != nullptr) w = w + "< " + *redir_in + " ";
        if(i == command_sequence.size() - 1 && redir_out != nullptr) w = w + "> " + *redir_out;
        else if(i < command_sequence.size() - 1) w = w + "| ";
    }
    return w;
}

// template <typename T>
// Maybe<T>::Maybe() : nothing(true) {}

// template <typename T>
// Maybe<T>::Maybe(const T& obj) : nothing(false) {
//     item = obj;
// }

// template <typename T>
// Maybe<T>::~Maybe() {}

// template <typename T>
// bool Maybe<T>::isNothing() const {return nothing;}

// template <typename T>
// T Maybe<T>::getItem() const {
//     if(!isNothing()) return item;
//     else throw(std::runtime_error("Invalid access of nonexistent item in a Nothing-constructed Maybe type."));
// }
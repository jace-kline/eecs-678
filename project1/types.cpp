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
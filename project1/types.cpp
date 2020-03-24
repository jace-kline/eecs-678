#include "types.h"

Builtin::Builtin(const Command& c) {
    exec_type = BUILTIN;
    command = c;
}

Builtin::~Builtin() {}

Pipeline::Pipeline(const std::vector<Command>& seq, const Maybe<FilePath>& in, const Maybe<FilePath>& out, bool bg) {
    exec_type = PIPELINE;
    command_sequence = seq;
    m_redir_in = in;
    m_redir_out = out;
    is_background = bg;
}

Pipeline::~Pipeline() {}

template <typename T>
Maybe<T>::Maybe() : nothing(true) {}

template <typename T>
Maybe<T>::Maybe(const T& obj) : nothing(false) {
    item = obj;
}

template <typename T>
Maybe<T>::~Maybe() {}

template <typename T>
bool Maybe<T>::isNothing() const {return nothing;}

template <typename T>
T Maybe<T>::getItem() const {
    if(!isNothing()) return item;
    else throw(std::runtime_error("Invalid access of nonexistent item in a Nothing-constructed Maybe type."));
}
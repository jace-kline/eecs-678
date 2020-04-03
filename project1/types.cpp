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
        i++;
    }
    return w;
}

ParseStruct::ParseStruct(Builtin* b, Pipeline* p) {
    builtin = b;
    pipeline = p;
    if((b == nullptr && p == nullptr) || (b != nullptr && p != nullptr)) {
        parse_type = INVALID;
    } 
    else if(b != nullptr) parse_type = BUILTIN;
    else parse_type = PIPELINE;
}

ParseStruct::~ParseStruct() {
    // clean up heap-allocated memory
    if(builtin != nullptr) {
        delete builtin;
        builtin = nullptr;
    }
    if(pipeline != nullptr) {
        delete pipeline;
        pipeline = nullptr;
    }
}
#ifndef TYPES_H
#define TYPES_H

#include <stdexcept>
#include <string>
#include <vector>

typedef std::string FilePath;

// First index is command, following are arguments
typedef std::vector<std::string> Command;

enum ParseType {BUILTIN, PIPELINE, INVALID};
enum RedirType {IN, OUT};

class Builtin {
    public:
        // ParseType parse_type;
        Command command;
        Builtin(const Command& c);
        ~Builtin();
};

class Pipeline {
    public:
        // ParseType parse_type;
        std::vector<Command> command_sequence;
        FilePath* redir_in;
        FilePath* redir_out;
        bool is_background;
        Pipeline(const std::vector<Command>& seq, FilePath* in, FilePath* out, bool bg);
        ~Pipeline();
        std::string toStr() const;
};

// Implemented by the Builtin or Pipeline types
class ParseStruct {
    public:
        ParseType parse_type;
        Builtin* builtin;
        Pipeline* pipeline;
        ParseStruct(Builtin*, Pipeline*);
        ~ParseStruct();
};


#endif
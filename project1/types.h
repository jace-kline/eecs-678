#ifndef TYPES_H
#define TYPES_H

#include <stdexcept>
#include <string>
#include <vector>

typedef std::string FilePath;

// First index is command, following are arguments
typedef std::vector<std::string> Command;

enum ParseType {BUILTIN, PIPELINE};
enum RedirType {IN, OUT};

// template <typename T>
// class Maybe {
//     private:
//         T item;
//         bool nothing;
//     public:
//         Maybe<T>();
//         Maybe<T>(const T& obj);
//         ~Maybe<T>();
//         bool isNothing() const;
//         T getItem() const;
// };


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
};

// Implemented by the Builtin or Pipeline types
struct ParseStruct {
    ParseType parse_type;
    Builtin* builtin;
    Pipeline* pipeline;
};


#endif
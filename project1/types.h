#ifndef TYPES_H
#define TYPES_H

#include <stdexcept>
#include <string>
#include <vector>

typedef std::string FilePath;

// First index is command, following are arguments
typedef std::vector<std::string> Command;

enum ExecutableType {BUILTIN, PIPELINE};
enum RedirType {IN, OUT};

// Implemented by the Builtin or Pipeline types
class Executable {
    // public:
    //     virtual void run() = 0;
};

class Builtin : public Executable {
    public:
        ExecutableType exec_type;
        Command command;
        Builtin(const Command& c);
        ~Builtin();
};

class Pipeline : public Executable {
    public:
        ExecutableType exec_type;
        std::vector<Command> command_sequence;
        Maybe<FilePath> m_redir_in;
        Maybe<FilePath> m_redir_out;
        bool is_background;
        Pipeline(const std::vector<Command>& seq, const Maybe<FilePath>& in, const Maybe<FilePath>& out, bool bg);
        ~Pipeline();
};

template <typename T>
class Maybe {
    private:
        T item;
        bool nothing;
    public:
        Maybe<T>();
        Maybe<T>(const T& obj);
        ~Maybe<T>();
        bool isNothing() const;
        T getItem() const;
};

#endif
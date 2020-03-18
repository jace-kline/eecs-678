#ifndef PARSE_H
#define PARSE_H

#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

template <typename T>
class return_w_msgs {
    public:
        T item;
        std::vector<std::string> msgs;
        return_w_msgs<T>(const T&, const std::vector<std::string>&);
        ~return_w_msgs<T>();
        bool anyMessages() const;
};

std::vector<return_w_msgs<Result*>> parseLines(std::string s);

class Result {
    public:
        Sequence* seq;
        bool background;
        Result(Sequence* s, bool bg);
        ~Result();
        bool valid() const;
};

return_w_msgs<Result*> parse_Result(std::string s);

class Sequence {
    public:
        ComWArgs* com_w_args;

        // CASE 1: pipe encountered
        // CASE 2: redirect stdout to file
        // CASE 3: redirect stdin to program
        // DEFAULT: single ComWArgs (no pipes or redirects)
        Sequence* next;
        FilePath* path;
        bool redir_stdout;
        Sequence(ComWArgs*);
        Sequence(ComWArgs*, Sequence*);
        Sequence(ComWArgs*, FilePath*, bool stdout);
        ~Sequence();
        bool isEnd() const;
        bool valid() const;
};

return_w_msgs<Sequence*> parse_Sequence(std::string s);

class ComWArgs {
    public:
        Command* command;
        ArgList* arg_list;

        /* First token should be executable file path
        and following tokens should be valid arguments
        */
        ComWArgs(Command* c, ArgList* args);
        bool valid() const;
};

return_w_msgs<ComWArgs*> parse_ComWArgs(std::string s);

class Command {
    public:
        std::string str;
        FilePath* exec_path;
        bool builtin;
        // CASE 1: cd
        // CASE 2: exit || quit
        // CASE 3: set
        // CASE 4: executable (file path)
        Command(std::string s);
        Command(FilePath* fp);
        ~Command();
        bool valid() const;
};

return_w_msgs<Command*> parse_Command(std::string s);

class ArgList {
    public:
        std::vector<std::string> args;

        ArgList(std::vector<std::string> args);
        ~ArgList();
        bool isEmpty() const;
        std::string toStr() const;

};

return_w_msgs<ArgList*> parse_ArgList(std::string s);

class FilePath {
    public:
        std::string str;
        FilePath(std::string s);
        ~FilePath();
        bool isRegFile() const;
        bool isDir() const;
        bool isExecutable() const;
};

return_w_msgs<FilePath*> parse_FilePath(std::string s);


#endif
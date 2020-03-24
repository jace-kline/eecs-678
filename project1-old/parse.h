#ifndef PARSE_H
#define PARSE_H

#define LOGFILE ""

#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

class ParseError {
    private:
        std::string msg;
    public:
        ParseError(std::string m);
        ~ParseError();
        std::string what();
};

// std::vector<return_w_msgs<Result*>> parseLines(std::string s);

class Result {
    public:
        RedirStdin* chain;
        bool background;
        Result(RedirStdin* s, bool bg);
        ~Result();
};

Result* parse_Result(std::string s);

class RedirStdin {
    public:
        ComWArgs* com_w_args; // optional (if no redirect in)
        FilePath* in_path; // optional (if no redirect in)
        PipeSequence* pipe_sequence;
        RedirStdin(ComWArgs*, FilePath*, PipeSequence*);
        ~RedirStdin();
        bool isRedir() const;
};

RedirStdin* parse_RedirStdin(std::string s);

class PipeSequence {
    public:
        ComWArgs* com_w_args; // optional (if 1 or fewer pipes)
        PipeSequence* next; // optional (if 1 or fewer pipes)
        RedirStdout* redir_stdout; // optional (if 0 pipes)
        PipeSequence(ComWArgs*, PipeSequence*, RedirStdout*);
        ~PipeSequence();
        bool isBridge() const;
        bool isCap() const;
};

PipeSequence* parse_PipeSequence(std::string s);

class RedirStdout {
    public:
        ComWArgs* com_w_args;
        FilePath* out_path; // optional (if no redirect out)
        RedirStdout(ComWArgs*, FilePath*);
        ~RedirStdout();
        bool isRedir() const;
};

RedirStdout* parse_RedirStdout(std::string s);

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

ComWArgs* parse_ComWArgs(std::string s);

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

Command* parse_Command(std::string s);

class ArgList {
    public:
        std::vector<std::string> args;

        ArgList(std::vector<std::string> args);
        ~ArgList();
        bool isEmpty() const;
        std::string toStr() const;

};

ArgList* parse_ArgList(std::string s);

class FilePath {
    public:
        std::string str;
        FilePath(std::string s);
        ~FilePath();
        bool isRegFile() const;
        bool isDir() const;
        bool isExecutable() const;
};

FilePath* parse_FilePath(std::string s);


#endif
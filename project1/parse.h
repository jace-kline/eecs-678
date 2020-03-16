#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>

class Result {
    public:
        Sequence* sq;
        bool background;
        Result(Sequence* s, bool bg);
        ~Result();
        bool valid() const;
};

Result* parse_Result(std::string s);

class Sequence {
    public:
        ComWArgs* com_w_args;

        // CASE 1: pipe encountered
        // CASE 2: redirect stdout to file
        // CASE 3: redirect stdin to program
        // DEFAULT: single ComWArgs (no pipes or redirects)
        Sequence* next;
        FilePath* path;
        Sequence(ComWArgs*, Sequence*);
        Sequence(ComWArgs*, FilePath*, bool redir_stdout, bool redir_stdin);
        ~Sequence();
        bool valid() const;
};

Sequence* parse_Sequence(std::string s);

class ComWArgs {
    public:
        std::string full_string;

        /* First token should be executable file path
        and following tokens should be valid arguments
        */
        ComWArgs(std::string s);
        bool validCommand() const;
        std::string command();
        std::string args();
};

ComWArgs* parse_ComWArgs(std::string s);




#endif
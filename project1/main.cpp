#include "user-interact.h"
#include "execute.h"

int main() {
    ExecutionEnvironment executer = ExecutionEnvironment();
    ParseStruct ps;
    while(true) {
        // read input line into a string
        std::string s = promptReadStdinLine();
        try {
            // attempt to construct a ParseStruct
            ps = constructParseStruct(s);

            // pass the ParseStruct to the executer to execute
            executer.execute(ps);

        } catch(std::runtime_error& e) {
            std::cout << e.what() << '\n';
        }

        // clean up heap-allocated memory
        if(ps.builtin != nullptr) {
            delete ps.builtin;
            ps.builtin = nullptr;
        }
        if(ps.pipeline != nullptr) {
            delete ps.pipeline;
            ps.pipeline = nullptr;
        }

    } 
    return 0;
}
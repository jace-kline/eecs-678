#include "user-interact.h"
#include "execute.h"

int main() {
    //setvbuf(stdout, NULL, _IONBF, 0);
    ExecutionEnvironment executer = ExecutionEnvironment();
    ParseStruct* ps = nullptr;
    while(true) {
        // read input line into a string
        std::string s = promptReadStdinLine();
        if(!s.empty()) {
            try {
                // attempt to construct a ParseStruct
                ps = constructParseStruct(s);
                // pass the ParseStruct to the executer to execute
                if(ps != nullptr && ps->parse_type != INVALID) executer.execute(*ps);
            } catch(std::runtime_error& e) {
                std::cout << e.what() << '\n';
            }
        }

        executer.job_handler.refresh();

        if(ps != nullptr) {
            delete ps;
            ps = nullptr;
        }
    } 
    return 0;
}
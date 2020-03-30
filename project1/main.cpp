#include "parse.h"
#include "user-interact.h"
#include "environment.h"

int main() {
    std::string s;
    ParseStruct ps;
    Environment env = Environment();
    while(true) {
        s = promptReadStdinLine();
        try {
            ps = constructParseStruct(s);
            if(ps.parse_type == BUILTIN) {
                // executeBuiltin(*ps.builtin);
                delete(ps.builtin);
                ps.builtin = nullptr;
            } else if(ps.parse_type == PIPELINE) {
                addPathPrefixes(ps.pipeline->command_sequence, env);
                for(Command com : ps.pipeline->command_sequence) {
                    std::cout << vectToStr(com) << '\n';
                }
                // executePipeline(*ps.pipeline);
                delete(ps.pipeline);
                ps.pipeline = nullptr;
            }
        } catch(std::runtime_error& e) {
            std::cout << e.what() << '\n';
        }

        // std::cout << isDirectory(s) << '\n';
        // std::cout << isFile(s) << '\n';
        // std::cout << isExecutable(s) << '\n';

        // std::cout << env.setPath(s) << '\n';
        // std::cout << "PATH: " << vectToStr(env.getPath()) << '\n';
        // std::cout << "HOME: " << env.getHome() << '\n';

    } 
    return 0;
}
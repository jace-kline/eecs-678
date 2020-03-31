#include "user-interact.h"
#include "execute.h"

int main() {
    ParseStruct ps;
    env = Environment();
    job_handler = JobHandler();
    while(true) {
        std::string s = promptReadStdinLine();
        try {
            bool status;
            ps = constructParseStruct(s);
            if(ps.parse_type == BUILTIN) {
                // executeBuiltin(*ps.builtin);
            } else if(ps.parse_type == PIPELINE) {
                addPathPrefixes(ps.pipeline->command_sequence, env);
                // for(Command com : ps.pipeline->command_sequence) {
                //     std::cout << vectToStr(com) << '\n';
                // }
                // executePipeline(*ps.pipeline);
            }
        } catch(std::runtime_error& e) {
            std::cout << e.what() << '\n';
        }

        if(ps.builtin != nullptr) delete ps.builtin;
        if(ps.pipeline != nullptr) delete ps.pipeline;

        // std::cout << isDirectory(s) << '\n';
        // std::cout << isFile(s) << '\n';
        // std::cout << isExecutable(s) << '\n';

        // std::cout << env.setPath(s) << '\n';
        // std::cout << "PATH: " << vectToStr(env.getPath()) << '\n';
        // std::cout << "HOME: " << env.getHome() << '\n';

    } 
    return 0;
}
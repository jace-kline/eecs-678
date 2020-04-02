#ifndef EXECUTE_H
#define EXECUTE_H

#include "parse.h"
#include "job-handler.h"
#include <signal.h>
#include <iomanip>

class ExecutionEnvironment {
    private:
        Environment env;
        JobHandler job_handler;

    protected:
        // use env to prefix the executable locations
        void prefixCommandPaths(std::vector<Command>& seq);
        void executePipeline(Pipeline& p);
        void executePipelineCore(Pipeline& p);
        void executeBuiltin(Builtin& b);

    public:
        ExecutionEnvironment();
        ~ExecutionEnvironment();
        void execute(ParseStruct& ps);
};

#endif
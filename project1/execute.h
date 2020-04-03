#ifndef EXECUTE_H
#define EXECUTE_H

#include "parse.h"
#include "job-handler.h"
#include <signal.h>
#include <iomanip>
#include <cstring>

class ExecutionEnvironment {

    protected:
        // use env to prefix the executable locations
        void prefixCommandPaths(std::vector<Command>& seq);
        void executePipeline(Pipeline& p);
        // void executePipelineCore(Pipeline& p);
        // void executePipelineRecurse(Pipeline& p, int* fds, int i);
        void executeBuiltin(Builtin& b);

    public:
        Environment env;
        JobHandler job_handler;
        ExecutionEnvironment();
        ~ExecutionEnvironment();
        void execute(ParseStruct& ps);
};

#endif
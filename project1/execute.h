#ifndef EXECUTE_H
#define EXECUTE_H

#include "parse.h"
#include "job-handler.h"
#include <unistd.h>

// These 2 objects make up the global state of the program
// Allows each child process to copy to their own address space
Environment env;
JobHandler job_handler;

// error messages are thrown as exceptions, caught 
// and displayed in main()
void executeBuiltin(const Builtin& b);

// returns pid of last command process in the pipeline
int executePipeline(const Pipeline& p);

#endif
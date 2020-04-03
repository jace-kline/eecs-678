#ifndef JOB_HANDLER_H
#define JOB_HANDLER_H

#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

struct Job {
    int job_id;
    pid_t process_id;
    std::string command_name;
};

class JobHandler {
    private:
        std::vector<Job> jobs;
        int id_iter;
        
    public:
        JobHandler();
        ~JobHandler();
        void add(pid_t pid, const std::string& name);
        const std::vector<Job>& getJobs();
        pid_t pidByJobID(int jobid);
        void refresh();
};

#endif
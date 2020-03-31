#ifndef JOB_HANDLER_H
#define JOB_HANDLER_H

#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>

struct Job {
    int job_id;
    pid_t process_id;
    std::string command_name;
};

class JobHandler {
    private:
        std::vector<Job> jobs;
        int id_iter;

    protected:
        // Query each job's process to see if still running
        // If job not running (or error), delete
        void refresh(); 
        
    public:
        JobHandler();
        ~JobHandler();
        void add(pid_t pid, const std::string& name);
        const std::vector<Job>& getJobs();
};

#endif
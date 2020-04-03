#include "job-handler.h"

JobHandler::JobHandler() {
    id_iter = 1;
}

JobHandler::~JobHandler() {}

void JobHandler::refresh() {
    pid_t ret;
    int status;
    for(int i = 0; i < jobs.size(); i++) {
        ret = waitpid(jobs.at(i).process_id, &status, WNOHANG);
        // Process terminated or error
        if(ret < 0 || ret == jobs.at(i).process_id) {
            std::cout << '[' << jobs.at(i).job_id << "]\t" << jobs.at(i).process_id << " finished " << jobs.at(i).command_name << "\n";
            jobs.erase(jobs.begin() + i);
        }
    }
}

void JobHandler::add(pid_t pid, const std::string& name) {
    struct Job j;
    j.job_id = id_iter;
    j.process_id = pid;
    j.command_name = name;
    jobs.push_back(j);
    id_iter++;
}

const std::vector<Job>& JobHandler::getJobs() {
    refresh();
    return jobs;
}

pid_t JobHandler::pidByJobID(int jobid) {
    for(int i = 0; i < jobs.size(); i++) {
        if(jobid == jobs[i].job_id) return(jobs[i].process_id);
    }
    return(-1);
}
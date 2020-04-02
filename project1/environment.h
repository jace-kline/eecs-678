#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "filepath.h"

# define PATH_MAX 4096

class Environment {
    private:
        std::vector<std::string> path;
        std::string home;
        std::string cwd;

    public:
        Environment();
        ~Environment();

        // remove the 'PATH=' before passing string
        // each vector string delimitted by ':'
        bool setPath(const std::string& s);
        bool setHome(const std::string& s);
        bool setCwd(const std::string& s);
        std::string* inPath(const std::string& s) const;
        const std::vector<std::string>& getPath() const; 
        std::string getHome() const;
        std::string getCwd() const;
};



#endif
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "filepath.h"

class Environment {
    private:
        std::vector<std::string> path;
        std::string home;
    public:
        Environment();
        ~Environment();

        // remove the 'PATH=' before passing string
        // each vector string delimitted by ':'
        bool setPath(const std::string& s);
        bool setHome(const std::string& s);
        std::string* inPath(const std::string& s) const;
        const std::vector<std::string>& getPath() const; 
        std::string getHome() const;
};



#endif
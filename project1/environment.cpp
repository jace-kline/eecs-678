#include "environment.h"

Environment::Environment() {
    setPath(std::getenv("PATH"));
    setHome(std::getenv("HOME"));
}

Environment::~Environment() {}

bool Environment::setPath(const std::string& s) {
    std::string w;
    std::vector<std::string> v;
    std::stringstream ss(s);
    while(!ss.eof()) {
        std::getline(ss, w, ':');
        if(w.back() != '/') w.push_back('/');
        if(isDirectory(w)) v.push_back(w);
        else return false;
    }
    path.clear();
    path = v;
    return true;
}

bool Environment::setHome(const std::string& s) {
    if(isDirectory(s)) {
        home = s;
        return true;
    }
    return false;
}

std::string* Environment::inPath(const std::string& s) const {
    std::string* p = nullptr;
    std::string t;
    for(std::string dir : path) {
        t = dir + s;
        if(isExecutable(t)) {
            p = new std::string(t);
            break;
        }
    }
    return p;
}

const std::vector<std::string>& Environment::getPath() const {
    return path;
}

std::string Environment::getHome() const {
    return home;
}
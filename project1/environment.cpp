#include "environment.h"

Environment::Environment() {
    setPath(std::getenv("PATH"));
    setHome(std::getenv("HOME"));

    // get cwd from system
    // if can't retrieve, then set cwd = "/"
    char wd[PATH_MAX];
    cwd = getcwd(wd, sizeof(wd)) ? std::string(wd) : "/";
}

Environment::~Environment() {}

bool Environment::setPath(const std::string& s) {
    std::string w;
    std::vector<std::string> v;
    std::stringstream ss(s);
    while(!ss.eof()) {
        std::getline(ss, w, ':');
        // w = resolvePath(w, cwd);
        if(isDirectory(w)) v.push_back(w);
        else return false;
    }
    path.clear();
    path = v;
    // put("PATH", s);
    return true;
}

bool Environment::setHome(const std::string& s) {
    // std::string w = resolvePath(s, cwd);
    if(isDirectory(s)) {
        home = s;
        // put("HOME", s);
        return true;
    }
    return false;
}

bool Environment::setCwd(const std::string& s) {
    char wd[PATH_MAX];
    if(chdir(s.c_str()) == 0) {
        if(getcwd(wd, sizeof(wd))) {
            cwd = std::string(wd);
            return true;
        }
    }
    return false;
}

// void Environment::put(const std::string& s1, const std::string& s2) {
//     std::regex r("^\\s*([^=]*)=\"?(.*)\"?\\s*$");
//     std::smatch m;
//     bool found = false;
//     std::string t = s1 + "=" + s2;
//     int i = 0;
//     for(std::string& s : vars) {
//         if(std::regex_match(s, m, r) && m[1] == s1) {
//             vars[i] = t;
//             found = true; 
//             break;
//         }
//     }
//     if(!found) vars.push_back(t);
// }

std::string* Environment::inPath(const std::string& s) const {
    std::string* p = nullptr;
    std::string t;
    for(std::string dir : path) {
        t = dir + "/" + s;
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

std::string Environment::getCwd() const {
    return cwd;
}

// const std::vector<std::string>& Environment::getVars() const {
//     return vars;
// }
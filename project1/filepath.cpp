#include "filepath.h"

bool check_file(const std::string& path, int code) {
    struct stat s;
    return((stat(path.c_str(),&s) == 0) && (s.st_mode & code));
}

bool isDirectory(const std::string& path) {
    return check_file(path, S_IFDIR);
}

bool isFile(const std::string& path) {
    return check_file(path, S_IFREG);
}

bool isExecutable(const std::string& path) {
    return check_file(path, S_IXUSR);
}

std::string resolvePath(const std::string& p, const std::string& wd) {
    if(p.empty()) throw(std::runtime_error("Given path cannot be empty for directory or file."));

    // trim leading and tailing ws
    std::string w;
    std::regex r("^\\s*(.*)\\s*$");
    std::smatch m;
    std::regex_match(p, m, r);
    w = m[1];

    if(w.front() == '/') w = "/" + resolveRel(w.substr(1), "");
    else w = resolveRel(w, wd);
    if(w.substr(0,2) == "//") w = w.substr(1);
    return w;
}

std::string resolveRel(const std::string& p, const std::string& wd) {
    if(p.empty()) return wd;
    std::regex r("^\\s*([^/]*)/(.*)$");
    std::smatch m;
    if(std::regex_match(p, m, r)) {
        std::string pre = m[1];
        std::string suff = m[2];
        if(pre.empty()) throw(std::runtime_error("Invalid file path syntax."));
        else if(pre == ".") return resolveRel(suff, wd);
        else if(pre == "..") return resolveRel(suff, parent(wd));
        else return resolveRel(suff, wd + "/" + pre);
    } else { 
        if(p == ".") return wd;
        else if(p == "..") return parent(wd);
        else if(wd == "") return p;
        // Should be a file name
        else return (wd + "/" + p);
    }
}

std::string parent(const std::string& p) {
    std::regex r("^\\s*(.*)/[^/]*/?\\s*$");
    std::smatch m;
    if(std::regex_match(p, m, r)) return m[1];
    else throw(std::runtime_error("Invalid file path syntax."));
}
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
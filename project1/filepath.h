#ifndef FILEPATH_H
#define FILEPATH_H

#include <string>
#include <sstream>
#include <regex>
#include <sys/stat.h>
#include <iostream>

bool check_file(const std::string& path, int code);
bool isDirectory(const std::string& path);
bool isFile(const std::string& path);
bool isExecutable(const std::string& path);

// Takes a path like './dir/file' or '../../dir/dir/file'
// and converts it to an absolute path
// Throws exception if invalid path
// If directory, appends '/' at the end
std::string resolvePath(const std::string& p, const std::string& wd);

std::string resolveRel(const std::string& p, const std::string& wd);

std::string parent(const std::string& p);

#endif
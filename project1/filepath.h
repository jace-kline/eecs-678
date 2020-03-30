#ifndef FILEPATH_H
#define FILEPATH_H

#include <string>
#include <sys/stat.h>

bool check_file(const std::string& path, int code);
bool isDirectory(const std::string& path);
bool isFile(const std::string& path);
bool isExecutable(const std::string& path);

#endif
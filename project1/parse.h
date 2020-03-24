#ifndef PARSE_H
#define PARSE_H
 
#include "types.h"
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>

// Each parse step will have the possiblility to 
// throw a std::runtime_error if construction not possible

const std::vector<std::string> builtins = {"cd", "exit", "quit", "set", "jobs", "kill"};
const std::string WHITESPACE = " \n\r\t\f\v";

template <typename T>
bool inVector(const T& element, const std::vector<T>& v);

Executable* constructExecutablePtr(const std::string& s);

Builtin* constructBuiltin(const std::string& s);

Pipeline* constructPipeline(const std::string& s);

std::string trimLeadingWhitespace(const std::string& s);

std::string trimTrailingWhitespace(const std::string& s);

std::string getFirstWord(const std::string& s);

Maybe<FilePath> parseRedir(RedirType t, Command& c);

#endif
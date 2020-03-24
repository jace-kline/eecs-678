#ifndef USER_INTERACT_H
#define USER_INTERACT_H

#include <unistd.h>
#include <poll.h>

#include <string>
#include <iostream>

std::string promptReadStdinLine();
bool stdinEmpty();

#endif
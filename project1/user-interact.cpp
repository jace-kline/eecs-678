#include "user-interact.h"

std::string promptReadStdinLine() {
    if(stdinEmpty()) {
        std::cout << "quash> ";
    }
    std::string s;
    std::getline(std::cin, s);
    return s;
}

bool stdinEmpty() {
    struct pollfd poller {
        .fd = STDIN_FILENO,
        .events = POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI
    };
    return(!(poll(&poller, 1, 0) == 1));
}
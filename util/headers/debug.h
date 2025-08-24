#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <cstdlib>

void inline print(const std::string& msg) {
    std::cout << msg << "\n" << std::endl;
}

void inline exit() {
    std::exit(0);
}

#endif // DEBUG_H
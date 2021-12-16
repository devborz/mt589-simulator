#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <cmath>
#include <iostream>

int fromStringBin(std::string input) {
    if (input.size() == 0) { return 0; }
    int result = 0;
    for (int i = input.size() - 1; i >= 0; --i) {
        int number = input[i] - '0';
        result += std::pow(2, i) * number;
    }
    return result;
}


#endif // UTIL_HPP

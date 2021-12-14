#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <cmath>
#include <iostream>

int fromStringBin(std::string input) {
    int result = 0;
    for (size_t i = input.size() - 1; i >= 0; --i) {
        std::string ch = "";
        ch += input[i];
        std::cout << ch << std::endl;
        int number = std::stoi(ch);
        result += std::pow(2, i) * number;
    }
    return result;
}


#endif // UTIL_HPP

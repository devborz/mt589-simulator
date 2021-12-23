#ifndef UTIL_HPP
#define UTIL_HPP

#include <ListModels.h>
#include <string>
#include <cmath>
#include <iostream>
#include <bitset>
#include <vector>
#include <map>

void fillArrays(std::vector<std::string>& mnemonics,
                            std::vector<std::string>& funcs,
                             std::vector<std::string>& ks);

std::bitset<7> getFunc(const std::string& mnemonic);

void getRGroupRegs(int r_group, std::vector<std::string>& regs, std::vector<std::string>& addresses);

int fromStringBin(std::string input) {
    if (input.size() == 0) { return 0; }
    int result = 0;
    for (int i = input.size() - 1; i >= 0; --i) {
        int number = input[i] - '0';
        result += std::pow(2, i) * number;
    }
    return result;
}

std::bitset<2> getFromFIC(const std::string& mnemonic) {
    std::string value = "";
    if (mnemonic == "SCZ" ) {
        value = "00";
    } else if (mnemonic == "STZ" ) {
        value = "10";
    } else if (mnemonic == "STC" ) {
        value = "01";
    } else {
        value = "11";
    }
    return std::bitset<2>(value);
}

std::bitset<2> getFromFOC(const std::string& mnemonic) {
    std::string value = "";
    if (mnemonic == "FFO" ) {
        value = "00";
    } else if (mnemonic == "FFC" ) {
        value = "10";
    } else if (mnemonic == "FFZ" ) {
        value = "01";
    } else {
        value = "11";
    }
    return std::bitset<2>(value);
}

std::bitset<7> getFromJump(const std::string& mnemonic) {
    if (mnemonic == "JCC") {
        return std::bitset<7>("0000000");
    } else if (mnemonic == "JZR") {
        return std::bitset<7>("0100000");
    } else if (mnemonic == "JCR") {
        return std::bitset<7>("0110000");
    } else if (mnemonic == "JCE") {
        return std::bitset<7>("1110000");
    } else if (mnemonic == "JFL") {
        return std::bitset<7>("1000000");
    } else if (mnemonic == "JCF") {
        return std::bitset<7>("1010000");
    } else if (mnemonic == "JZF") {
        return std::bitset<7>("1011000");
    } else if (mnemonic == "JPR") {
        return std::bitset<7>("1100000");
    } else if (mnemonic == "JLL") {
        return std::bitset<7>("1101000");
    } else if (mnemonic == "JRL") {
        return std::bitset<7>("1111100");
    } else { //(mnemonic == "JPX")
        return std::bitset<7>("1111000");
    }
}


#endif // UTIL_HPP

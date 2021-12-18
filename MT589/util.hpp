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
int getRGroup(int index) {
  std::vector<int> r_groups = {
    1,
    2,
    3,
    1,
    2,
    1,
    2,
    3,
    1,
    2,
    3,
    1,
    2,
    1,
    2,
    3,
    1,
    3,
    1,
    3,
    1,
    2,
    1,
    2,
    3,
    1,
    2,
    3,
    1,
    2,
    1,
    2,
    3,
    1,
    2,
    3,
    1,
    2,
    3
  };

  return r_groups[index];
}

std::bitset<7> getFromFunc(const std::string& mnemonic) {
    std::bitset<7> func(getFunc(mnemonic));
    return func;
}

std::bitset<4> getFromReg(const std::string& mnemonic, int rGroup) {
    std::vector<std::string> mnemonics;
    std::vector<std::string> addresses;
    getRGroupRegs(rGroup, mnemonics, addresses);
    for (size_t i = 0; i < mnemonics.size(); ++i) {
        if (mnemonics[i] == mnemonic) {
            return std::bitset<4>(addresses[i]);
        }
    }
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
    } else {
        return std::bitset<7>("1110000");
    }
}

std::bitset<7> getFunc(const std::string& mnemonic) {
    std::vector<std::string> mnemonics;
    std::vector<std::string> funcs;
    std::vector<std::string> ks;
    fillArrays(mnemonics, funcs, ks);
    for (size_t i = 0; i < mnemonics.size(); ++i) {
        if (mnemonics[i] == mnemonic) {
            return std::bitset<7>(funcs[i]);
        }
    }
}

std::string getKFromFunc(int index) {
    std::vector<std::string> mnemonics;
    std::vector<std::string> funcs;
    std::vector<std::string> ks;
    fillArrays(mnemonics, funcs, ks);
    return ks[index];
}



void fillArrays(std::vector<std::string>& mnemonics,
                            std::vector<std::string>& funcs,
                             std::vector<std::string>& ks) {
    mnemonics = {
      "ILR",
      "ACM",
      "SRA",
      "ALR",
      "AMA",
      "LMI",
      "LMM",
      "CIA",
      "DSM",
      "LDM",
      "DCA",
      "CSR",
      "CSA",
      "SDR",
      "SDA",
      "LDI",
      "INR",
      "INA",
      "ADR",
      "AIA",
      "CLR",
      "CLA",
      "ANR",
      "ANM",
      "ANI",
      "TZR",
      "LTM",
      "TZA",
      "NOP",
      "LMF",
      "ORR",
      "ORM",
      "ORI",
      "CMR",
      "LCM",
      "CMA",
      "XNR",
      "XNM",
      "XNI"
    };
    funcs = {
      "0001001",
      "0001010",
      "0001110",
      "0001001",
      "0001010",
      "0011001",
      "0011010",
      "0011110",
      "0011001",
      "0011010",
      "0011110",
      "0101001",
      "0101010",
      "0101001",
      "0101010",
      "0101110",
      "0111001",
      "0111110",
      "0111001",
      "0111110",
      "1001001",
      "1001010",
      "1001001",
      "1001010",
      "1001110",
      "1011001",
      "1011010",
      "1011110",
      "1101001",
      "1101010",
      "1101001",
      "1101010",
      "1101110",
      "1101001",
      "1101010",
      "1101110",
      "1101001",
      "1101010",
      "1101110"
    };
    ks = {
      "00",
      "00",
      "00",
      "11",
      "11",
      "00",
      "00",
      "00",
      "11",
      "11",
      "11",
      "00",
      "00",
      "11",
      "11",
      "11",
      "00",
      "00",
      "11",
      "11",
      "00",
      "00",
      "11",
      "11",
      "11",
      "11",
      "11",
      "11",
      "00",
      "00",
      "11",
      "11",
      "11",
      "00",
      "00",
      "00",
      "11",
      "11",
      "11"
    };
}

void getRGroupRegs(int r_group, std::vector<std::string>& regs, std::vector<std::string>& addresses) {
    switch (r_group) {
    case 1: {
        regs = {
            "RO",
            "R1",
            "R2",
            "R3",
            "R4",
            "R5",
            "R6",
            "R7",
            "R8",
            "R9",
            "T",
            "AC"
        };
        addresses = {
            "0000",
            "0001",
            "0010",
            "0011",
            "0100",
            "0101",
            "0110",
            "0111",
            "1000",
            "1001",
            "1100",
            "1101"
        };
        return;
    }
    case 2: {
        regs = {
            "T",
            "AC"
        };
        addresses = {
            "1010",
            "1011"
        };
        return;
    }
    default: {
        regs = {
            "T",
            "AC"
        };
        addresses = {
            "1110",
            "1111"
        };
    }
    }
}


#endif // UTIL_HPP

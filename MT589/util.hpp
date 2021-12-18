#ifndef UTIL_HPP
#define UTIL_HPP

//#include <ListModels.h>
#include <string>
#include <cmath>
#include <iostream>
#include <bitset>
#include <vector>
#include <map>

int fromStringBin(std::string input) {
    if (input.size() == 0) { return 0; }
    int result = 0;
    for (int i = input.size() - 1; i >= 0; --i) {
        int number = input[i] - '0';
        result += std::pow(2, i) * number;
    }
    return result;
}
int getRGroup(const std::string& mnemonic) {
  std::vector<std::string> mnemonics = {
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

  for (size_t i = 0; i < mnemonics.size(); ++i) {
      if (mnemonics[i] == mnemonic) {
          return r_groups[i];
      }
  }
}

std::bitset<3> getFromFunc(const std::string& mnemonic) {

}

std::bitset<4> getFromReg(const std::string& mnemonic, int rGroup) {

}

std::bitset<2> getFromFIC(const std::string& mnemonic) {

}

std::bitset<2> getFromFOC(const std::string& mnemonic) {

}

std::bitset<7> getFromJump(const std::string& mnemonic) {

}


#endif // UTIL_HPP

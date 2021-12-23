#ifndef ROM_H
#define ROM_H

#include "mt_global.h"

struct microcommand {
    bool empty = true;
    // to mcu
    std::bitset<7> AC;
    std::bitset<8> X;
    BYTE FC : 4;
    // to cpe
    std::bitset<7> F = {0b1100000};
    BYTE K;
    //[[maybe_unused]] BYTE CI : 1;
    //[[maybe_unused]] BYTE LI : 1;
    [[maybe_unused]] BYTE M;
    [[maybe_unused]] BYTE I;
    //// to ram
    BYTE RAMC : 2;
    ////  UI HELPERS
    int index_F;
    int index_REG;
    int index_FIC;
    int index_FOC;
    int index_Jump;
    std::string address_control = "0000000";
};

struct ROM
{
    ROM();
    void write(size_t row, size_t col, microcommand cmd);
    microcommand read(size_t row, size_t col);
    bool is_nop(size_t row, size_t col);

    std::vector< std::vector<microcommand> > memory;
    const size_t _rows = 0x20; // 32
    const size_t _cols = 0x10; // 16
};

#endif // ROM_H

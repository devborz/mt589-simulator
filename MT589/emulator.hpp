#ifndef MK589_HPP
#define MK589_HPP

#include "cpe.h"
#include "mcu.h"
#include "rom.h"
#include "ram.h"

// Microcomputer with microcommand control (by default)
class MK589
{
public:
    MK589();
    ROM rom {}; // rom with microprogramm
    RAM ram {}; // read/write memory

    void load(std::bitset<8> x);
    void fetch(const microcommand& mc);
    void decode();


    // for step
     void execute();
    // for run button
    void execute_microprogram();
    //=====MEMORY
    BYTE MEM[0xC];
    BYTE MAR;
    //=====OUTPUTS
    BYTE D; // cpe
    BYTE A; // cpe

    BYTE FI : 1; // FI - is CO or RO

    size_t get_row_adr(); // mcu
    size_t get_col_adr(); // mcu
//private:
    //===============MCU
    MCU mcu {};

    void fetch_mcu(std::bitset<7> AC, std::bitset<8> X, BYTE FC);
    void decode_adr();
    //=====INPUTS
    std::bitset<7> _AC;
    std::bitset<8> X;
    BYTE FC : 4;
    // decoding
    size_t col_adr;
    size_t row_adr;
    //==================

    //===============CPE
    void fetch_cpe(std::bitset<7> f,
                            BYTE k,
                            BYTE i,
                            BYTE m);
    void decode_cpe();
    void execute_cpe();
    void execute_cpe_right_rot();
    void unite_registers();

    size_t cpe_amount = 4;
    std::vector<CPE> cpe_arr;
    //=====INPUTS
    std::bitset<7> F;
    BYTE K;

    BYTE CO : 1;
    BYTE RO : 1;
    BYTE FO : 1; // FO is CI or LI
    [[maybe_unused]] BYTE CI : 1; // only affects lowest 2bits (cpe_arr[0])
    [[maybe_unused]] BYTE LI : 1; // only affects highest 2bits (cpe_arr[cpe_amount - 1])
    [[maybe_unused]] BYTE I;
    [[maybe_unused]] BYTE M;

    // decoding
    size_t r_group;
    size_t f_group;
    BYTE ADR : 4;
    bool is_performing_right_rot = false;
    //==================
};

#endif // MK589_HPP

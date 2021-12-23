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
    size_t cpe_amount = 4;
    std::vector<CPE> cpe_arr;
    ROM rom {}; // rom with microprogramm
    RAM ram {}; // read/write memory
    MCU mcu {};
    // MC functions
    void reset();
    void load(std::bitset<8> x);
    void decode();
    void do_fetch_decode_execute_cycle(const microcommand& mc);
    // MCU
    // INPUTS
    BYTE FI : 1; // FI - is CO or RO
    void decode_adr();
    // OUTPUTS
    BYTE FO : 1; // FO is CI or LI
    size_t get_row_adr(); // mcu
    size_t get_col_adr(); // mcu
    // decoding
    size_t col_adr;
    size_t row_adr;
    // CPE_ARRAY
    //=====INPUTS
    std::bitset<7> F;
    BYTE K;
    BYTE CI : 1; // only affects lowest 2bits (cpe_arr[0])
    BYTE LI : 1; // only affects highest 2bits (cpe_arr[cpe_amount - 1])
    BYTE I; // from IO devices
    BYTE M; // from RAM
    //=====MEMORY
    BYTE MEM[0xC];
    BYTE MAR;
    //=====OUTPUTS
    BYTE CO : 1;
    BYTE RO : 1;
    BYTE D; // cpe
    BYTE A; // cpe
    void fetch_cpe(std::bitset<7> f,
                            BYTE k,
                            BYTE i,
                            BYTE m);
    void decode_cpe();
    void execute_cpe();
    void execute_cpe_right_rot();
    void unite_registers();
    // decoding
    size_t r_group;
    size_t f_group;
    BYTE ADR : 4;
    bool is_performing_right_rot = false;
};
#endif // MK589_HPP

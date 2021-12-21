#ifndef MCU_H
#define MCU_H

#include "mt_global.h"

class MCU
{
public:
    MCU();
    void load(std::bitset<8> x); // emulation of strob signal LD -> X lines loads into MPAR
    void fetch(std::bitset<7> ac, std::bitset<8> x, BYTE fi, BYTE fc );
    void decode();
    void execute();

    //======INPUTS
    BYTE FI : 1; // flag input

    //======OUTPUTS
    BYTE FO : 1;
    BYTE PR : 3;
    std::bitset<9> MA;

//private:
    void decode_jmp();
    void decode_fl(); // decode flag logic

    void execute_flag_logic();
    void compute_next_addr();

    //======INPUTS
    BYTE FC_10 : 2; // flag input control
    BYTE FC_32 : 2; // flag output control
    std::bitset<7> AC; // address control command
    std::bitset<8> X; // 4-7 = PX, 0-3 = SX
    //======

    //======MEMORY
    BYTE TF : 1; // flag latch
    BYTE TC : 1; // CO trigger (lathc)
    BYTE TZ : 1; // ZERO flag latch (trigger)
    BYTE PR_latch : 4; // aka command register
    std::bitset<9> MPAR;

    enum class FCI {
        SCZ = 0,
        STZ,
        STC,
        HCZ
    };
    enum class FCO {
        FF0 = 0,
        FFC = 1,
        FFZ = 2,
        FF1 = 3
    };
    FCI cur_FCI;
    FCO cur_FCO;

    enum class JUMP {
        JCC = 0,
        JZR,
        JCR,
        JCE,
        JFL,
        JCF,
        JZF,
        JPR,
        JLL,
        JRL,
        JPX
    };
    JUMP cur_jmp;

    static constexpr size_t total_jmps = 11;
    BYTE _signals[total_jmps] = {
        0b0011111, // JCC
        0b0101111, // JZR
        0b0111111, // JCR
        0b1110111, // JCE
        0b1001111,
        0b1010111,
        0b1011111,
        0b1100111,
        0b1101111,
        0b1111111,
        0b1111011
    };
};

#endif // MCU_H

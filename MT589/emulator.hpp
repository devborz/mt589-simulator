#ifndef MK589_HPP
#define MK589_HPP

#include <array>
#include <bitset>
#include <vector>

using BYTE = uint8_t;
using WORD = uint16_t;

constexpr BYTE AC = 0xB;
constexpr BYTE T = 0xA;

// r group    | F GROUP
// F0,F1,F2,F3, F4,F5,F6
class CPE
{
public:
    CPE();
    void fetch(const std::bitset<7>& f, BYTE i, BYTE k, BYTE m, BYTE CI,
            BYTE LI);
    void decode();
    void execute();
    void reset();

    //======OUTPUT
    BYTE A : 2; // MAR output
    BYTE D : 2; // AC output
    BYTE RO : 1;
    BYTE CO : 1;
    BYTE X : 1;
    BYTE Y : 1;
    //======MEMORY
    BYTE MAR : 2;
    BYTE MEM[0xC];

    //======MISC
    int r_group;
    int f_group;
    BYTE ADR : 4;

private:
    void propogate();
    void compute_CO();
    // Commands
    void execute_f0();
    void f_group1();
    void f_group2();
    void execute_f3();
    void execute_f4();
    void execute_f5();
    void execute_f6();
    void execute_f7();
    // Utility
    bool is_RO_out = false; // CO and RO - tri-state
    bool is_D_out = false; // D - tri-state
    bool is_A_out = false; // A - tri-state

    BYTE word_wise_or(BYTE op);
    BYTE get_lb(BYTE src);
    BYTE get_hb(BYTE src);

    //======INPUT
    std::bitset<7> F;
    BYTE I : 2;
    BYTE K : 2;
    BYTE CI : 1;
    BYTE LI : 1;
    BYTE M : 2;
    //======

    //======MISC
    BYTE BUF1 : 1;
    BYTE BUF2 : 2;
    BYTE opA : 2;
    BYTE opB : 2;
    //======
};

class FCC
{
public:
    FCC();
};

class MCU
{
public:
    MCU();
    void load(); // emulation of strob signal LD -> X lines loads into MPAR
    void fetch(std::bitset<7> ac, std::bitset<8> x, BYTE fi, BYTE fc );
    void decode();
    void execute();

    //======INPUTS
    BYTE FI : 1; // flag input

    //======OUTPUTS
    BYTE FO : 1;
    BYTE PR : 3;
    std::bitset<9> MA;

private:
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
        FF1 = 0,
        FFC,
        FFZ,
        FF0
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

struct microcommand {
    bool empty = true;
    // to mcu
    std::bitset<7> AC;
    std::bitset<8> X;
    BYTE FC : 4;
    // to cpe
    std::bitset<7> F = {0b1100000};
    BYTE K;
    [[maybe_unused]] BYTE CI : 1;
    [[maybe_unused]] BYTE LI : 1;
    [[maybe_unused]] BYTE M;
    [[maybe_unused]] BYTE I;
    // to ram

    //  UI HELPERS
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

struct RAM {
    RAM();
    void write(size_t address, BYTE data);
    BYTE read(size_t address);

    size_t size = 0xFF;
    std::vector<BYTE> memory;
};

// Microcomputer with microcommand control (by default)
class MK589
{
public:
    MK589();
    ROM rom {}; // rom with microprogramm
    RAM ram {}; // read/write memory

    void load();
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
    size_t get_row_adr(); // mcu
    size_t get_col_adr(); // mcu
private:
    //===============MCU
    MCU mcu {};

    void fetch_mcu(std::bitset<7> AC, std::bitset<8> X, BYTE FC);
    void decode_adr();
    //=====INPUTS
    std::bitset<7> _AC;
    std::bitset<8> X;
    BYTE FC : 4;
    BYTE FI : 1; // FI - is CO or RO
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

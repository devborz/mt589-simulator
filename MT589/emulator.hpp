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
struct CPE
{
    CPE();
    void fetch(const std::bitset<7>& f, BYTE i, BYTE k, BYTE m, BYTE CI,
            BYTE RI);
    void decode();
    void execute();

    void propogate();
    void compute_CO();

    void reset();

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
    bool is_RO_out = false;
    bool is_D_out = false;
    bool is_A_out = false;
    const BYTE word_wise_or(BYTE op);
    const BYTE get_lb(BYTE src);
    const BYTE get_hb(BYTE src);
    //======INPUT
    std::bitset<7> F;
    BYTE I : 2;
    BYTE K : 2;
    BYTE CI : 1;
    BYTE LI : 1;
    BYTE M : 2;
    //======

    //======MEMORY
    BYTE MAR : 2;
    BYTE MEM[0xC];
    //======

    //======OUTPUT
    BYTE A : 2; // MAR output
    BYTE D : 2; // AC output
    BYTE RO : 1;
    BYTE CO : 1;
    BYTE X : 1;
    BYTE Y : 1;
    //======

    //======MISC
    int r_group;
    int f_group;
    BYTE ADR : 4;
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

struct microcommand {
    std::bitset<7> F = {0b1100000};
    BYTE FC : 4;
    std::bitset<7> AC;
    BYTE K;
    //BYTE RAM_ACCESS : 1;
    //BYTE WR : 2;
    BYTE CI : 1;
    BYTE M;

    bool empty = true;
    // Input indexes
    int index_F;
    int index_REG;
    int index_FIC;
    int index_FOC;
    int index_Jump;
    std::string address_control = "0000000";
};

template<std::size_t N>
void reverse(std::bitset<N> &b) {
    for(std::size_t i = 0; i < N/2; ++i) {
        bool t = b[i];
        b[i] = b[N-i-1];
        b[N-i-1] = t;
    }
}

struct MCU
{
    MCU();
    void load(); // emulation of strob signal LD -> X lines loads into MPAR
    void connect_data(std::bitset<7> ac, std::bitset<8> x, BYTE fi, BYTE fc );
    void decode_jmp();
    void decode_fl(); // decode flag logic
    void do_flag_logic();
    void compute_next_addr();
    void execute();

    //======INPUTS
    BYTE FI : 1; // flag input
    BYTE FC_10 : 2; // flag input control
    BYTE FC_32 : 2; // flag output control
    std::bitset<7> AC; // address control command

    // ЭТО ИГРУШКА ДbЯВОЛА
    std::bitset<8> X; // 4-7 = PX, 0-3 = SX
    //======

    //======MEMORY
    BYTE TF : 1; // flag latch
    BYTE TC : 1; // CO trigger (lathc)
    BYTE TZ : 1; // ZERO flag latch (trigger)
    BYTE PR_latch : 4; // aka command register
    std::bitset<9> MPAR;
    //======

    //======OUTPUTS
    BYTE FO : 1;
    BYTE PR : 3;
    std::bitset<9> MA;
    //======

    //======MISC
    void decode_adr();
    size_t get_row_adr();
    size_t get_col_adr();
    size_t col_adr;
    size_t row_adr;

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
    //======
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

class MK589
{
public:
    MK589();
    std::vector<CPE> cpe_arr;
    MCU mcu {};
    ROM rom {};
    RAM ram {};

    size_t cpe_amount = 4;

    //=====MEMORY
    BYTE MEM[0xC];
    BYTE MAR;

    //=====OUTPUT
    BYTE CO : 1;
    BYTE RO : 1;

    //=====CPE
    size_t r_group;
    size_t f_group;
    BYTE ADR : 4;

    void execute_microprogram();
    void execute_cpe(std::bitset<7> F, BYTE K, BYTE I, BYTE M, BYTE CI);

    void unite_registers();
};

#endif // MK589_HPP

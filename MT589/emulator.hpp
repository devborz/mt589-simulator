#ifndef MK589_HPP
#define MK589_HPP

#include <vector>

using BYTE = uint8_t;
constexpr BYTE AC = 0xB;
constexpr BYTE T = 0xA;

const get_lb(BYTE src) {
    BUF1 = src & 0b01;
    return BUF1;
}
const get_hb(BYTE src) {
    BUF1 = (src >> 1) & 0b01;
    return BUF1;
}
// r group    | F GROUP
// F0,F1,F2,F3, F4,F5,F6

struct CPE
{
    CPE();
    void fetch(const std::vector<BYTE>& f, BYTE i, BYTE k, BYTE m, BYTE CI,
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

    // Memory
    BYTE MAR : 2;
    BYTE MEM[0xC];

    // inputs
    std::vector<BYTE> F;
    BYTE I : 2;
    BYTE K : 2;
    BYTE CI : 1;
    BYTE RI : 1;
    BYTE M : 2;

    // outputs
    BYTE A : 2; // MAR output
    BYTE D : 2; // AC output
    BYTE RO : 1;
    BYTE CO : 1;
    BYTE X : 1;
    BYTE Y : 1;
    
    // r and f groups after decoding of microcommand
    int r_group;
    int f_group;

    // used for addressing registers in r-group 1 and for adress of AT in r-group 2 and 3
    BYTE ADR : 4; 
    // 2bit buffer
    BYTE BUF1 : 1;
    BYTE BUF2 : 2;
    // 2 bit operands
    BYTE opA : 2;
    BYTE opB : 2;
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
};

class ROM
{
public:
    ROM();
};

class mk589
{
public:
    mk589();
    void execute();
    
};

#endif // MK589_HPP

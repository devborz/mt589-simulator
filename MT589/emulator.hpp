#ifndef MK589_HPP
#define MK589_HPP

#include <vector>

using BYTE = uint8_t;
constexpr BYTE AC = 0xB;
constexpr BYTE T = 0xA;

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

    // Commands
    void execute_f0();
    void f_group1();
    void f_group2();
    void f_group3();
    void f_group4();
    void f_group5();
    void f_group6();
    void f_group7();

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
    
    // utility fields
    int r_group;
    int f_group;

    BYTE ADR : 4;
    BYTE BUF : 2; // 2bit buffer

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

#ifndef CPE_H
#define CPE_H

#include "mt_global.h"

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

//private:
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

#endif // CPE_H

#include "emulator.hpp"

CPE::CPE() {
    this->reset();
}

void CPE::reset() {
    for (size_t i = 0x0; i < 0xE; ++i) {
        CPE::MEM[i] = 0b00;
    }
    MAR = 0b00;
}

const BYTE CPE::word_wise_or(BYTE op) {
    return get_lb(op) | get_hb(op);
}

const BYTE CPE::get_lb(BYTE src) {
    BUF1 = src & 0b01;
    return BUF1;
}
const BYTE CPE::get_hb(BYTE src) {
    BUF1 = (src >> 1) & 0b01;
    return BUF1;
}

void CPE::fetch(const std::vector<BYTE>& f, BYTE i, BYTE k, BYTE m, BYTE CI, BYTE LI) {
    this->F = f;
    this->I = i;
    this->K = k;
    this->CI = CI;
    this->LI = LI;
    this->M = m;
}

void CPE::decode() {
    f_group = 0b000;
    f_group |= (F[6] & 0b1) << 2;
    f_group |= (F[5] & 0b1) << 1;
    f_group |= (F[4] & 0b1) << 0;

    r_group = 0b0000;
    r_group |= (F[3] & 0b1) << 3;
    r_group |= (F[2] & 0b1) << 2;
    r_group |= (F[1] & 0b1) << 1;
    r_group |= (F[0] & 0b1) << 0;

    if (r_group == 0xA or r_group == 0xB) {
        ADR = r_group;
        r_group = 2;
    } else if (r_group == 0xE or r_group == 0xF) {
        ADR = r_group - 4;
        r_group = 3;
    } else {
        if (r_group == 0xC or r_group == 0xD) {
            ADR = r_group - 2;
        } else {
            ADR = r_group;
        }
        r_group = 1;
    }
}
void CPE::execute() {
    switch(f_group) {
        case 0:
            execute_f0(); 
            break;
        case 1:
            f_group1(); 
            break;
        case 2:
            f_group2(); 
            break;
        case 3:
            execute_f3(); 
            break;
        case 4:
            execute_f4(); 
            break;
        case 5:
            execute_f5(); 
            break;
        case 6:
            execute_f6(); 
            break;
        case 7:
            execute_f7(); 
            break;
    }
    // set buses output
    A = MAR;
    D = MEM[AC];
}
void CPE::propogate() {
    if (f_group > 3) {
        Y = 1;
        X = CO;
    } else {
        BYTE a1 = (opA >> 1) & 0b1;
        BYTE a0 = opA & 0b1;
        BYTE b0 = opB & 0b1;
        BYTE b1 = (opB >> 1) & 0b1;
        X = (a1 & b1) | (a0 | b0);
        Y = (a1 & b1) | ((a1 | b1) & (a0 | b0));
    }
}
void CPE::compute_CO() {
    CO = (CI & Y) | (X & Y);
}
void CPE::execute_f0() {
    switch(r_group) {
        case 1:
            opA = MEM[ADR];
            opB = (MEM[AC] & K);
            propogate();
            compute_CO();
            BUF2 = opA + opB + CI;
            MEM[ADR] = BUF2;
            MEM[AC] = BUF2;
            break;
        case 2:
            opA = M;
            opB = (MEM[AC] & K);
            propogate();
            compute_CO();
            BUF2 = opA + opB + CI;
            MEM[ADR] = BUF2;
            break;
        case 3:
            // CO? XY? => inclompleted
            RO = get_lb(MEM[ADR]) & (get_lb(I) & get_lb(K)); // inverse before 2nd brackets
            BUF1 = (get_lb(MEM[ADR]) & (get_lb(I) & get_lb(K))) | (get_hb(MEM[ADR]) | (get_hb(I) & get_hb(K)));
            MEM[ADR] = MEM[ADR] & BUF1;
            BUF1 = LI | ((get_hb(I) & get_hb(K)) & get_hb(MEM[ADR]));
            MEM[ADR] = MEM[ADR] | (BUF1 << 1);
            break;
    }
}
void CPE::f_group1() {
    switch(r_group) {
        case 1:
            MAR = K | MEM[ADR];
            opA = MEM[ADR];
            opB = K;
            propogate();
            compute_CO();
            BUF2 = opA + opB + CI;
            MEM[ADR] = BUF2;
            break;
        case 2:
            MAR = K | M;
            opA = M;
            opB = K;
            propogate();
            compute_CO();
            BUF2 = opA + opB + CI;
            MEM[ADR] = BUF2;
            break;
        case 3:
            // CO? XY? => inclompleted
            BUF2 = (~MEM[ADR] | K) + (MEM[ADR] & K) + CI;
            MEM[ADR] = BUF2;
            break;
    }
}
// how to propogate and compute carry out?????
void CPE::f_group2() {
    switch(r_group) {
        case 1:
            BUF2 = (MEM[AC] & K) - 1 + CI;
            MEM[ADR] = BUF2;
            break;
        case 2:
            BUF2 = (MEM[AC] & K) - 1 + CI;
            MEM[ADR] = BUF2;
            break;
        case 3:
            BUF2 = (I & K) - 1 + CI;
            MEM[ADR] = BUF2;
            break;
    }
}
void CPE::execute_f3() {
    switch(r_group) {
        case 1:
            opA = MEM[ADR];
            opB = (MEM[AC] & K);
            propogate();
            compute_CO();
            BUF2 = opA + opB + CI;
            MEM[ADR] = BUF2;
            break;
        case 2:
            opA = M;
            opB = (MEM[AC] & K);
            propogate();
            compute_CO();
            BUF2 = opA + opB + CI;
            MEM[ADR] = BUF2;
            break;
        case 3:
            opA = MEM[ADR];
            opB = I & K;
            propogate();
            compute_CO();
            BUF2 = opA + opB + CI;
            MEM[ADR] = BUF2;
            break;
    }
}
void CPE::execute_f4() {
    switch(r_group) {
        case 1:
            BUF1 = CI | word_wise_or(MEM[ADR] & MEM[AC] & K);
            BUF2 = MEM[ADR] & (MEM[AC] & K);
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
        case 2:
            BUF1 = CI | word_wise_or(M & MEM[AC] & K);
            BUF2 = M & (MEM[AC] & K);
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
        case 3:
            BUF1 = CI | word_wise_or(I & MEM[ADR] & K);
            BUF2 = MEM[ADR] & (I & K);
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
    }
}
void CPE::execute_f5() {
    switch(r_group) {
        case 1:
            BUF1 = word_wise_or(MEM[ADR] & K) | CI;
            BUF2 = MEM[ADR] & K;
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
        case 2:
            BUF1 = word_wise_or(M & K) | CI;
            BUF2 = M & K;
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
        case 3:
            BUF1 = word_wise_or(MEM[ADR] & K) | CI;
            BUF2 = MEM[ADR] & K;
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
    }
}
void CPE::execute_f6() {
    switch(r_group) {
        case 1:
            BUF1 = word_wise_or(MEM[AC] & K) | CI;
            BUF2 = (MEM[AC] & K) | MEM[ADR];
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
        case 2:
            BUF1 = word_wise_or(MEM[AC] & K) | CI;
            BUF2 = (MEM[AC] & K) | M;
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
        case 3:
            BUF1 = word_wise_or(I & K) | CI;
            BUF2 = (I & K) | MEM[ADR];
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
    }
}
void CPE::execute_f7() {
    switch(r_group) {
        case 1:
            BUF1 = word_wise_or(MEM[AC] & MEM[ADR] & K) | CI;
            BUF2 = ~((MEM[AC] & K) ^ MEM[ADR]);
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
        case 2:
            BUF1 = word_wise_or(MEM[AC] & M & K) | CI;
            BUF2 = ~((MEM[AC] & K) ^ M);
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
        case 3:
            BUF1 = word_wise_or(MEM[ADR] & I & K) | CI;
            BUF2 = ~((I & K) ^ MEM[ADR]);
            MEM[ADR] = BUF2;
            CO = BUF1;
            propogate();
            break;
    }
}

mk589::mk589()
{

}

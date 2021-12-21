#include "mcu.h"

MCU::MCU() {
    TF = 0b0;
    TC = 0b0;
    TZ = 0b0;
    PR_latch = 0b0000;
    MPAR = 0b000000000;
}

void MCU::load(std::bitset<8> x) {
    this->X = x;
    MPAR[8] = 0;
    for (size_t i = 0; i < 4; ++i) {
        MPAR[i] = X[i + 4];
    }
    for (size_t i = 4; i < 8; ++i) {
        MPAR[i] = X[i - 4];
    }
    MA = MPAR;
}
void MCU::fetch(std::bitset<7> ac, std::bitset<8> x, BYTE fi, BYTE fc ) {
    this->AC = ac;
    this->X = x;
    this->FC_10 = fc & 0b0011;
    this->FC_32 = (fc >> 2) & 0b11;
    this->FI = fi;
}
void MCU::decode_jmp() {
    for (size_t i = 0; i < total_jmps; ++i ){
        if ((AC & std::bitset<7> {_signals[i]}) == AC) {
            cur_jmp = static_cast<JUMP>(i);
            break;
        }
    }
}
void MCU::decode_fl() {
    cur_FCI = static_cast<FCI>(FC_10);
    cur_FCO = static_cast<FCO>(FC_32);
    switch (FC_32) {
    case 0: {
        cur_FCO = FCO::FF0;
        break;
    }
    case 1: {
        cur_FCO = FCO::FFC;
        break;
    }
    case 2: {
        cur_FCO = FCO::FFZ;
        break;
    }
    case 3: {
        cur_FCO = FCO::FF1;
        break;
    }
    }
}
void MCU::execute_flag_logic() {
    TF = FI;
    switch(cur_FCI) {
        case FCI::SCZ:
            TC = TF;
            TZ = TF;
            break;
        case FCI::STZ:
            TZ = TF;
            break;
        case FCI::STC:
            TC = TF;
            break;
        case FCI::HCZ:
            break;
    }

    switch(cur_FCO) {
        case FCO::FF0:
            FO = 0b0;
            break;
        case FCO::FFC:
            FO = TC;
            break;
        case FCO::FFZ:
            FO = TZ;
            break;
        case FCO::FF1:
            FO = 0b1;
            break;
    }
}

void MCU::compute_next_addr() {
    switch(cur_jmp) {
        case JUMP::JCC:
            for (size_t i = 4; i < 9; ++i) {
                MPAR[i] = AC[i - 4];
            }

            break;
        case JUMP::JZR:
            for (size_t i = 0; i < 9; ++i) {
                MPAR[i] = 0;
            }
            for (size_t i = 0; i < 4; ++i) {
                MPAR[i] = AC[i];
            }
            break;
        case JUMP::JCR:
            for (size_t i = 0; i < 4; ++i) {
                MPAR[i] = AC[i];
            }

            break;
        case JUMP::JCE:
            MPAR[4] = AC[0];
            MPAR[5] = AC[1];
            MPAR[6] = AC[2];
            break;
        case JUMP::JFL:
            break;
        case JUMP::JCF:
            break;
        case JUMP::JZF:
            break;
        case JUMP::JPR:
            break;
        case JUMP::JLL:
            break;
        case JUMP::JRL:
            break;
        case JUMP::JPX:
            break;
    }
    MA = MPAR;
}
void MCU::decode() {
    decode_jmp();
    decode_fl();
}

void MCU::execute() {
    // order?
    execute_flag_logic();
    compute_next_addr();
}

#include "emulator.hpp"

MK589::MK589() {
    cpe_arr.resize(cpe_amount);
    this->reset();
}

void MK589::reset() {
    MAR = 0b00000000;
    CO = 0b0;
    RO = 0b0;
    CI = 0b0;
    LI = 0b0;
    D = 0b00000000;
    A = 0b00000000;
    for (size_t i = 0; i < 0xC; ++i) {
        MEM[i] = 0;
    }
}
void MK589::do_fetch_decode_execute_cycle(const microcommand &mc) {
    mcu.fetch(mc.AC, mc.X, mc.FC);
    fetch_cpe(mc.F, mc.K, mc.I, mc.M);

    decode(); // both mcu and cpe

    if (is_performing_right_rot) {
        LI = FO;
        execute_cpe_right_rot();
    } else {
        CI = FO;
        execute_cpe();
    }
    // when FI flag is set (after cpe execution)
    mcu.fetch_flag(FI);
    mcu.execute();
    this->FO = mcu.FO;

    decode_adr();
}
void MK589::decode_adr() {
    row_adr = (mcu.MA >> 4).to_ulong();
    std::string ma = mcu.MA.to_string();
    col_adr = (mcu.MA & std::bitset<9> {0b000001111}).to_ulong();
}

size_t MK589::get_row_adr() {
    return row_adr;
}

size_t MK589::get_col_adr() {
    return col_adr;
}

void MK589::load(std::bitset<8> x) {
    mcu.load(x);
    decode_adr();
}

void MK589::decode() {
    decode_cpe();
    mcu.decode();
}

void MK589::fetch_cpe(std::bitset<7> f,
                               BYTE k,
                               BYTE i,
                               BYTE m)
{
    this->F = f;
    this->I = i;
    this->K = k;
    this->M = m;

}

void MK589::decode_cpe() {
    f_group = (F >> 4).to_ulong();
    r_group = (F & std::bitset<7> {0b0001111}).to_ulong();

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
    for (size_t i = 0; i < cpe_amount; ++i) {
        cpe_arr[i].r_group = r_group;
        cpe_arr[i].f_group = f_group;
        cpe_arr[i].ADR = ADR;
    }
    if (r_group == 3 and f_group == 0) {
        is_performing_right_rot = true;
    }
}

void MK589::execute_cpe_right_rot() {
    for (size_t i = cpe_amount - 1; i < cpe_amount; --i) {
        cpe_arr[i].fetch(F, ((I >> (i*2)) & 0b11), ((K >> (i*2)) & 0b11),
                ((M >> (i*2)) & 0b11), 0, LI);
        cpe_arr[i].execute();
        LI = cpe_arr[i].RO;
    }
    unite_registers();
    RO = cpe_arr[0].RO;
    FI = RO;
    A = MAR;
    D = MEM[AC];
}

void MK589::execute_cpe() {
    for (size_t i = 0; i < 4; ++i) {
        cpe_arr[i].fetch(F, ((I >> (i*2)) & 0b11), ((K >> (i*2)) & 0b11),
                ((M >> (i*2)) & 0b11), CI, 0);
        cpe_arr[i].execute();
        CI = cpe_arr[i].CO;
    }
    unite_registers();
    CO = cpe_arr[3].CO;
    FI = CO;
    A = MAR;
    D = MEM[AC];
}


void MK589::unite_registers() {
    for (size_t i = 0; i < 0xC; ++i) {
//        for (size_t k = 0; k < cpe_amount; ++k) {
//            MEM[i] += (cpe_arr[k].MEM[i] << k*2);
//        }
        MEM[i] = (cpe_arr[3].MEM[i] << 6) |
                 (cpe_arr[2].MEM[i] << 4) |
                 (cpe_arr[1].MEM[i] << 2) |
                 (cpe_arr[0].MEM[i] << 0);
    }
//    for (size_t k = 0; k < cpe_amount; ++k) {
//        MAR |= (cpe_arr[k].MAR << k*2);
//    }
    MAR = (cpe_arr[3].MAR << 6) |
          (cpe_arr[2].MAR << 4) |
          (cpe_arr[1].MAR << 2) |
          (cpe_arr[0].MAR << 0);
}

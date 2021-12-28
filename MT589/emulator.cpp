#include "emulator.hpp"

MK589::MK589() {
    cpe_arr.resize(cpe_amount);
    this->reset();
}
MK589& MK589::operator=(const MK589 &mk) {
    // Guard self assignment
    if (this == &mk)
        return *this;
    this->reset();
    cpe_arr.resize(mk.cpe_amount);
    MAR = mk.MAR;
    CO = mk.CO;
    RO = mk.RO;
    CI = mk.CI;
    LI = mk.LI;
    D = mk.D;
    A = mk.A;
    for (size_t i = 0; i < 0xC; ++i) {
        MEM[i] = mk.MEM[i];
    }
    for (size_t i = 0; i < cpe_amount; ++i) {
        cpe_arr[i].reset();
    }
    rom.memory = mk.rom.memory;
    return *this;
}

MK589::MK589(const MK589& mk) {
    cpe_arr.resize(mk.cpe_amount);
    MAR = mk.MAR;
    CO = mk.CO;
    RO = mk.RO;
    CI = mk.CI;
    LI = mk.LI;
    D = mk.D;
    A = mk.A;
    for (size_t i = 0; i < 0xC; ++i) {
        MEM[i] = mk.MEM[i];
    }
    for (size_t i = 0; i < cpe_amount; ++i) {
        cpe_arr[i].reset();
    }
    rom.memory = mk.rom.memory;
}

void MK589::reset() {
    MAR = 0x0000;
    A.reset();
    D.reset();
    RO.reset();
    CO.reset();
    CI = 0b0;
    LI = 0b0;
    M = 0x0000;
    I = 0x0000;
    for (size_t i = 0; i < 0xC; ++i) {
        MEM[i] = 0;
    }
    for (size_t i = 0; i < cpe_amount; ++i) {
        cpe_arr[i].reset();
    }
    mcu.reset();
}

void MK589::do_fetch_decode_execute_cycle(const microcommand &mc) {
    mcu.X = std::bitset<8> ((mc.M & 0xFF00) >> 8);
    mcu.fetch(mc.AC, mcu.X, mc.FC, mc.LD);
    fetch_cpe(mc.F, mc.K, mc.I, mc.M, mc.ED, mc.EA);

    decode(); // both mcu and cpe

    mcu.execute_output_flag_logic();
    this->FO = mcu.FO;

    A.reset();
    D.reset();
    if (is_performing_right_rot) {
        LI = FO;
        execute_cpe_right_rot();
    } else {
        CI = FO;
        execute_cpe();
    }

    if (ED == 0b1) {
       D = MEM[AC];
    }
    if (EA == 0b1) {
        A = MAR;
    }
    // when FI flag is set (after cpe execution)
    mcu.fetch_flag(FI);
    mcu.execute_input_flag_logic();
    mcu.execute_next_address_logic();
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
    mcu.fetch(0, x, 0, 0b1);
    mcu.execute_next_address_logic();
    decode_adr();
}

void MK589::decode() {
    decode_cpe();
    mcu.decode();
}

void MK589::fetch_cpe(std::bitset<7> f,
                               WORD k,
                               WORD i,
                               WORD m,
                               BYTE ed,
                               BYTE ea)
{
    this->F = f;
    this->I = i;
    this->K = k;
    this->M = m;
    this->ED = ed;
    this->EA = ea;
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
                ((M >> (i*2)) & 0b11), 0, LI, ED, EA);
        cpe_arr[i].execute();
        LI = cpe_arr[i].RO.value();
    }
    unite_registers();
    RO = cpe_arr[0].RO;
    FI = RO.value();
}

void MK589::execute_cpe() {
    for (size_t i = 0; i < cpe_amount; ++i) {
        cpe_arr[i].fetch(F, ((I >> (i*2)) & 0b11), ((K >> (i*2)) & 0b11),
                ((M >> (i*2)) & 0b11), CI, 0, ED, EA);
        cpe_arr[i].execute();
        CI = cpe_arr[i].CO.value();
    }
    unite_registers();
    CO = cpe_arr[cpe_amount - 1].CO;
    FI = CO.value();
}

void MK589::unite_registers() {
    for (size_t i = 0; i < 0xC; ++i) {
        MEM[i] = (cpe_arr[7].MEM[i] << 14) |
                (cpe_arr[6].MEM[i] << 12) |
                (cpe_arr[5].MEM[i] << 10) |
                (cpe_arr[4].MEM[i] << 8) |
                (cpe_arr[3].MEM[i] << 6) |
                (cpe_arr[2].MEM[i] << 4) |
                (cpe_arr[1].MEM[i] << 2) |
                (cpe_arr[0].MEM[i] << 0);
    }
    MAR = (cpe_arr[7].MAR << 14) |
            (cpe_arr[6].MAR << 12) |
            (cpe_arr[5].MAR << 10) |
            (cpe_arr[4].MAR << 8) |
            (cpe_arr[3].MAR << 6) |
            (cpe_arr[2].MAR << 4) |
            (cpe_arr[1].MAR << 2) |
            (cpe_arr[0].MAR << 0);
}

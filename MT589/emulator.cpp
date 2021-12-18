#include "emulator.hpp"

CPE::CPE() {
    this->reset();
}

void CPE::reset() {
    for (size_t i = 0x0; i < 0xE; ++i) {
        CPE::MEM[i] = 0b00;
    }
    MAR = 0b00;

    // set zero output?
    A = 0b00;
    D = 0b00;
    RO = 0b0;
    CO = 0b0;
    X = 0b0;
    Y = 0b0;
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

void CPE::fetch(const std::bitset<7>& f, BYTE i, BYTE k, BYTE m, BYTE CI, BYTE LI) {
    this->F = f;
    this->I = i;
    this->K = k;
    this->CI = CI;
    this->LI = LI;
    this->M = m;
}

void CPE::decode() {
    f_group = (F >> 4).to_ulong();// = 0b000;
    //f_group |= (F[6] & 0b1) << 2;
    //f_group |= (F[5] & 0b1) << 1;
    //f_group |= (F[4] & 0b1) << 0;

    r_group = (F & std::bitset<7> {0b0001111}).to_ulong(); //= 0b0000;
    //r_group |= (F[3] & 0b1) << 3;
    //r_group |= (F[2] & 0b1) << 2;
    //r_group |= (F[1] & 0b1) << 1;
    //r_group |= (F[0] & 0b1) << 0;

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
    CO = ~(~(CI & Y) | (X & Y));
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

MCU::MCU() {
    TF = 0b0;
    TC = 0b0;
    TZ = 0b0;
    PR_latch = 0b0000;
    MPAR = 0b000000000;
}

void MCU::load() {
    MPAR[8] = 0;
    for (size_t i = 0; i < 4; ++i) {
        MPAR[i] = X[i + 4];
    }
    for (size_t i = 4; i < 8; ++i) {
        MPAR[i] = X[i - 4];
    }
    MA = MPAR;
    decode_adr();
}
void MCU::connect_data(std::bitset<7> ac, std::bitset<8> x, BYTE fi, BYTE fc ) {
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
}
void MCU::do_flag_logic() {
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
            for (size_t i = 4; i < 9; ++i) {
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
void MCU::execute() {
    decode_jmp();
    decode_fl();
    do_flag_logic();
    compute_next_addr();
    decode_adr();
}

void MCU::decode_adr() {
    row_adr = (MA >> 4).to_ulong();
    col_adr = (MA & std::bitset<9> {0b000001111}).to_ulong();
}
size_t MCU::get_row_adr() {
    return row_adr;
}
size_t MCU::get_col_adr() {
    return col_adr;
}

RAM::RAM() {
   memory.resize(size);
   for (auto& cell : memory) {
       cell = 0b00000000;
   }
}
void RAM::write(size_t address, BYTE data) {
    memory[address] = data;
}
BYTE RAM::read(size_t address) {
    return memory[address];
}
ROM::ROM() {
   memory.resize(_rows);
   for(auto& row : memory) {
       row.resize(_cols);
   }
   for (auto& row : memory) {
       for (auto& cell : row) {
           cell.F = 0b1100000;
           cell.K = 0b00000000;
           cell.FC = 0b0011;
           cell.AC = 0b0000000;
       }
   }
}

bool ROM::is_nop(size_t row, size_t col) {
    return memory[row][col].F == 0b1100000;
}

void ROM::write(size_t row, size_t col, microcommand cmd) {
    memory[row][col] = cmd;
}

microcommand ROM::read(size_t row, size_t col) {
    return memory[row][col];
}
MK589::MK589() {
    cpe_arr.resize(cpe_amount);
    CO = 0b0;
    RO = 0b0;
}

void MK589::execute_cpe(std::bitset<7> F, BYTE K, BYTE I, BYTE M, BYTE LI) {

    for (size_t i = 0; i < 4; ++i) {
        cpe_arr[i].fetch(F, ((I >> (i*2)) & 0b11), ((K >> (i*2)) & 0b11),
                ((M >> (i*2)) & 0b11), CO, LI);
        cpe_arr[i].decode();
        cpe_arr[i].execute();
        CO = cpe_arr[i].CO;
    }
    unite_registers();

}

void MK589::unite_registers() {
    for (size_t i = 0; i < 0xD; ++i) {
        MEM[i] = (cpe_arr[3].MEM[i] << 6) |
                 (cpe_arr[2].MEM[i] << 4) |
                 (cpe_arr[1].MEM[i] << 2) |
                 (cpe_arr[0].MEM[i] << 0);
    }
    MAR = (cpe_arr[3].MAR << 6) |
          (cpe_arr[2].MAR << 4) |
          (cpe_arr[1].MAR << 2) |
          (cpe_arr[0].MAR << 0);
}

void MK589::execute_microprogram() {
    bool running = true;
    while (running) {
       auto mcmd = rom.read(mcu.get_row_adr(), mcu.get_col_adr());
       execute_cpe(mcmd.F, mcmd.K, 0, 0, 0);
       mcu.connect_data(mcmd.AC, 0b00000000, CO, mcmd.FC);
       mcu.execute();
    }
}

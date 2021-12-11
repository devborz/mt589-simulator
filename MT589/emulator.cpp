#include "emulator.hpp"

CPE::CPE() {
    for (size_t i = 0x0; i < 0xE; ++i) {
        CPE::MEM[i] = 0b00;
    }
    MAR = 0b00;
}

void CPE::fetch(const std::vector<BYTE>& f, BYTE i, BYTE k, BYTE m, BYTE CI, BYTE RI) {
    this->F = f;
    this->I = i;
    this->K = k;
    this->CI = CI;
    this->RI = RI;
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
            f_group3(); 
            break;
        case 4:
            f_group4(); 
            break;
        case 5:
            f_group5(); 
            break;
        case 6:
            f_group6(); 
            break;
        case 7:
            f_group7(); 
            break;
    }
}
void CPE::propogate() {

}
void CPE::compute_CO() {

}
void CPE::execute_f0() {
    switch(r_group) {
        case 1:
            // я хз как делать !!!!!!!!!!!!!!!!!! 
            // либо  отдельно  каждый случай - будет длинно
            // коротко - не могу придумать как сделать красиво и четко
            // тяжело просто впихнуть сюда генерацию X и Y и CO
            if (K == 0b00) { // ILR
                BUF = MEM[ADR] + CI;
                MEM[ADR] = BUF;
                MEM[AC] = BUF;
            } else if (K == 0b11){ // ALR
                BUF = MEM[ADR] + MEM[AC] + CI;
                MEM[ADR] = BUF;
                MEM[AC] = BUF;
            } else {

            }
            break;
        case 2:
            BUF = M + (AC ^ K) + CI;
            MEM[ADR] = BUF;
            break;
        case 3:
            break;

    }
}
void CPE::f_group1() {
    switch(r_group) {
        case 1:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 2:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 3:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
    }
}
void CPE::f_group2() {
    switch(r_group) {
        case 1:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 2:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 3:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
    }
}
void CPE::f_group3() {
    switch(r_group) {
        case 1:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 2:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 3:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
    }
}
void CPE::f_group4() {
    switch(r_group) {
        case 1:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 2:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 3:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
    }
}
void CPE::f_group5() {
    switch(r_group) {
        case 1:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 2:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 3:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
    }
}
void CPE::f_group6() {
    switch(r_group) {
        case 1:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 2:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 3:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
    }
}
void CPE::f_group7() {
    switch(r_group) {
        case 1:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 2:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
        case 3:
            if (K == 0b00) { // opcode

            } else if (K == 0b11){

            } else {

            }
            break;
    }
}

mk589::mk589()
{

}

#include "emulator.hpp"
#include <iostream>

void print_registers(const MK589& mk) {
    for (size_t i = 0; i < 0xD; ++i) {
        std::cout << i << " = " << (unsigned)mk.MEM[i] << ", " << std::bitset<8>(mk.MEM[i]) << std::endl;
    }

}
void print_out(const CPE& cpe) {

}

int main() {
   MK589 mk {};

   mk.CO = 0;
   std::cout << "ACM, AC <- M" << std::endl;
   mk.execute_cpe(0b0001011, 0b00000000, 0, 0b10110010, 0);

   print_registers(mk);

   mk.CO = 1;
   std::cout << "ADR, R8 <- AC + CI" << std::endl;
   mk.execute_cpe(0b0111000, 0b11111111, 0, 0, 0);
   print_registers(mk);
   return 0;
}

#include "ram.h"

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

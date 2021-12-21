#include "rom.h"

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

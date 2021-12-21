#ifndef RAM_H
#define RAM_H

#include "mt_global.h"

struct RAM {
    RAM();
    void write(size_t address, BYTE data);
    BYTE read(size_t address);

    size_t size = 0xFF;
    std::vector<BYTE> memory;
};

#endif // RAM_H

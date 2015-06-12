//
// Created by jglrxavpok on 08/06/2015.
//

#ifndef TEST_RAM_H
#define TEST_RAM_H

#include <stdint.h>

struct RAM {
    char *buffer;
    long size;
};

RAM *ram_init(char *buffer, long size);

uint8_t ram_read(RAM *ram, uint16_t index);

bool ram_write(RAM *ram, uint16_t index, char value);

void ram_dispose(RAM* ram);
#endif //TEST_RAM_H
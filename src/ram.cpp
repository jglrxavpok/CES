//
// Created by jglrxavpok on 08/06/2015.
//

#include <stdlib.h>
#include <iostream>
#include "ram.h"

RAM *ram_init(char *buffer, long size) {
    RAM *ram = (RAM *) malloc(sizeof(buffer) + sizeof(size));
    ram->buffer = buffer;
    ram->size = size;
    return ram;
}

uint8_t ram_read(RAM *ram, uint16_t index) {
    if (index >= ram->size || index < 0) {
        return NULL;
    }
    return (uint8_t) ram->buffer[index];
}

bool ram_write(RAM *ram, uint16_t index, char value) {
    if (index >= ram->size) {
        return false;
    }
    ram->buffer[index] = value;
    return true;
}

void ram_dispose(RAM* ram) {
    delete ram->buffer;
    delete ram;
}
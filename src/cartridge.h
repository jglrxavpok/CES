//
// Created by jglrxavpok on 08/06/2015.
//

#ifndef TEST_CARTRIDGE_H
#define TEST_CARTRIDGE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include "binutils.h"

#define CART_MIRRTYPE_HORIZONTAL false
#define CART_MIRRTYPE_VERTICAL true

struct CartData {
    char* data;
    long length;
    uint8_t prgnbr;
    uint8_t chrnbr;
    char rambanknbr;

    char* name;

    int mirrtype;
    bool battram;
    bool trailer;

    bool fourscreenmirr;

    uint8_t mapnum;
};

CartData* cart_init(char* rompath, char* data, int length);

void dump(CartData* data);
#endif //TEST_CARTRIDGE_H

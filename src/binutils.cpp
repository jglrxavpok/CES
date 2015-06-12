//
// Created by jglrxavpok on 08/06/2015.
//

#include <stdint.h>
#include "binutils.h"

using namespace std;

bool get_flag(uint8_t *data, uint8_t index) {
    return (*data & (1<<index)) != 0;
}

bool get_flag(int *data, uint8_t index) {
    return (*data & (1<<index)) != 0;
}

bool get_flag(unsigned int *data, uint8_t index) {
    return (*data & (1<<index)) != 0;
}

char hex_chars[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

void to_hex(uint8_t byte, string* result) {
    uint8_t high = (uint8_t) (byte >> 4 & 0x0F);
    uint8_t low = (uint8_t) (byte & 0x0F);
    *result = hex_chars[high];
    *result += hex_chars[low];
}
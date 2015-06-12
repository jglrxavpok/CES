//
// Created by jglrxavpok on 08/06/2015.
//

#ifndef TEST_BINUTILS_H
#define TEST_BINUTILS_H
#include <string>

bool get_flag(uint8_t *data, uint8_t index);
bool get_flag(int *data, uint8_t index);
bool get_flag(unsigned int *data, uint8_t index);
void to_hex(uint8_t byte, std::string* result);
#endif //TEST_BINUTILS_H

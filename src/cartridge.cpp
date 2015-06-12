//
// Created by jglrxavpok on 08/06/2015.
//

#include <c++/4.8.3/sstream>
#include "cartridge.h"
#include "string.h"

using namespace std;

CartData* cart_init(char* rompath, char* data, int length) {
    CartData* cart = (CartData *) malloc(sizeof(CartData));
    cart->name = rompath;
    if(data[0] == 'N' && data[1] == 'E' && data[2] == 'S') {
        if(data[3] == 0x1A) {
            cart->prgnbr = (uint8_t) data[4];
            cart->chrnbr = (uint8_t) data[5];

            int romcontrol1 = data[6];
            int romcontrol2 = data[7];

            cart->mirrtype = get_flag(&romcontrol1, 0);
            cart->battram = get_flag(&romcontrol1, 1);
            cart->trailer = get_flag(&romcontrol1, 2);
            cart->fourscreenmirr = get_flag(&romcontrol1, 3);

            cart->mapnum = (uint8_t) (romcontrol1 >> 4 | (romcontrol2 >> 4));

            cart->rambanknbr = data[8];

            cart->data = data;
            cart->length = length;
        }

        return cart;
    }
    delete cart;
    return NULL;
}

void dump(CartData* data) {
    char* indent = "    ";
    cout << "ROM {" << endl;
    cout << indent << "Name: "+string(data->name) << endl;
    cout << indent << "PRG-ROM count: "+to_string(data->prgnbr) << endl;
    cout << indent << "CHR-ROM count: "+to_string(data->chrnbr) << endl;
    cout << indent << "RAM banks count: "+to_string(data->rambanknbr) << endl;
    cout << indent << "Has trailer: "+to_string(data->trailer) << endl;
    cout << indent << "Has battery-backed RAM: "+to_string(data->battram) << endl;
    cout << "}" << endl;
}
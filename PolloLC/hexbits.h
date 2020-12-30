#ifndef HEXBITS_H
#define HEXBITS_H

#include <iostream>
#include <map>
#include <snap7.h>

#define BIT_1 0x01
#define BIT_2 0x02
#define BIT_3 0x04
#define BIT_4 0x08
#define BIT_5 0x10
#define BIT_6 0x20
#define BIT_7 0x40
#define BIT_8 0x80

const std::map<int, byte> bitmap {
    std::make_pair(0, BIT_1),
    std::make_pair(1, BIT_2),
    std::make_pair(2, BIT_3),
    std::make_pair(3, BIT_4),
    std::make_pair(4, BIT_5),
    std::make_pair(5, BIT_6),
    std::make_pair(6, BIT_7),
    std::make_pair(7, BIT_8)
};

#endif // HEXBITS_H

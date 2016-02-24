//
// Created by chnvideo on 2016/2/2.
//

#include <sstream>
#include <stdio.h>
#include "f4v_utility.hpp"

using namespace std;

uint32_t f4v_bytes_to_uint32(unsigned char** buf, int num)
{
    uint32_t temp = 0;
    
    for(int i = 0; i < num; i++) {
        temp = temp | buf[i] << (num - i - 1) * 8;
    }
    *buf = *buf + num;

    return temp;
}

string f4v_int2str(int32_t value)
{
    char *pp = (char*)(&value);
    stringstream ss;
    ss << pp[3] << pp[2] << pp[1] << pp[0];
    return ss.str();
}


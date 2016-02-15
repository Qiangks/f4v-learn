//
// Created by chnvideo on 2016/2/2.
//

#include <sstream>
#include <stdio.h>
#include "f4v_utility.hpp"

using namespace std;

int64_t  f4v_generate_type(char a, char b, char c, char d)
{
    return a << 24 | b << 16 | c << 8 | d;
}

string f4v_int2str(int32_t value)
{
    char *pp = (char*)(&value);
    stringstream ss;
    ss << pp[3] << pp[2] << pp[1] << pp[0];
    return ss.str();
}


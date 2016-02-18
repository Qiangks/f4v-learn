//
// Created by chnvideo on 2016/2/2.
//

#ifndef F4V_PARSEER_F4V_UTILITY_HPP
#define F4V_PARSEER_F4V_UTILITY_HPP

#include <string>
#include <stdint.h>

int64_t f4v_generate_type(char a, char b, char c, char d);

uint32_t f4v_char_join(unsigned char* buf, int num);

std::string f4v_int2str(int32_t value);

void parse_ftyp();

void parse_moov();

#endif //F4V_PARSEER_F4V_UTILITY_HPP

//
// Created by chnvideo on 2016/2/2.
//

#ifndef F4V_PARSEER_F4V_UTILITY_HPP
#define F4V_PARSEER_F4V_UTILITY_HPP

#include <string>
#include <stdint.h>

uint32_t f4v_bytes_to_uint32(unsigned char* buf, int num);

std::string f4v_int2str(int32_t value);

void parse_ftyp();

void parse_moov();

#endif //F4V_PARSEER_F4V_UTILITY_HPP

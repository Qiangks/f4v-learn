//
// Created by chnvideo on 2016/2/2.
//

#ifndef F4V_PARSEER_F4V_UTILITY_HPP
#define F4V_PARSEER_F4V_UTILITY_HPP

#include <stdint.h>

int get_box_header(FILE** fp, uint32_t& hs, uint32_t& type, uint64_t& size, uint64_t& sp, uint64_t& ep);

uint32_t f4v_bytes_to_uint32(unsigned char** buf, int num);

unsigned char* f4v_skip_bytes(unsigned char* buf, int num);

std::string f4v_int2str(int32_t value);

void parse_ftyp();

void parse_moov();

#endif //F4V_PARSEER_F4V_UTILITY_HPP

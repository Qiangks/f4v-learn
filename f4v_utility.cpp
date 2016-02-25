//
// Created by chnvideo on 2016/2/2.
//

#include <sstream>
#include "f4v_utility.hpp"
#include "f4v_type.hpp"
#include "f4v_log.hpp"

#define BOX_HEADER_SIZE       8
#define BOX_EXTENDED_SIZE   8

using namespace std;

int get_box_header(FILE** fp, uint32_t& header_size, uint32_t& type, uint64_t& size, uint64_t& sp, uint64_t& ep)
{
    int ret = ERROR_SUCCESS;

    // get the box start position
    sp = ftell(*fp);

    // read box header
    unsigned char head[BOX_HEADER_SIZE];
    if((fread(head, 1, BOX_HEADER_SIZE, *fp)) != BOX_HEADER_SIZE) {
        ret = ERROR_SYSTEM_READ_FAILED;
        f4v_error("read box header error, ret=%d", ret);
    }

    unsigned char* curr = head;
    uint32_t temp = 0;
    temp = f4v_bytes_to_uint32(&curr, 4);

    // get the box size
    if(temp == 1) {
        char extend[BOX_EXTENDED_SIZE];
        if((fread(extend, 1, BOX_EXTENDED_SIZE, *fp)) != BOX_EXTENDED_SIZE) {
            ret = ERROR_READ_BOX_HEADER_FAILED;
            f4v_error("read the box header extended size error, ret=%d", ret);
            return ret;
        }
        char* pp = (char*)&(size);
        pp[7] = extend[0];
        pp[6] = extend[1];
        pp[5] = extend[2];
        pp[4] = extend[3];
        pp[3] = extend[4];
        pp[2] = extend[5];
        pp[1] = extend[6];
        pp[0] = extend[7];

        header_size = BOX_HEADER_SIZE + BOX_EXTENDED_SIZE;
    } else {
        header_size = BOX_HEADER_SIZE;
        size = temp;
    }

    // get the box type
    type = f4v_bytes_to_uint32(&curr, 4);
    // get the box end position
    ep = sp + size;

    return ret;
}

uint32_t f4v_bytes_to_uint32(unsigned char** buf, int num)
{
    uint32_t temp = 0;
    
    for(int i = 0; i < num; i++) {
        temp = temp | (*(*buf + i) << ((num - i - 1) * 8));
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


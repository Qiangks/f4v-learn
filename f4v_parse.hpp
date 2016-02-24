//
// Created by chnvideo on 2016/2/2.
//

#ifndef F4V_PARSEER_HPP
#define F4V_PARSEER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>
#include "f4v_box.hpp"

class IF4vParser
{
public:
    IF4vParser();
    virtual ~IF4vParser();
private:
    virtual int read(uint64_t start, uint64_t end) = 0;
    virtual int parse() = 0;
public:
    virtual int show() = 0;
};

class F4vFileParser:: virtual public IF4vParser
{
private:
    FILE *fp;
    std::string file_name;
private:
    std::vector<F4vBox*> f4v_boxes;
public:
    F4vFileParser(std::string str);
    virtual ~F4vFileParser();
public:
    int initialize();
private:
    int start();
    int read(uint64_t start, uint64_t end);
    int add(F4vBox* fb);
    int parse();
private:
    int parse_ftyp();
    int parse_pdin();
    int parse_afra();
    int parse_abst();
    int parse_moov();
    int parse_uuid();
    int parse_moof();
    int parse_mdat();
    int parse_meta();
    int parse_frde();
    int parse_skip();
    int parse_mfra();
};

#endif //F4V_PARSEER_F4V_READ_HPP

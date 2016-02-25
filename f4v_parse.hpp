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

class F4vFileParser : public IF4vParser
{
private:
    FILE *fp;
    std::string file_name;
private:
    std::vector<F4vBox*> f4v_boxes;
    std::vector<std::vector<F4vSample> > f4v_vvs;
public:
    F4vFileParser(std::string str);
    virtual ~F4vFileParser();
public:
    int initialize();
private:
    int start();
    int add(F4vBox* fb);
private:    
    int read(uint64_t start, uint64_t end);
    int parse();
    int parse_sample();
public:
    virtual int show();
    virtual int show_sample();
private:
    int64_t get_filesize();
    F4vBox* get_box(std::string type);
    F4vBox* get_box(F4vBox* fb, int32_t type);
    F4vBox* get_moov();
    std::vector<F4vBox*> get_traks();
};

#endif //F4V_PARSEER_F4V_READ_HPP

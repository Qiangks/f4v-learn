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

class F4vFileParser : virtual public IF4vParser
{
private:
    FILE * fp;
    std::string name;
    bool is_eof;
    std::vector<F4vBoxAtom*> f4v_atomes;
    std::vector<std::vector<F4vSample> > f4v_vvs;
private:
    FtypBox* ftypx;
    PdinBox* pdinx;
    AfraBox* afrax;
    AbstBox* abstx;
    MoovBox* moovx;
    UuidBox* uuidx;
    MoofBox* moofx;
    MdatBox* mdatx;
    MetaBox* metax;
    FreeBox* freex;
    SkipBox* skipx;
    MfraBox* mfrax;
/*    
    StscBox* stscb;
    StszBox* stszb;
    SttsBox* sttsb;
    StcoBox* stcob;
    StssBox* stssb;
    CttsBox* cttsb;
*/
public:
    F4vFileParser(std::string str);
    virtual ~F4vFileParser();
public:
    int initialize();
    int start();
// IF4vParser
private:
    int read(uint64_t start, uint64_t end);
    int read2(uint64_t start, uint64_t end);
    int set_vecbox(F4vBoxAtom* fb);
    int read_ftypx();
    int read_pdinx();
    int read_afrax();
    int read_abstx();
    int read_moovx();
    int read_uuidx();
    int read_moofx();
    int read_mdatx();
    int read_metax();
    int read_freex();
    int read_skipx();
    int read_mfrax();
    int parse();
public:
    int show();
private:
    int64_t get_filesize();
private:
    int parse_sample();
private:
    void parse_ftyp(F4vBoxAtom** ppfb);
    void parse_moov(F4vBoxAtom** ppfb);
    void parse_mvhd(F4vBoxAtom** ppfb);
    void parse_trak(F4vBoxAtom** ppfb);
    void parse_tkhd(F4vBoxAtom** ppfb);
    void parse_mdia(F4vBoxAtom** ppfb);
    void parse_mdhd(F4vBoxAtom** ppfb);
    void parse_hdlr(F4vBoxAtom** ppfb);
    void parse_minf(F4vBoxAtom** ppfb);
    void parse_vmhd(F4vBoxAtom** ppfb);
    void parse_dinf(F4vBoxAtom** ppfb);
    void parse_dref(F4vBoxAtom** ppfb);
    void parse_url(F4vBoxAtom** ppfb);
    void parse_stbl(F4vBoxAtom** ppfb);
    void parse_stsd(F4vBoxAtom** ppfb);
    SttsBox* parse_stts(F4vBoxAtom** ppfb);
    CttsBox* parse_ctts(F4vBoxAtom** ppfb);
    StscBox* parse_stsc(F4vBoxAtom** ppfb);
    StszBox* parse_stsz(F4vBoxAtom** ppfb);
    StcoBox* parse_stco(F4vBoxAtom** ppfb);
    StssBox* parse_stss(F4vBoxAtom** ppfb);
    void parse_smhd(F4vBoxAtom** ppfb);
    void parse_free(F4vBoxAtom** ppfb);
    void parse_mdat(F4vBoxAtom** ppfb);
};

#endif //F4V_PARSEER_F4V_READ_HPP

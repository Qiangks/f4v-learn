//
// Created by chnvideo on 2016/2/2.
//

#include <algorithm>
#include <sstream>
#include <assert.h>
#include "f4v_parse.hpp"
#include "f4v_type.hpp"
#include "f4v_log.hpp"
#include "f4v_utility.hpp"

using namespace std;

IF4vParser::IF4vParser()
{
}

IF4vParser::~IF4vParser()
{
}

F4vFileParser::F4vFileParser(std::string str)
{
    file_name = str;
}

F4vFileParser::~F4vFileParser()
{
    for(vector<F4vBox*>::iterator it =f4v_boxes.begin(); it != f4v_boxes.end(); it++) {
        delete *it;
    }
}

int F4vFileParser::initialize()
{
    int ret = ERROR_SUCCESS;
    
    if ((fp = fopen(file_name.c_str(), "r")) == NULL) {
        ret = ERROR_SYSTEM_OPEN_FAILED;
        f4v_error("open the file %s failed, ret=%d", file_name.c_str(), ret);
        return ret;
    }

    if ((ret = start()) != ERROR_SUCCESS) {
        f4v_error("start parse file %s error, ret=%d", file_name.c_str(), ret);
        return ret;
    }

    if ((ret = parse_sample()) != ERROR_SUCCESS) {
        f4v_error("parse sample failed. ret=%d", ret);
        return ret;    
    }

    return ret;
}

int F4vFileParser:: start()
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(0, get_filesize())) != ERROR_SUCCESS) {
        f4v_error("read the file %s error, ret=%d", file_name.c_str(), ret);
        return ret;
    }
    
    return ret;
}

int F4vFileParser::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(f4v_boxes.begin(), f4v_boxes.end(), fb)) != f4v_boxes.end()) {
        return ret;
    }
    f4v_boxes.push_back(fb);
    
    return ret;
}

int F4vFileParser::read(uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(&fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case ftyp:
                fb = new FtypBox(sp, size, type, header_size, ep);
                break;
            case pdin:
                fb = new PdinBox(sp, size, type, header_size, ep);
                break;
            case afra:
                fb = new AfraBox(sp, size, type, header_size, ep);
                break;
            case abst:
                fb = new AbstBox(sp, size, type, header_size, ep);
                break;
            case moov:
                fb = new MoovBox(sp, size, type, header_size, ep);
                break;
            case uuid:
                fb = new UuidBox(sp, size, type, header_size, ep);
                break;
            case moof:
                fb = new MoofBox(sp, size, type, header_size, ep);
                break;
            case mdat:
                fb = new MdatBox(sp, size, type, header_size, ep);
                break;
            case meta:
                fb = new MetaBox(sp, size, type, header_size, ep);
                break;
            case frde:
                fb = new FreeBox(sp, size, type, header_size, ep);
                break;
            case skip:
                fb = new SkipBox(sp, size, type, header_size, ep);
                break;
            case mfra:
                fb  = new MfraBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(&fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(fp, ep, SEEK_SET);
    }

    return ret;
}

int F4vFileParser::parse()
{
    int ret = ERROR_SUCCESS;

    return ret;
}

int F4vFileParser::parse_sample()
{
    int ret = ERROR_SUCCESS;

    std::vector<F4vBox*> traks = get_traks();
    std::vector<F4vBox*>::iterator it;
    for (it = traks.begin(); it != traks.end(); it++) {
        F4vBox* tkb = *it;
        F4vBox* mdb = get_box(tkb, mdia);
        F4vBox* hdb = get_box(mdb, hdlr);
        F4vBox* mfb = get_box(mdb, minf);
        F4vBox* stblb = get_box(mfb, stbl);
        F4vBox* stsdb = get_box(stblb, stsd);
        F4vBox* sttsb = get_box(stblb, stts);
        F4vBox* cttsb = get_box(stblb, ctts);
        F4vBox* stscb = get_box(stblb, stsc);
        F4vBox* stszb = get_box(stblb, stsz);
        F4vBox* stcob = get_box(stblb, stco);
        F4vBox* stssb = get_box(stblb, stss);
        F4vBox* sdtpb = get_box(stblb, sdtp);
        
    }

    return ret;
}

int F4vFileParser::show()
{
    int ret = ERROR_SUCCESS;


    vector<F4vBox*>::iterator it;
    for (it = f4v_boxes.begin(); it != f4v_boxes.end(); it++) {
        F4vBox* fb = *it;
        fb->display();
    }

    return ret;
}

int F4vFileParser::show_sample()
{
    int ret = ERROR_SUCCESS;

    return ret;
}

int64_t F4vFileParser::get_filesize()
{
    int64_t cur = ftell(fp);
    ::fseek(fp, 0, SEEK_END);
    int64_t size = ftell(fp);
    ::fseek(fp, cur, SEEK_SET);
    return size;
}

F4vBox* F4vFileParser::get_box(std::string type)
{
    std::vector<F4vBox*>::iterator it;
    for (it = f4v_boxes.begin(); it != f4v_boxes.end(); it++) 
    {
        F4vBox* fb = *it;
        string box_type = f4v_int2str(fb->type);
        if (box_type == type) {
            return fb;
        }
    }

    return NULL;
}

F4vBox* F4vFileParser::get_box(F4vBox* fb, int32_t type)
{
    std::vector<F4vBox*>::iterator it;
    for (it = fb->container.begin(); it != fb->container.end(); it++) {
        F4vBox* f = *it;
        if (f->type == type) {
            return f;
        }
    }

    return NULL;
}

F4vBox* F4vFileParser::get_moov()
{
    return get_box("moov");
}

std::vector<F4vBox*> F4vFileParser::get_traks()
{
    std::vector<F4vBox*> traks;

    F4vBox* mvb = get_moov();
    assert(mvb != NULL);

    std::vector<F4vBox*>::iterator it;
    for (it = mvb->container.begin(); it != mvb->container.end(); it++) {
        F4vBox* tkb = get_box(mvb, trak);
        if (tkb != NULL) {
            traks.push_back(tkb);
        }
    }

    return traks;
}

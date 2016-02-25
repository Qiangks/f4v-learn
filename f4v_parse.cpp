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
    
    delete fp;
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
        HdlrBox* hdb = dynamic_cast<HdlrBox*>(get_box(mdb, hdlr));
        F4vBox* mfb = get_box(mdb, minf);
        F4vBox* stblb = get_box(mfb, stbl);
        StsdBox* stsdb = dynamic_cast<StsdBox*>(get_box(stblb, stsd));
        SttsBox* sttsb = dynamic_cast<SttsBox*>(get_box(stblb, stts));
        CttsBox* cttsb = dynamic_cast<CttsBox*>(get_box(stblb, ctts));
        StscBox* stscb = dynamic_cast<StscBox*>(get_box(stblb, stsc));
        StszBox* stszb = dynamic_cast<StszBox*>(get_box(stblb, stsz));
        StcoBox* stcob = dynamic_cast<StcoBox*>(get_box(stblb, stco));
        StssBox* stssb = dynamic_cast<StssBox*>(get_box(stblb, stss));
        SdtpBox* sdtpb = dynamic_cast<SdtpBox*>(get_box(stblb, sdtp));
            
        std::vector<F4vSample> f4v_samples;

        uint32_t total_chunk = stcob->offset_count;
        F4vChunk f4v_chunk[total_chunk];

        // restore the chunk list
        uint32_t last_chk_no = total_chunk + 1;
        for (int i = stscb->count - 1; i >= 0; --i) {
            uint32_t beg_real_chkno = stscb->stsc_records[i].first_chunk;
            
            for (uint32_t chk_i = beg_real_chkno - 1; chk_i < last_chk_no - 1; chk_i++) {
                f4v_chunk[chk_i].sample_count = stscb->stsc_records[i].spc;      
                f4v_chunk[chk_i].sdi = stscb->stsc_records[i].sdi;
            }
            last_chk_no = beg_real_chkno; 
        }

        // restore the sample list
        uint32_t sam_index = 0;
        for(uint32_t i = 0; i < total_chunk; i++) {
            f4v_chunk[i].first_sample_index = sam_index;

            uint32_t index_in_chunk = 0;
            for(uint32_t sam_i = 0; sam_i < f4v_chunk[i].sample_count; sam_i++) {
                F4vSample fs;
                fs.chunk_index = i;
                fs.index_in_chunk = index_in_chunk;
                f4v_samples.push_back(fs);

                sam_index++;
                index_in_chunk++;
            }
        }

        // get each sample's size, type
        for(uint32_t i = 0; i < stszb->size_count; i++) {
            f4v_samples[i].size = stszb->size_table[i];
            f4v_samples[i].type = hdb->handler_type;
        }

        // get each sample's offset
        for(int i = 0; i < stszb->size_count; i++) {
            uint32_t chk_no = f4v_samples[i].chunk_index;
            uint32_t index_in_chunk = f4v_samples[i].index_in_chunk;
            f4v_samples[i].offset = stcob->offsets[chk_no];
            for (int j = 0; j < index_in_chunk; j++) {
                f4v_samples[i].offset += f4v_samples[i-j].size;
            }
        }

        // get each sample's duration
        int count = 0;
        for(int i = 0; i < sttsb->count; i++) {
            count = count + sttsb->stts_records[i].sample_count;

            for(int j = 0; j < stszb->size_count; j++) {
                if(j < count) {
                    f4v_samples[j].duration = sttsb->stts_records[i].sample_delta;
                }
            }
        }

        f4v_vvs.push_back(f4v_samples);
        
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

    if ((ret = show_sample()) != ERROR_SUCCESS) {
        f4v_error("show sample failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int F4vFileParser::show_sample()
{
    int ret = ERROR_SUCCESS;
    
    vector< vector<F4vSample> >::iterator vv_it;

    for (vv_it = f4v_vvs.begin(); vv_it != f4v_vvs.end(); vv_it++) {
        std::vector<F4vSample> vfs = *vv_it;
        vector<F4vSample>::iterator v_it;
        for (v_it = vfs.begin(); v_it != vfs.end(); v_it++) {
            F4vSample fs = *v_it;
            fs.display();
        }
    }

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
        F4vBox* tkb = *it;
        if (tkb->type == trak) {
            traks.push_back(tkb);
        }
    }

    return traks;
}

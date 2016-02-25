//
// Created by chnvideo on 2016/2/3.
//

#include <sstream>
#include <assert.h>
#include <algorithm>
#include "f4v_box.hpp"
#include "f4v_log.hpp"
#include "f4v_utility.hpp"
#include "f4v_type.hpp"

using namespace std;

F4vSample::F4vSample()
{
}

F4vSample::~F4vSample()
{
}

int F4vSample::display()
{
    int ret = ERROR_SUCCESS;

    f4v_trace("sample: type=%s, size=%d, duration=%d, offset=%d, chunk index=%d, index in chunk=: %d",
        f4v_int2str(type).c_str(), size, duration, offset, chunk_index, index_in_chunk);

    return ret;
}

F4vBox::F4vBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
{
    sp = start_position;
    this->size = size;
    this->type = type;
    hs = header_size;
    ep = end_position;
}

F4vBox::~F4vBox()
{
    std::vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        delete (*it);
    }
}

 int F4vBox::initialize(FILE** fp)
 {
 }
 
 void F4vBox::display()
 {
 }

FtypBox::FtypBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

FtypBox::~FtypBox()
{
}

int FtypBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    major_brand = f4v_bytes_to_uint32(&curr, 4);
    minor_version = f4v_bytes_to_uint32(&curr, 4);

    stringstream ss;
    for(int i = 8; i < sz; i++) {
        ss << *curr;
        curr++;
    }
    compatible_brands = ss.str();

    return ret;
}

void FtypBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Major Brand: %s, Minor Version: %ld, compatible brands: %s",
        f4v_int2str(type).c_str(), size, f4v_int2str(major_brand).c_str(), minor_version,
                    compatible_brands.c_str());

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

PdinBox::PdinBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

PdinBox::~PdinBox()
{
}

int PdinBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void PdinBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

AfraBox::AfraBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AfraBox::~AfraBox()
{
}

int AfraBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void AfraBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

AbstBox::AbstBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AbstBox::~AbstBox()
{
}

int AbstBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void AbstBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

AsrtBox::AsrtBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AsrtBox::~AsrtBox()
{
}

int AsrtBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void AsrtBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

AfrtBox::AfrtBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AfrtBox::~AfrtBox()
{
}

int AfrtBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void AfrtBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MoovBox::MoovBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MoovBox::~MoovBox()
{
}

int MoovBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read moov box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int MoovBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case mvhd:
                fb = new MvhdBox(sp, size, type, header_size, ep);
                break;
            case trak:
                fb = new TrakBox(sp, size, type, header_size, ep);
                break;
            case mvex:
                fb = new MvexBox(sp, size, type, header_size, ep);
                break;
            case auth:
                fb = new AuthBox(sp, size, type, header_size, ep);
                break;
            case titl:
                fb = new TitlBox(sp, size, type, header_size, ep);
                break;
            case dscp:
                fb = new DscpBox(sp, size, type, header_size, ep);
                break;
            case cprt:
                fb = new CprtBox(sp, size, type, header_size, ep);
                break;
            case udta:
                fb = new UdtaBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }

    return ret;
}

int MoovBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void MoovBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

F4vBox* MoovBox::get_box(string type)
{
    std::vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) 
    {
        F4vBox* fb = *it;
        string box_type = f4v_int2str(fb->type);
        if (box_type == type) {
            return fb;
        }
    }

    return NULL;
}

MvhdBox::MvhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MvhdBox::~MvhdBox()
{
}

int MvhdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char buf[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    if (version == 0) {
        creation_time = f4v_bytes_to_uint32(&curr, 4);
        modification_time = f4v_bytes_to_uint32(&curr, 4);
        timescale = f4v_bytes_to_uint32(&curr, 4);
        duration = f4v_bytes_to_uint32(&curr, 4);
        rate = f4v_bytes_to_uint32(&curr, 2) + (float)f4v_bytes_to_uint32(&curr, 2)/10;
    } else {
        creation_time = f4v_bytes_to_uint32(&curr, 8);
        modification_time = f4v_bytes_to_uint32(&curr, 8);
        timescale = f4v_bytes_to_uint32(&curr, 4);
        duration = f4v_bytes_to_uint32(&curr, 8);
        rate = f4v_bytes_to_uint32(&curr, 2) + (float)f4v_bytes_to_uint32(&curr, 2)/10;
    }

    return ret;
}

void MvhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, CreationTime: %ld, ModificationTime: %ld, TimeScale: %d, Duration: %ld, "
        "Rate: %0.1f", f4v_int2str(type).c_str(), size, creation_time, modification_time, timescale, duration, rate);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

TrakBox::TrakBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

TrakBox::~TrakBox()
{
}

int TrakBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read trak box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int TrakBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case tkhd:
                fb = new TkhdBox(sp, size, type, header_size, ep);
                break;
            case edts:
                fb = new EdtsBox(sp, size, type, header_size, ep);
                break;
            case mdia:
                fb = new MdiaBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }  

    return ret;
}

int TrakBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void TrakBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

TkhdBox::TkhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

TkhdBox::~TkhdBox()
{
}

int TkhdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    if (version == 0) {
        creation_time = f4v_bytes_to_uint32(&curr, 4);
        modification_time = f4v_bytes_to_uint32(&curr, 4);
        trak_id= f4v_bytes_to_uint32(&curr, 4);
        // skip the reserved
        curr = curr + 4;
        duration = f4v_bytes_to_uint32(&curr, 4);
    } else {
        creation_time = f4v_bytes_to_uint32(&curr, 8);
        modification_time = f4v_bytes_to_uint32(&curr, 8);
        trak_id = f4v_bytes_to_uint32(&curr, 4);
        // skip the reserved
        curr = curr + 4;
        duration = f4v_bytes_to_uint32(&curr, 8);
    }

    return ret;
}

void TkhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, CreationTime: %ld, ModificationTime: %ld, Duration: %ld, "
        "TrakID: %d", f4v_int2str(type).c_str(), size, creation_time, modification_time,
        duration, trak_id);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

EdtsBox::EdtsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

EdtsBox::~EdtsBox()
{
}

int EdtsBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read edts box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int EdtsBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case elst:
                fb = new ElstBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }

    return ret;
}

int EdtsBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void EdtsBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

ElstBox::ElstBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

ElstBox::~ElstBox()
{
}

int ElstBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void ElstBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MdiaBox::MdiaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MdiaBox::~MdiaBox()
{
}

int MdiaBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read mdia box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int MdiaBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case mdhd:
                fb = new MdhdBox(sp, size, type, header_size, ep);
                break;
            case hdlr:
                fb = new HdlrBox(sp, size, type, header_size, ep);
                break;
            case minf:
                fb = new MinfBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }

    return ret;
}


int MdiaBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void MdiaBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MdhdBox::MdhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MdhdBox::~MdhdBox()
{
}

int MdhdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    if (version == 0) {
        creation_time = f4v_bytes_to_uint32(&curr, 4);
        modification_time = f4v_bytes_to_uint32(&curr, 4);
        timescale = f4v_bytes_to_uint32(&curr, 4);
        duration = f4v_bytes_to_uint32(&curr, 4);
    } else {
        creation_time = f4v_bytes_to_uint32(&curr, 8);
        modification_time = f4v_bytes_to_uint32(&curr, 8);
        timescale = f4v_bytes_to_uint32(&curr, 4);
        duration = f4v_bytes_to_uint32(&curr, 8);
    }

    return ret;
}

void MdhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, CreationTime: %ld, ModificationTime: %ld, Duration: %ld, "
        "timescale: %d", f4v_int2str(type).c_str(), size, creation_time, modification_time,
        duration, timescale);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

HdlrBox::HdlrBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

HdlrBox::~HdlrBox()
{
}

int HdlrBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    version  = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    predefined = f4v_bytes_to_uint32(&curr, 4);
    handler_type = f4v_bytes_to_uint32(&curr, 4);

    return ret;
}

void HdlrBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, HandlerType: %s", f4v_int2str(type).c_str(), size, version,
        f4v_int2str(handler_type).c_str());

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MinfBox::MinfBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MinfBox::~MinfBox()
{
}

int MinfBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read minf box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int MinfBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }
    
    while (ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case vmhd:
                fb = new VmhdBox(sp, size, type, header_size, ep);
                break;
            case smhd:
                fb = new SmhdBox(sp, size, type, header_size, ep);
                break;
            case hmhd:
                fb = new HmhdBox(sp, size, type, header_size, ep);
                break;
            case nmhd:
                fb = new NmhdBox(sp, size, type, header_size, ep);
                break;
            case dinf:
                fb = new DinfBox(sp, size, type, header_size, ep);
                break;
            case stbl:
                fb = new StblBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }

    return ret;
}

int MinfBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void MinfBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

VmhdBox::VmhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

VmhdBox::~VmhdBox()
{
}

int VmhdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    graphic_mode = f4v_bytes_to_uint32(&curr, 2);
    op_color[0] = f4v_bytes_to_uint32(&curr, 2);
    op_color[1] = f4v_bytes_to_uint32(&curr, 2);
    op_color[2] = f4v_bytes_to_uint32(&curr, 2);

    return ret;
}

void VmhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, GraphicMode: %d, OpColor(%d, %d, %d)",
        f4v_int2str(type).c_str(), size, version, flags, graphic_mode, op_color[0], op_color[1], op_color[3]);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

SmhdBox::SmhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

SmhdBox::~SmhdBox()
{
}

int SmhdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    balance = (float)f4v_bytes_to_uint32(&curr, 1) + (float)f4v_bytes_to_uint32(&curr, 1)/10;

    return ret;
}

void SmhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Balance: %0.1f",
        f4v_int2str(type).c_str(), size, version, flags, balance);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

HmhdBox::HmhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

HmhdBox::~HmhdBox()
{
}

int HmhdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void HmhdBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

NmhdBox::NmhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

NmhdBox::~NmhdBox()
{
}

int NmhdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void NmhdBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

DinfBox::DinfBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

DinfBox::~DinfBox()
{
}

int DinfBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read dinf box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int DinfBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case dref:
                fb = new DrefBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }

    return ret;
}

int DinfBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void DinfBox::display()
{
     f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
        
    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

DrefBox::DrefBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

DrefBox::~DrefBox()
{
}

int DrefBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    entry_count = f4v_bytes_to_uint32(&curr, 4);

    if ((ret = read(fp, sp + hs + 8, ep)) != ERROR_SUCCESS) {
        f4v_error("rad dref box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int DrefBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case url:
                fb = new UrlBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }

    return ret;
}

int DrefBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void DrefBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, EntryCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, entry_count);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

UrlBox::UrlBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

UrlBox::~UrlBox()
{
}

int UrlBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);

    return ret;
}

void UrlBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d",
        f4v_int2str(type).c_str(), size, version, flags);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

StblBox::StblBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

StblBox::~StblBox()
{
}

int StblBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("rad dref box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int StblBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case stsd:
                fb = new StsdBox(sp, size, type, header_size, ep);
                break;
            case stts:
                fb = new SttsBox(sp, size, type, header_size, ep);
                break;
            case ctts:
                fb = new CttsBox(sp, size, type, header_size, ep);
                break;
            case stsc:
                fb = new StscBox(sp, size, type, header_size, ep);
                break;
            case stsz:
                fb = new StszBox(sp, size, type, header_size, ep);
                break;
            case stco:
                fb = new StcoBox(sp, size, type, header_size, ep);
                break;
            case co64:
                fb = new Co64Box(sp, size, type, header_size, ep);
                break;
            case stss:
                fb = new StssBox(sp, size, type, header_size, ep);
                break; 
            case sdtp:
                fb = new SdtpBox(sp, size, type, header_size, ep);
                break; 
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }

    return ret;
}

int StblBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void StblBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

StsdBox::StsdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

StsdBox::~StsdBox()
{
}

int StsdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    count = f4v_bytes_to_uint32(&curr, 4);

    return ret;
}

void StsdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

SttsBox::SttsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

SttsBox::~SttsBox()
{
}

int SttsBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    count = f4v_bytes_to_uint32(&curr, 4);
    for (uint32_t i = 0; i < count; i++) {
        SttsRecord sr;
        sr.sample_count =  f4v_bytes_to_uint32(&curr, 4);
        sr.sample_delta = f4v_bytes_to_uint32(&curr, 4);
        stts_records.push_back(sr);
    }

    return ret;
}

void SttsBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

CttsBox::CttsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

CttsBox::~CttsBox()
{
}

int CttsBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    count = f4v_bytes_to_uint32(&curr, 4);
    for(uint32_t i = 0; i < count; i++) {
        CttsRecord cr;
        cr.sample_count = f4v_bytes_to_uint32(&curr, 4);
        cr.sample_offset = f4v_bytes_to_uint32(&curr, 4);
        ctts_records.push_back(cr);
    }

    return ret;
}

void CttsBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

StscBox::StscBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

StscBox::~StscBox()
{
}

int StscBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    count = f4v_bytes_to_uint32(&curr, 4);
    for (int i = 0; i< count; i++) {
        StscRecord sr;
        sr.first_chunk = f4v_bytes_to_uint32(&curr, 4);
        sr.spc = f4v_bytes_to_uint32(&curr, 4);
        sr.sdi = f4v_bytes_to_uint32(&curr, 4);
        stsc_records.push_back(sr);
    }

    return ret;
}

void StscBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

StszBox::StszBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

StszBox::~StszBox()
{
}

int StszBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    constant_size = f4v_bytes_to_uint32(&curr, 4);
    size_count = f4v_bytes_to_uint32(&curr, 4);
    for (uint32_t i = 0; i < size_count; i++) {
        uint32_t st = f4v_bytes_to_uint32(&curr, 4);
       size_table.push_back(st);
    }

    return ret;
}

void StszBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, ConstantSize: %d, SizeCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, constant_size, size_count);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

StcoBox::StcoBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

StcoBox::~StcoBox()
{
}

int StcoBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    offset_count = f4v_bytes_to_uint32(&curr, 4);
    for (uint32_t i = 0; i < offset_count; i++) {
        uint32_t offset = f4v_bytes_to_uint32(&curr, 4);
        offsets.push_back(offset);
    }

    return ret;
}

void StcoBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, OffsetCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, offset_count);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

Co64Box::Co64Box(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

Co64Box::~Co64Box()
{
}

int Co64Box::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void Co64Box::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

StssBox::StssBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

StssBox::~StssBox()
{
}

int StssBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;
    version = f4v_bytes_to_uint32(&curr, 1);
    flags = f4v_bytes_to_uint32(&curr, 3);
    sync_count = f4v_bytes_to_uint32(&curr, 4);
    for (uint32_t i = 0; i < sync_count; i++) {
        uint32_t st = f4v_bytes_to_uint32(&curr, 4);
        sync_table.push_back(st);
    }

    return ret;
}

void StssBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, SyncCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, sync_count);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

SdtpBox::SdtpBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

SdtpBox::~SdtpBox()
{
}

int SdtpBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void SdtpBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MvexBox::MvexBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MvexBox::~MvexBox()
{
}

int MvexBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read mvex box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int MvexBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case mehd:
                fb = new MehdBox(sp, size, type, header_size, ep);
                break;
            case trex:
                fb = new TrexBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }

    return ret;
}

int MvexBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}


void MvexBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MehdBox::MehdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MehdBox::~MehdBox()
{
}

int MehdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void MehdBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

TrexBox::TrexBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

TrexBox::~TrexBox()
{
}

int TrexBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void TrexBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

AuthBox::AuthBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AuthBox::~AuthBox()
{
}

int AuthBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void AuthBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

TitlBox::TitlBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

TitlBox::~TitlBox()
{
}

int TitlBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void TitlBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

DscpBox::DscpBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

DscpBox::~DscpBox()
{
}

int DscpBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void DscpBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

CprtBox::CprtBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

CprtBox::~CprtBox()
{
}

int CprtBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void CprtBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

UdtaBox::UdtaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

UdtaBox::~UdtaBox()
{
}

int UdtaBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void UdtaBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

UuidBox::UuidBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

UuidBox::~UuidBox()
{
}

int UuidBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void UuidBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MoofBox::MoofBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MoofBox::~MoofBox()
{
}

int MoofBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

int MoofBox::read(FILE** fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(*fp, start, SEEK_SET);
    if (end <= ftell(*fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(*fp) < end) {
        uint32_t header_size = -1;
        uint32_t type = -1;
        uint64_t size = -1;
        uint64_t sp = 0;
        uint64_t ep = 0;
        
        if ( (ret = get_box_header(fp, header_size, type, size, sp, ep)) !=ERROR_SUCCESS) {
            f4v_error("get the box header data failed. ret=%d", ret);
            return ret;
        }

        F4vBox* fb = NULL;
        switch(type){
            case mfhd:
                fb = new MfhdBox(sp, size, type, header_size, ep);
                break;
            case traf:
                fb = new TrafBox(sp, size, type, header_size, ep);
                break;
            default:
                break;
        }
        assert(fb != NULL);

        if ((ret = fb->initialize(fp)) != ERROR_SUCCESS) {
            return ret;
        }

        if ((ret = add(fb)) != ERROR_SUCCESS) {
            return ret;
        }

        ::fseek(*fp, ep, SEEK_SET);
    }
}

int MoofBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void MoofBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MfhdBox::MfhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MfhdBox::~MfhdBox()
{
}

int MfhdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void MfhdBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

TrafBox::TrafBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

TrafBox::~TrafBox()
{
}

int TrafBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void TrafBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

TfhdBox::TfhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

TfhdBox::~TfhdBox()
{
}

int TfhdBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void TfhdBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

TrunBox::TrunBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

TrunBox::~TrunBox()
{
}

int TrunBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void TrunBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MdatBox::MdatBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MdatBox::~MdatBox()
{
}

int MdatBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void MdatBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MetaBox::MetaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MetaBox::~MetaBox()
{
}

int MetaBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void MetaBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

IlstBox::IlstBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

IlstBox::~IlstBox()
{
}

int IlstBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void IlstBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

FreeBox::FreeBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

FreeBox::~FreeBox()
{
}

int FreeBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void FreeBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

SkipBox::SkipBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

SkipBox::~SkipBox()
{
}

int SkipBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void SkipBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MfraBox::MfraBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MfraBox::~MfraBox()
{
}

int MfraBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void MfraBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

TfraBox::TfraBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

TfraBox::~TfraBox()
{
}

int TfraBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void TfraBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}

MfroBox::MfroBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

MfroBox::~MfroBox()
{
}

int MfroBox::initialize(FILE** fp)
{
    int ret = ERROR_SUCCESS;
    
    int sz = size - hs;
    ::fseek(*fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[sz];
    ::fread(buf, 1, sz, *fp);

    unsigned char* curr = buf;

    return ret;
}

void MfroBox::display()
{

    if (container.size() == 0) {
        return;
    }
    
    vector<F4vBox*>::iterator it;
    for (it = container.begin(); it != container.end(); it++) {
        (*it)->display();
    }
}


/*
RtmpBox::RtmpBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

RtmpBox::~RtmpBox()
{
}

void RtmpBox::display()
{
}

AmhpBox::AmhpBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AmhpBox::~AmhpBox()
{
}

void AmhpBox::display()
{
}

AmtoBox::AmtoBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AmtoBox::~AmtoBox()
{
}

void AmtoBox::display()
{
}

EncvBox::EncvBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

EncvBox::~EncvBox()
{
}

void EncvBox::display()
{
}

EncaBox::EncaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

EncaBox::~EncaBox()
{
}

void EncaBox::display()
{
}

EncrBox::EncrBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

EncrBox::~EncrBox()
{
}

void EncrBox::display()
{
}

SinfBox::SinfBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

SinfBox::~SinfBox()
{
}

void SinfBox::display()
{
}

FrmaBox::FrmaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

FrmaBox::~FrmaBox()
{
}

void FrmaBox::display()
{
}

SchmBox::SchmBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

SchmBox::~SchmBox()
{
}

void SchmBox::display()
{
}

SchiBox::SchiBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

SchiBox::~SchiBox()
{
}

void SchiBox::display()
{
}

AdkmBox::AdkmBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AdkmBox::~AdkmBox()
{
}

void AdkmBox::display()
{
}

AhdrBox::AhdrBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AhdrBox::~AhdrBox()
{
}

void AhdrBox::display()
{
}

AprmBox::AprmBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AprmBox::~AprmBox()
{
}

void AprmBox::display()
{
}

AeibBox::AeibBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AeibBox::~AeibBox()
{
}

void AeibBox::display()
{
}

AkeyBox::AkeyBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AkeyBox::~AkeyBox()
{
}

void AkeyBox::display()
{
}

ApsBox::ApsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

ApsBox::~ApsBox()
{
}

void ApsBox::display()
{
}

FlxsBox::FlxsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

FlxsBox::~FlxsBox()
{
}

void FlxsBox::display()
{
}

AsigBox::AsigBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AsigBox::~AsigBox()
{
}

void AsigBox::display()
{
}

AdafBox::AdafBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position)
: F4vBox(start_position, size, type, header_size, end_position)
{
}

AdafBox::~AdafBox()
{
}

void AdafBox::display()
{
}
*/

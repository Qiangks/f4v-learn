//
// Created by chnvideo on 2016/2/3.
//

#include "f4v_box.hpp"
#include "f4v_log.hpp"
#include "f4v_utility.hpp"
#include "f4v_type.hpp"

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
        delete *it;
    }
}


F4vSample::F4vSample()
{
}

F4vSample::~F4vSample()
{
}

int F4vSample::display()
{
    int ret = ERROR_SUCCESS;

    f4v_trace("sample size: %d, sample duration: %d, sample offset: %d, sample chunk index: %d "
                    "sample index in chunk: %d", size, duration, offset, chunk_index, index_in_chunk);

    return ret;
}

FtypBox::FtypBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

FtypBox::~FtypBox()
{
}

int FtypBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;
    
    int size = size - hs;
    ::fseek(fp, sp + hs, SEEK_SET);
    unsigned char* buf = new unsigned char[size];
    ::fread(buf, 1, size, fp);

    unsigned char* curr = buf;
    major_brand = f4v_bytes_to_uint32(curr, 4);
    minor_version = f4v_bytes_to_uint32(curr, 4);

    stringstream ss;
    for(int i = 8; i < size; i++) {
        ss << buf[i];
    }
    compatible_brands = ss.str();

    return ret;
}

void FtypBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Major Brand: %s, Minor Version: %ld, compatible brands: %s",
        f4v_int2str(type).c_str(), size, f4v_int2str(major_brand).c_str(), minor_version,
                    compatible_brands.c_str());
}

PdinBox::PdinBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

PdinBox::~PdinBox()
{
}

int PdinBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void PdinBox::display()
{
}

AfraBox::AfraBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AfraBox::~AfraBox()
{
}

int AfraBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void AfraBox::display()
{
}

AbstBox::AbstBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AbstBox::~AbstBox()
{
}

int AbstBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void AbstBox::display()
{
}

MoovBox::MoovBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MoovBox::~MoovBox()
{
}

int MoovBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read moov box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int MoovBox::read(FILE* fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;

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

        ::fseek(fp, ep, SEEK_SET);
}

int MoovBox::add(F4vBox* fb)
{
}

void MoovBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

int MoovBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

UuidBox::UuidBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

UuidBox::~UuidBox()
{
}

void UuidBox::display()
{
}

MoofBox::MoofBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MoofBox::~MoofBox()
{
}


int MoofBox::read(FILE * fp, uint64_t start, uint64_t end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;
        
        switch(type){
            case mfhd:
                mfhdx = new MfhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case traf:
                trafx = new TrafBox(sp, size, type, header_size, ep, header_size, true);
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

        ::fseek(fp, ep, SEEK_SET);
    }
}

int MoofBox::add()
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void MoofBox::display()
{
}

MdatBox::MdatBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MdatBox::~MdatBox()
{
}

int MdatBox::initialize(FILE* fp)
{
}

void MdatBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

MetaBox::MetaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MetaBox::~MetaBox()
{
}

int MetaBox::initialize(FILE* fp);
{
}

void MetaBox::display()
{
}

FreeBox::FreeBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

FreeBox::~FreeBox()
{
}

int FreeBox::initialize(FILE* fp);
{
}

void FreeBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

SkipBox::SkipBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

SkipBox::~SkipBox()
{
}

int SkipBox::initialize(FILE* fp);
{
}

void SkipBox::display()
{
}

MfraBox::MfraBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MfraBox::~MfraBox()
{
}

int SkipBox::initialize(FILE* fp);
{
}

void MfraBox::display()
{
}

MvhdBox::MvhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MvhdBox::~MvhdBox()
{
}

int MvhdBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;
    
    int size = size - hs;
    ::fseek(fp, sp + hs, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    version = f4v_bytes_to_uint32(buf, 1);
    flags = f4v_bytes_to_uint32(&buf[1], 3);
    if (version == 0) {
        creation_time = f4v_bytes_to_uint32(&buf[4], 4);
        modification_time = f4v_bytes_to_uint32(&buf[8], 4);
        timescale = f4v_bytes_to_uint32(&buf[12], 4);
        duration = f4v_bytes_to_uint32(&buf[16], 4);
        rate = f4v_bytes_to_uint32(&buf[20], 2) + (float)f4v_bytes_to_uint32(&buf[22], 2)/10;
    } else {
        creation_time = f4v_bytes_to_uint32(&buf[4], 8);
        modification_time = f4v_bytes_to_uint32(&buf[12], 8);
        timescale = f4v_bytes_to_uint32(&buf[20], 4);
        duration = f4v_bytes_to_uint32(&buf[24], 8);
        rate = f4v_bytes_to_uint32(&buf[32], 2) + (float)f4v_bytes_to_uint32(&buf[34], 2)/10;
    }

    return ret;
}

void MvhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, CreationTime: %ld, ModificationTime: %ld, TimeScale: %d, Duration: %ld, "
        "Rate: %0.1f", f4v_int2str(type).c_str(), size, creation_time, modification_time, timescale, duration, rate);
}

TrakBox::TrakBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

TrakBox::~TrakBox()
{
}

int TrakBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read trak box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int TrakBox::read(FILE* fp, int start, int end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;

        F4vBox* fb = NULL;
        switch(type){
            case mvhd:
                fb = new TkhdBox(sp, size, type, header_size, ep);
                break;
            case trak:
                fb = new EdtsBox(sp, size, type, header_size, ep);
                break;
            case mvex:
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

        ::fseek(fp, ep, SEEK_SET);
}

int TrakBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void TrakBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

MvexBox::MvexBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MvexBox::~MvexBox()
{
}

int MvexBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read mvex box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int MvexBox::read(FILE* fp, int start, int end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;

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

        ::fseek(fp, ep, SEEK_SET);
}

int MvexBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}


void MvexBox::display()
{
}

AuthBox::AuthBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AuthBox::~AuthBox()
{
}

int AuthBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void AuthBox::display()
{
}

TitlBox::TitlBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

TitlBox::~TitlBox()
{
}

int TitlBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void TitlBox::display()
{
}

DscpBox::DscpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

DscpBox::~DscpBox()
{
}

int DscpBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void DscpBox::display()
{
}

CprtBox::CprtBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

CprtBox::~CprtBox()
{
}

int CprtBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void CprtBox::display()
{
}

UdtaBox::UdtaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

UdtaBox::~UdtaBox()
{
}

int UdtaBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void UdtaBox::display()
{
}

TkhdBox::TkhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

TkhdBox::~TkhdBox()
{
}


int TkhdBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void TkhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, CreationTime: %ld, ModificationTime: %ld, Duration: %ld, "
        "TrakID: %d", f4v_int2str(type).c_str(), size, creation_time, modification_time,
        duration, trak_id);
}

EdtsBox::EdtsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

EdtsBox::~EdtsBox()
{
}

int EdtsBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read edts box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int EdtsBox::read(FILE* fp, int start, int end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;

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

        ::fseek(fp, ep, SEEK_SET);

}

int EdtsBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void EdtsBox::display()
{
}

ElstBox::ElstBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

ElstBox::~ElstBox()
{
}

void ElstBox::display()
{
}

MdiaBox::MdiaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MdiaBox::~MdiaBox()
{
}

int MdiaBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("read mdia box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int MdiaBox::read(FILE* fp, int start, int end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;

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

        ::fseek(fp, ep, SEEK_SET);
}


int MdiaBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void MdiaBox::display()
{
}

MdhdBox::MdhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MdhdBox::~MdhdBox()
{
}

int MdhdBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void MdhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, CreationTime: %ld, ModificationTime: %ld, Duration: %ld, "
        "timescale: %d", f4v_int2str(type).c_str(), size, creation_time, modification_time,
        duration, timescale);
}

HdlrBox::HdlrBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

HdlrBox::~HdlrBox()
{
}

int HdlrBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void HdlrBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, HandlerType: %s", f4v_int2str(type).c_str(), size, version,
        f4v_int2str(handler_type).c_str());
}

MinfBox::MinfBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MinfBox::~MinfBox()
{
}

int MinfBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

int MinfBox::read(FILE* fp, int start, int end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;

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

        ::fseek(fp, ep, SEEK_SET);
}

int MinfBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void MinfBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

VmhdBox::VmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

VmhdBox::~VmhdBox()
{
}

int VmhdBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void VmhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, GraphicMode: %d, OpColor(%d, %d, %d)",
        f4v_int2str(type).c_str(), size, version, flags, graphic_mode, op_color[0], op_color[1], op_color[3]);
}

SmhdBox::SmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

SmhdBox::~SmhdBox()
{
}

int SmhdBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void SmhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Balance: %0.1f",
        f4v_int2str(type).c_str(), size, version, flags, balance);
}

HmhdBox::HmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

HmhdBox::~HmhdBox()
{
}

int HmhdBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void HmhdBox::display()
{
}

NmhdBox::NmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

NmhdBox::~NmhdBox()
{
}

int NmhdBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void NmhdBox::display()
{
}

DinfBox::DinfBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

DinfBox::~DinfBox()
{
}

int DinfBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("rad dinf box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int DinfBox::read(FILE* fp, int start, int end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;

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

        ::fseek(fp, ep, SEEK_SET);
}

int DinfBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void DinfBox::display()
{
}

DrefBox::DrefBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

DrefBox::~DrefBox()
{
}

int DrefBox::initialize(FILE * fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("rad dref box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int DrefBox::read(FILE* fp, int start, int end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;

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

        ::fseek(fp, ep, SEEK_SET);
}

int DrefBox::add(F4vBox* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void DrefBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, EntryCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, entry_count);
}

UrlBox::UrlBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

UrlBox::~UrlBox()
{
}

int UrlBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void UrlBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d",
        f4v_int2str(type).c_str(), size, version, flags);
}

StblBox::StblBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

StblBox::~StblBox()
{
}

int StblBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(fp, sp + hs, ep)) != ERROR_SUCCESS) {
        f4v_error("rad dref box failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int StblBox::read(FILE* fp, int start, int end)
{
    int ret = ERROR_SUCCESS;
    
    ::fseek(fp, start, SEEK_SET);
    if (end <= ftell(fp)) {
        ret = ERROR_END_POSITION;
        f4v_error("the box position start=%ld, end=%ld error, ret=%d", start, end, ret);
        return ret;
    }

    while(ftell(fp) < end) {
        // get the box start position
        uint64_t sp = ftell(fp);

        // read box header
        unsigned char head[BOX_HEADER_SIZE];
        if((fread(head, 1, BOX_HEADER_SIZE, fp)) != BOX_HEADER_SIZE) {
            ret = ERROR_SYSTEM_READ_FAILED;
            f4v_error("read box header error, ret=%d", ret);
        }
        
        uint32_t header_size = -1;
        uint64_t size = -1;

        uint32_t temp = 0;
        char* pp = (char*)&temp;
        pp[3] = head[0];
        pp[2] = head[1];
        pp[1] = head[2];
        pp[0] = head[3];

        // get the box size
        if(temp == 1) {
            char extend[BOX_EXTENDED_SIZE];
            if((fread(extend, 1, BOX_EXTENDED_SIZE, fp)) != BOX_EXTENDED_SIZE) {
                ret = ERROR_READ_BOX_HEADER_FAILED;
                f4v_error("read the box header extended size error, ret=%d", ret);
                return ret;
            }
            pp = (char*)&(size);
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
        int32_t type = f4v_bytes_to_uint32(&head[4], 4);
        // get the box end position
        uint64_t ep = sp + size;

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

        ::fseek(fp, ep, SEEK_SET);
}

int StblBox::add()
{
    int ret = ERROR_SUCCESS;

    vector<F4vBox*>::iterator it;
    if((it = ::find(container.begin(), container.end(), fb)) != container.end()) {
        return ret;
    }
    container.push_back(fb);
    
    return ret;
}

void StblBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

StsdBox::StsdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

StsdBox::~StsdBox()
{
}

int StsdBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void StsdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);
}

SttsBox::SttsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

SttsBox::~SttsBox()
{
}

int SttsBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void SttsBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);
}

CttsBox::CttsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

CttsBox::~CttsBox()
{
}

int CttsBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void CttsBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);
}

StscBox::StscBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

StscBox::~StscBox()
{
}

int StscBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void StscBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);
}

StszBox::StszBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

StszBox::~StszBox()
{
}

int StszBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void StszBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, ConstantSize: %d, SizeCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, constant_size, size_count);
}

StcoBox::StcoBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

StcoBox::~StcoBox()
{
}

int StcoBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void StcoBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, OffsetCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, offset_count);
}

Co64Box::Co64Box(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

Co64Box::~Co64Box()
{
}

int Co64Box::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void Co64Box::display()
{
}

StssBox::StssBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

StssBox::~StssBox()
{
}

int StssBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void StssBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, SyncCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, sync_count);
}

SdtpBox::SdtpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

SdtpBox::~SdtpBox()
{
}

int SdtpBox::initialize(FILE* fp)
{
    int ret = ERROR_SUCCESS;

    return ret;
}

void SdtpBox::display()
{
}



// TODO: =====================================================================

AsrtBox::AsrtBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AsrtBox::~AsrtBox()
{
}

void AsrtBox::display()
{
}

AfrtBox::AfrtBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AfrtBox::~AfrtBox()
{
}

void AfrtBox::display()
{
}


MehdBox::MehdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MehdBox::~MehdBox()
{
}

void MehdBox::display()
{
}

TrexBox::TrexBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

TrexBox::~TrexBox()
{
}

void TrexBox::display()
{
}

MfhdBox::MfhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MfhdBox::~MfhdBox()
{
}

void MfhdBox::display()
{
}

TrafBox::TrafBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

TrafBox::~TrafBox()
{
}

void TrafBox::display()
{
}

TfhdBox::TfhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

TfhdBox::~TfhdBox()
{
}

void TfhdBox::display()
{
}

TrunBox::TrunBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

TrunBox::~TrunBox()
{
}

void TrunBox::display()
{
}

IlstBox::IlstBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

IlstBox::~IlstBox()
{
}

void IlstBox::display()
{
}

TfraBox::TfraBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

TfraBox::~TfraBox()
{
}

void TfraBox::display()
{
}

MfroBox::MfroBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

MfroBox::~MfroBox()
{
}

void MfroBox::display()
{
}

RtmpBox::RtmpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

RtmpBox::~RtmpBox()
{
}

void RtmpBox::display()
{
}

AmhpBox::AmhpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AmhpBox::~AmhpBox()
{
}

void AmhpBox::display()
{
}

AmtoBox::AmtoBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AmtoBox::~AmtoBox()
{
}

void AmtoBox::display()
{
}

EncvBox::EncvBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

EncvBox::~EncvBox()
{
}

void EncvBox::display()
{
}

EncaBox::EncaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

EncaBox::~EncaBox()
{
}

void EncaBox::display()
{
}

EncrBox::EncrBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

EncrBox::~EncrBox()
{
}

void EncrBox::display()
{
}

SinfBox::SinfBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

SinfBox::~SinfBox()
{
}

void SinfBox::display()
{
}

FrmaBox::FrmaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

FrmaBox::~FrmaBox()
{
}

void FrmaBox::display()
{
}

SchmBox::SchmBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

SchmBox::~SchmBox()
{
}

void SchmBox::display()
{
}

SchiBox::SchiBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

SchiBox::~SchiBox()
{
}

void SchiBox::display()
{
}

AdkmBox::AdkmBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AdkmBox::~AdkmBox()
{
}

void AdkmBox::display()
{
}

AhdrBox::AhdrBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AhdrBox::~AhdrBox()
{
}

void AhdrBox::display()
{
}

AprmBox::AprmBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AprmBox::~AprmBox()
{
}

void AprmBox::display()
{
}

AeibBox::AeibBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AeibBox::~AeibBox()
{
}

void AeibBox::display()
{
}

AkeyBox::AkeyBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AkeyBox::~AkeyBox()
{
}

void AkeyBox::display()
{
}

ApsBox::ApsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

ApsBox::~ApsBox()
{
}

void ApsBox::display()
{
}

FlxsBox::FlxsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

FlxsBox::~FlxsBox()
{
}

void FlxsBox::display()
{
}

AsigBox::AsigBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AsigBox::~AsigBox()
{
}

void AsigBox::display()
{
}

AdafBox::AdafBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBox(st, sz, ty, hs, ed, off, ic)
{
}

AdafBox::~AdafBox()
{
}

void AdafBox::display()
{
}

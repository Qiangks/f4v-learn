//
// Created by chnvideo on 2016/2/3.
//

#include "f4v_box.hpp"
#include "f4v_log.hpp"
#include "f4v_utility.hpp"
#include "f4v_type.hpp"


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

F4vBoxAtom::F4vBoxAtom(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
{
    start = st;
    size = sz;
    type = ty;
    header_size = hs;
    end = ed;
    is_container = ic;
    offset = off;
}

F4vBoxAtom::~F4vBoxAtom()
{

}

FtypBox::FtypBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

FtypBox::~FtypBox()
{
}

void FtypBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Major Brand: %s, Minor Version: %ld, compatible brands: %s",
        f4v_int2str(type).c_str(), size, f4v_int2str(major_brand).c_str(), minor_version,
                    compatible_brands.c_str());
}

PdinBox::PdinBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

PdinBox::~PdinBox()
{
}

void PdinBox::display()
{
}

AfraBox::AfraBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AfraBox::~AfraBox()
{
}

void AfraBox::display()
{
}

AbstBox::AbstBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AbstBox::~AbstBox()
{
}

void AbstBox::display()
{
}

AsrtBox::AsrtBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AsrtBox::~AsrtBox()
{
}

void AsrtBox::display()
{
}

AfrtBox::AfrtBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AfrtBox::~AfrtBox()
{
}

void AfrtBox::display()
{
}

MoovBox::MoovBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MoovBox::~MoovBox()
{
}

int MoovBox::read(FILE * fp, uint64_t start, uint64_t end)
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
            case mvhd:
                mvhdx = new MvhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case trak:
                TrakBox* trakx = new TrakBox(sp, size, type, header_size, ep, header_size, true);
                trakxv.push_back(trakx);
                break;
            case mvex:
                mvexx = new MvexBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case auth:
                authx = new AuthBox(sp, size, type, header_size, ep, 0, false);
                break;
            case titl:
                titlx = new TitlBox(sp, size, type, header_size, ep, 0, false);
                break;
            case dscp:
                dscpx = new DscpBox(sp, size, type, header_size, ep, 0, false);
                break;
            case cprt:
                cprtx = new CprtBox(sp, size, type, header_size, ep, 0, false);
                break;
            case udta:
                udtax = new UdtaBox(sp, size, type, header_size, ep, 0, false);
                break;
            default:
                break;
        }

        ::fseek(fp, ep, SEEK_SET);
}

void MoovBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

MvhdBox::MvhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MvhdBox::~MvhdBox()
{
}

void MvhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, CreationTime: %ld, ModificationTime: %ld, TimeScale: %d, Duration: %ld, "
        "Rate: %0.1f", f4v_int2str(type).c_str(), size, creation_time, modification_time, timescale, duration, rate);
}

TrakBox::TrakBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

TrakBox::~TrakBox()
{
}

void TrakBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

TkhdBox::TkhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

TkhdBox::~TkhdBox()
{
}

void TkhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, CreationTime: %ld, ModificationTime: %ld, Duration: %ld, "
        "TrakID: %d", f4v_int2str(type).c_str(), size, creation_time, modification_time,
        duration, trak_id);
}

EdtsBox::EdtsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

EdtsBox::~EdtsBox()
{
}

void EdtsBox::display()
{
}

ElstBox::ElstBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

ElstBox::~ElstBox()
{
}

void ElstBox::display()
{
}

MdiaBox::MdiaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MdiaBox::~MdiaBox()
{
}

void MdiaBox::display()
{
}

MdhdBox::MdhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MdhdBox::~MdhdBox()
{
}

void MdhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, CreationTime: %ld, ModificationTime: %ld, Duration: %ld, "
        "timescale: %d", f4v_int2str(type).c_str(), size, creation_time, modification_time,
        duration, timescale);
}

HdlrBox::HdlrBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

HdlrBox::~HdlrBox()
{
}

void HdlrBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, HandlerType: %s", f4v_int2str(type).c_str(), size, version,
        f4v_int2str(handler_type).c_str());
}

MinfBox::MinfBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MinfBox::~MinfBox()
{
}

void MinfBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

VmhdBox::VmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

VmhdBox::~VmhdBox()
{
}

void VmhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, GraphicMode: %d, OpColor(%d, %d, %d)",
        f4v_int2str(type).c_str(), size, version, flags, graphic_mode, op_color[0], op_color[1], op_color[3]);
}

SmhdBox::SmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

SmhdBox::~SmhdBox()
{
}

void SmhdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Balance: %0.1f",
        f4v_int2str(type).c_str(), size, version, flags, balance);
}

HmhdBox::HmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

HmhdBox::~HmhdBox()
{
}

void HmhdBox::display()
{
}

NmhdBox::NmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

NmhdBox::~NmhdBox()
{
}

void NmhdBox::display()
{
}

DinfBox::DinfBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

DinfBox::~DinfBox()
{
}

void DinfBox::display()
{
}

DrefBox::DrefBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

DrefBox::~DrefBox()
{
}

void DrefBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, EntryCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, entry_count);
}

UrlBox::UrlBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

UrlBox::~UrlBox()
{
}

void UrlBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d",
        f4v_int2str(type).c_str(), size, version, flags);
}

StblBox::StblBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

StblBox::~StblBox()
{
}

void StblBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

StsdBox::StsdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

StsdBox::~StsdBox()
{
}

void StsdBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);
}

SttsBox::SttsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

SttsBox::~SttsBox()
{
}

void SttsBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);
}

CttsBox::CttsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

CttsBox::~CttsBox()
{
}

void CttsBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);
}

StscBox::StscBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

StscBox::~StscBox()
{
}

void StscBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, Count: %d",
        f4v_int2str(type).c_str(), size, version, flags, count);
}

StszBox::StszBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

StszBox::~StszBox()
{
}

void StszBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, ConstantSize: %d, SizeCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, constant_size, size_count);
}

StcoBox::StcoBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

StcoBox::~StcoBox()
{
}

void StcoBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, OffsetCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, offset_count);
}

Co64Box::Co64Box(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

Co64Box::~Co64Box()
{
}

void Co64Box::display()
{
}

StssBox::StssBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

StssBox::~StssBox()
{
}

void StssBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld, Version: %d, Flags: %d, SyncCount: %d",
        f4v_int2str(type).c_str(), size, version, flags, sync_count);
}

SdtpBox::SdtpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

SdtpBox::~SdtpBox()
{
}

void SdtpBox::display()
{
}

MvexBox::MvexBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MvexBox::~MvexBox()
{
}

void MvexBox::display()
{
}

MehdBox::MehdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MehdBox::~MehdBox()
{
}

void MehdBox::display()
{
}

TrexBox::TrexBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

TrexBox::~TrexBox()
{
}

void TrexBox::display()
{
}

AuthBox::AuthBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AuthBox::~AuthBox()
{
}

void AuthBox::display()
{
}

TitlBox::TitlBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

TitlBox::~TitlBox()
{
}

void TitlBox::display()
{
}

DscpBox::DscpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

DscpBox::~DscpBox()
{
}

void DscpBox::display()
{
}

CprtBox::CprtBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

CprtBox::~CprtBox()
{
}

void CprtBox::display()
{
}

UdtaBox::UdtaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

UdtaBox::~UdtaBox()
{
}

void UdtaBox::display()
{
}

UuidBox::UuidBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

UuidBox::~UuidBox()
{
}

void UuidBox::display()
{
}

MoofBox::MoofBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
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

        ::fseek(fp, ep, SEEK_SET);
}

void MoofBox::display()
{
}

MfhdBox::MfhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MfhdBox::~MfhdBox()
{
}

void MfhdBox::display()
{
}

TrafBox::TrafBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

TrafBox::~TrafBox()
{
}

void TrafBox::display()
{
}

TfhdBox::TfhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

TfhdBox::~TfhdBox()
{
}

void TfhdBox::display()
{
}

TrunBox::TrunBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

TrunBox::~TrunBox()
{
}

void TrunBox::display()
{
}

MdatBox::MdatBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MdatBox::~MdatBox()
{
}

void MdatBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

MetaBox::MetaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MetaBox::~MetaBox()
{
}

void MetaBox::display()
{
}

IlstBox::IlstBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

IlstBox::~IlstBox()
{
}

void IlstBox::display()
{
}

FreeBox::FreeBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

FreeBox::~FreeBox()
{
}

void FreeBox::display()
{
    f4v_trace("Box Type: %s, Box Size: %ld", f4v_int2str(type).c_str(), size);
}

SkipBox::SkipBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

SkipBox::~SkipBox()
{
}

void SkipBox::display()
{
}

MfraBox::MfraBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MfraBox::~MfraBox()
{
}

void MfraBox::display()
{
}

TfraBox::TfraBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

TfraBox::~TfraBox()
{
}

void TfraBox::display()
{
}

MfroBox::MfroBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

MfroBox::~MfroBox()
{
}

void MfroBox::display()
{
}

RtmpBox::RtmpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

RtmpBox::~RtmpBox()
{
}

void RtmpBox::display()
{
}

AmhpBox::AmhpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AmhpBox::~AmhpBox()
{
}

void AmhpBox::display()
{
}

AmtoBox::AmtoBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AmtoBox::~AmtoBox()
{
}

void AmtoBox::display()
{
}

EncvBox::EncvBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

EncvBox::~EncvBox()
{
}

void EncvBox::display()
{
}

EncaBox::EncaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

EncaBox::~EncaBox()
{
}

void EncaBox::display()
{
}

EncrBox::EncrBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

EncrBox::~EncrBox()
{
}

void EncrBox::display()
{
}

SinfBox::SinfBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

SinfBox::~SinfBox()
{
}

void SinfBox::display()
{
}

FrmaBox::FrmaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

FrmaBox::~FrmaBox()
{
}

void FrmaBox::display()
{
}

SchmBox::SchmBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

SchmBox::~SchmBox()
{
}

void SchmBox::display()
{
}

SchiBox::SchiBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

SchiBox::~SchiBox()
{
}

void SchiBox::display()
{
}

AdkmBox::AdkmBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AdkmBox::~AdkmBox()
{
}

void AdkmBox::display()
{
}

AhdrBox::AhdrBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AhdrBox::~AhdrBox()
{
}

void AhdrBox::display()
{
}

AprmBox::AprmBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AprmBox::~AprmBox()
{
}

void AprmBox::display()
{
}

AeibBox::AeibBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AeibBox::~AeibBox()
{
}

void AeibBox::display()
{
}

AkeyBox::AkeyBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AkeyBox::~AkeyBox()
{
}

void AkeyBox::display()
{
}

ApsBox::ApsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

ApsBox::~ApsBox()
{
}

void ApsBox::display()
{
}

FlxsBox::FlxsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

FlxsBox::~FlxsBox()
{
}

void FlxsBox::display()
{
}

AsigBox::AsigBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AsigBox::~AsigBox()
{
}

void AsigBox::display()
{
}

AdafBox::AdafBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic)
: F4vBoxAtom(st, sz, ty, hs, ed, off, ic)
{
}

AdafBox::~AdafBox()
{
}

void AdafBox::display()
{
}

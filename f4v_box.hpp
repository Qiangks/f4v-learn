//
// Created by chnvideo on 2016/2/3.
//

#ifndef F4V_PARSEER_F4V_BOX_HPP
#define F4V_PARSEER_F4V_BOX_HPP

#include <stdint.h>
#include <string>
#include <vector>

class F4vSample
{
public:
    uint32_t offset;
    uint32_t size;
    uint32_t duration;
    uint32_t chunk_index;
    uint32_t index_in_chunk;
public:
    F4vSample();
    virtual ~F4vSample();
public:
    int display();
    
};

typedef struct _f4v_chunk
{
    uint32_t first_sample_index;
    uint32_t sample_count;
    // sample id
    uint32_t sdi;
}F4vChunk;

class F4vBoxAtom
{
public:
    uint64_t start;
    uint64_t size;
    int32_t type;
    uint32_t header_size;
    uint64_t end;
    uint32_t offset;
public:
    // whether the box is a container box
    bool is_container;
public:
    F4vBoxAtom(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~F4vBoxAtom();
public:
    virtual void display() = 0;
};

class FtypBox : public F4vBoxAtom
{
public:
    int32_t major_brand;
    int32_t minor_version;
    std::string compatible_brands;
public:
    // @param st: the box start position
    // @param sz: the box size
    // @param ty: the box type
    // @param hs: the box header size
    // @param ed: the box end position
    // @param off: the offset of the box data
    // @param ic : if box is container box
    FtypBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~FtypBox();
public:
    virtual void display();
};

//    std::vecotr<F4vBoxAtom*> container;

class PdinBox : public F4vBoxAtom
{
public:

public:
    PdinBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~PdinBox();
public:
    virtual void display();
};

class AfraBox : public F4vBoxAtom
{
public:
    AfraBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AfraBox();
public:
    virtual void display();
};

class AbstBox : public F4vBoxAtom
{
public:
   
public:
    AbstBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AbstBox();
public:
    virtual void display();
};

class AsrtBox : public F4vBoxAtom
{
public:

public:
    AsrtBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AsrtBox();
public:
    virtual void display();
};

class AfrtBox : public F4vBoxAtom
{
public:

public:
    AfrtBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AfrtBox();
public:
    virtual void display();
};

class MoovBox : public F4vBoxAtom
{
public:

public:
    MoovBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MoovBox();
public:
    virtual void display();
};

class MvhdBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint64_t creation_time;
    uint64_t modification_time;
    uint32_t timescale;
    uint64_t duration;
    float rate;
public:
    MvhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MvhdBox();
public:
    virtual void display();
};

class TrakBox : public F4vBoxAtom
{
public:
public:
    TrakBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~TrakBox();
public:
    virtual void display();
};

class TkhdBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint64_t creation_time;
    uint64_t modification_time;
    uint32_t trak_id;
    uint64_t duration;

public:
    TkhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~TkhdBox();
public:
    virtual void display();
};

class EdtsBox : public F4vBoxAtom
{
public:

public:
    EdtsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~EdtsBox();
public:
    virtual void display();
};

class ElstBox : public F4vBoxAtom
{
public:

public:
    ElstBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~ElstBox();
public:
    virtual void display();
};

class MdiaBox : public F4vBoxAtom
{
public:

public:
    MdiaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MdiaBox();
public:
    virtual void display();
};

class MdhdBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint64_t creation_time;
    uint64_t modification_time;
    uint32_t timescale;
    uint64_t duration;
public:
    MdhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MdhdBox();
public:
    virtual void display();
};

class HdlrBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t handler_type;
public:
    HdlrBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~HdlrBox();
public:
    virtual void display();
};

class MinfBox : public F4vBoxAtom
{
public:
   
public:
    MinfBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MinfBox();
public:
    virtual void display();
};

class VmhdBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t graphic_mode;
    uint32_t op_color[3];
public:
    VmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~VmhdBox();
public:
    virtual void display();
};

class SmhdBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    float balance;
public:
    SmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~SmhdBox();
public:
    virtual void display();
};

class HmhdBox : public F4vBoxAtom
{
public:

public:
    HmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~HmhdBox();
public:
    virtual void display();
};

class NmhdBox : public F4vBoxAtom
{
public:

public:
    NmhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~NmhdBox();
public:
    virtual void display();
};

class DinfBox : public F4vBoxAtom
{
public:

public:
    DinfBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~DinfBox();
public:
    virtual void display();
};

class DrefBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t entry_count;
public:
    DrefBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~DrefBox();
public:
    virtual void display();
};

class UrlBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
public:
    UrlBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~UrlBox();
public:
    virtual void display();
};

class StblBox : public F4vBoxAtom
{
public:

public:
    StblBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~StblBox();
public:
    virtual void display();
};

class StsdBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t count;
    std::vector<F4vBoxAtom*>descriptions;
public:
    StsdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~StsdBox();
public:
    virtual void display();
};

typedef struct _stts_record{
    uint32_t sample_count;
    uint32_t sample_delta;
}SttsRecord;

class SttsBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t count;
    std::vector<SttsRecord> stts_records;
public:
    SttsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~SttsBox();
public:
    virtual void display();
};

typedef struct _ctts_record{
    uint32_t sample_count;
    uint32_t sample_offset;
}CttsRecord;

class CttsBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t count;
    std::vector<CttsRecord>ctts_records;
public:
    CttsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~CttsBox();
public:
    virtual void display();
};

typedef struct _stsc_record{
    uint32_t first_chunk;
    // samples per chunk
    uint32_t spc;
    // sample description index
    uint32_t sdi;
} StscRecord;

class StscBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t count;
    std::vector<StscRecord> stsc_records;
public:
    StscBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~StscBox();
public:
    virtual void display();
};

class StszBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t constant_size;
    uint32_t size_count;
    std::vector<uint32_t>size_table;
public:
    StszBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~StszBox();
public:
    virtual void display();
};

class StcoBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t offset_count;
    std::vector<uint32_t>offsets;
public:
    StcoBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~StcoBox();
public:
    virtual void display();
};

class Co64Box : public F4vBoxAtom
{
public:

public:
    Co64Box(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~Co64Box();
public:
    virtual void display();
};

class StssBox : public F4vBoxAtom
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t sync_count;
    std::vector<uint32_t>sync_table;
public:
    StssBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~StssBox();
public:
    virtual void display();
};

class SdtpBox : public F4vBoxAtom
{
public:

public:
    SdtpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~SdtpBox();
public:
    virtual void display();
};

class MvexBox : public F4vBoxAtom
{
public:

public:
    MvexBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MvexBox();
public:
    virtual void display();
};

class MehdBox : public F4vBoxAtom
{
public:

public:
    MehdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MehdBox();
public:
    virtual void display();
};

class TrexBox : public F4vBoxAtom
{
public:

public:
    TrexBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~TrexBox();
public:
    virtual void display();
};

class AuthBox : public F4vBoxAtom
{
public:

public:
    AuthBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AuthBox();
public:
    virtual void display();
};

class TitlBox : public F4vBoxAtom
{
public:

public:
    TitlBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~TitlBox();
public:
    virtual void display();
};

class DscpBox : public F4vBoxAtom
{
public:

public:
    DscpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~DscpBox();
public:
    virtual void display();
};

class CprtBox : public F4vBoxAtom
{
public:

public:
    CprtBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~CprtBox();
public:
    virtual void display();
};

class UdtaBox : public F4vBoxAtom
{
public:

public:
    UdtaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~UdtaBox();
public:
    virtual void display();
};

class UuidBox : public F4vBoxAtom
{
public:

public:
    UuidBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~UuidBox();
public:
    virtual void display();
};

class MoofBox : public F4vBoxAtom
{
public:

public:
    MoofBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MoofBox();
public:
    virtual void display();
};

class MfhdBox : public F4vBoxAtom
{
public:

public:
    MfhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MfhdBox();
public:
    virtual void display();
};

class TrafBox : public F4vBoxAtom
{
public:
public:
    TrafBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~TrafBox();
public:
    virtual void display();
};

class TfhdBox : public F4vBoxAtom
{
public:

public:
    TfhdBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~TfhdBox();
public:
    virtual void display();
};

class TrunBox : public F4vBoxAtom
{
public:

public:
    TrunBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~TrunBox();
public:
    virtual void display();
};

class MdatBox : public F4vBoxAtom
{
public:

public:
    MdatBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MdatBox();
public:
    virtual void display();
};

class MetaBox : public F4vBoxAtom
{
public:

public:
    MetaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MetaBox();
public:
    virtual void display();
};

class IlstBox : public F4vBoxAtom
{
public:

public:
    IlstBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~IlstBox();
public:
    virtual void display();
};

class FreeBox : public F4vBoxAtom
{
public:

public:
    FreeBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~FreeBox();
public:
    virtual void display();
};

class SkipBox : public F4vBoxAtom
{
public:

public:
    SkipBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~SkipBox();
public:
    virtual void display();
};

class MfraBox : public F4vBoxAtom
{
public:

public:
    MfraBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MfraBox();
public:
    virtual void display();
};

class TfraBox : public F4vBoxAtom
{
public:

public:
    TfraBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~TfraBox();
public:
    virtual void display();
};

class MfroBox : public F4vBoxAtom
{
public:

public:
    MfroBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~MfroBox();
public:
    virtual void display();
};

// box in StsdBox
class RtmpBox : public F4vBoxAtom
{
public:

public:
    RtmpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~RtmpBox();
public:
    virtual void display();
};

class AmhpBox : public F4vBoxAtom
{
public:

public:
    AmhpBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AmhpBox();
public:
    virtual void display();
};

class AmtoBox : public F4vBoxAtom
{
public:

public:
    AmtoBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AmtoBox();
public:
    virtual void display();
};

class EncvBox : public F4vBoxAtom
{
public:

public:
    EncvBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~EncvBox();
public:
    virtual void display();
};

class EncaBox : public F4vBoxAtom
{
public:

public:
    EncaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~EncaBox();
public:
    virtual void display();
};

class EncrBox : public F4vBoxAtom
{
public:

public:
    EncrBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~EncrBox();
public:
    virtual void display();
};

class SinfBox : public F4vBoxAtom
{
public:

public:
    SinfBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~SinfBox();
public:
    virtual void display();
};

class FrmaBox : public F4vBoxAtom
{
public:

public:
    FrmaBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~FrmaBox();
public:
    virtual void display();
};

class SchmBox : public F4vBoxAtom
{
public:

public:
    SchmBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~SchmBox();
public:
    virtual void display();
};

class SchiBox : public F4vBoxAtom
{
public:

public:
    SchiBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~SchiBox();
public:
    virtual void display();
};

class AdkmBox : public F4vBoxAtom
{
public:

public:
    AdkmBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AdkmBox();
public:
    virtual void display();
};

class AhdrBox : public F4vBoxAtom
{
public:

public:
    AhdrBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AhdrBox();
public:
    virtual void display();
};

class AprmBox : public F4vBoxAtom
{
public:

public:
    AprmBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AprmBox();
public:
    virtual void display();
};

class AeibBox : public F4vBoxAtom
{
public:

public:
    AeibBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AeibBox();
public:
    virtual void display();
};

class AkeyBox : public F4vBoxAtom
{
public:

public:
    AkeyBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AkeyBox();
public:
    virtual void display();
};

class ApsBox : public F4vBoxAtom
{
public:

public:
    ApsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~ApsBox();
public:
    virtual void display();
};

class FlxsBox : public F4vBoxAtom
{
public:

public:
    FlxsBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~FlxsBox();
public:
    virtual void display();
};

class AsigBox : public F4vBoxAtom
{
public:

public:
    AsigBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AsigBox();
public:
    virtual void display();
};

class AdafBox : public F4vBoxAtom
{
public:

public:
    AdafBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed, uint32_t off, bool ic);
    virtual ~AdafBox();
public:
    virtual void display();
};

#endif //F4V_PARSEER_F4V_BOX_HPP

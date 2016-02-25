//
// Created by chnvideo on 2016/2/3.
//

#ifndef F4V_PARSEER_F4V_BOX_HPP
#define F4V_PARSEER_F4V_BOX_HPP

#include <string>
#include <vector>
#include <stdio.h>
#include <stdint.h>

class F4vSample
{
public:
    int size;
    int duration;
    int offset;
    int chunk_index;
    int index_in_chunk;
public:
    F4vSample();
    virtual ~F4vSample();
public:
    virtual int display();
};


class F4vBox
{
public:
    // start position
    uint64_t sp;
    // box header size
    uint32_t hs;
    // end position
    uint64_t ep;
    // size
    uint64_t size;
    int32_t type;
public:
    std::vector<F4vBox*> container;
public:
    F4vBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~F4vBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};


class FtypBox : public F4vBox
{
public:
    int32_t major_brand;
    int32_t minor_version;
    std::string compatible_brands;
public:
    FtypBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~FtypBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class PdinBox : public F4vBox
{
public:

public:
    PdinBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~PdinBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class AfraBox : public F4vBox
{
public:
    AfraBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AfraBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class AbstBox : public F4vBox
{
public:
   
public:
    AbstBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AbstBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class AsrtBox : public F4vBox
{
public:

public:
    AsrtBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AsrtBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class AfrtBox : public F4vBox
{
public:

public:
    AfrtBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AfrtBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class MoovBox : public F4vBox
{
public:
    MoovBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed);
    virtual ~MoovBox();
public:
    virtual int initialize(FILE** fp);
    virtual int read(FILE** fp, uint64_t start, uint64_t end);
    int add(F4vBox* fb);
    virtual void display();
};

class MvhdBox : public F4vBox
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
    MvhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MvhdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class TrakBox : public F4vBox
{
public:
public:
    TrakBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed);
    virtual ~TrakBox();
public:
    virtual int initialize(FILE** fp);
    virtual int read(FILE** fp, uint64_t start, uint64_t end);
    int add(F4vBox* fb);
    virtual void display();
};

class TkhdBox : public F4vBox
{
public:
    uint32_t version;
    uint64_t creation_time;
    uint64_t modification_time;
    uint32_t trak_id;
    uint64_t duration;

public:
    TkhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~TkhdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class EdtsBox : public F4vBox
{
public:
public:
    EdtsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~EdtsBox();
public:
    virtual int initialize(FILE** fp);
    virtual int read(FILE** fp, uint64_t start, uint64_t end);
    virtual int add(F4vBox* fb);
    virtual void display();
};

class ElstBox : public F4vBox
{
public:

public:
    ElstBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~ElstBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class MdiaBox : public F4vBox
{
public:
public:
    MdiaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MdiaBox();
public:
    virtual int initialize(FILE** fp);
    virtual int read(FILE** fp, uint64_t start, uint64_t end);
    virtual int add(F4vBox* fb);
    virtual void display();
};

class MdhdBox : public F4vBox
{
public:
    uint32_t version;
    uint64_t creation_time;
    uint64_t modification_time;
    uint32_t timescale;
    uint64_t duration;
public:
    MdhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MdhdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class HdlrBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t predefined;
    uint32_t handler_type;
public:
    HdlrBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~HdlrBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class MinfBox : public F4vBox
{
public:
public:
    MinfBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MinfBox();
public:
    virtual int initialize(FILE** fp);
    virtual int read(FILE** fp, uint64_t start, uint64_t end);
    virtual int add(F4vBox* fb);
    virtual void display();
};

class VmhdBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t graphic_mode;
    uint32_t op_color[3];
public:
    VmhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~VmhdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class SmhdBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    float balance;
public:
    SmhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~SmhdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class HmhdBox : public F4vBox
{
public:

public:
    HmhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~HmhdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class NmhdBox : public F4vBox
{
public:

public:
    NmhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~NmhdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class DinfBox : public F4vBox
{
public:
public:
    DinfBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~DinfBox();
public:
    virtual int initialize(FILE** fp);
    virtual int read(FILE** fp, uint64_t start, uint64_t end);
    virtual int add(F4vBox* fb);
    virtual void display();
};

class DrefBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t entry_count;
public:
    DrefBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~DrefBox();
public:
    virtual int initialize(FILE** fp);
    virtual int read(FILE** fp, uint64_t start, uint64_t end);
    virtual int add(F4vBox* fb);
    virtual void display();
};

class UrlBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
public:
    UrlBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~UrlBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class StblBox : public F4vBox
{
public:
public:
    StblBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~StblBox();
public:
    virtual int initialize(FILE** fp);
    virtual int read(FILE** fp, uint64_t start, uint64_t end);
    virtual int add(F4vBox* fb);
    virtual void display();
};

class StsdBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t count;
    std::vector<F4vBox*>descriptions;
public:
    StsdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~StsdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

typedef struct _stts_record{
    uint32_t sample_count;
    uint32_t sample_delta;
}SttsRecord;

class SttsBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t count;
    std::vector<SttsRecord> stts_records;
public:
    SttsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~SttsBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

typedef struct _ctts_record{
    uint32_t sample_count;
    uint32_t sample_offset;
}CttsRecord;

class CttsBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t count;
    std::vector<CttsRecord>ctts_records;
public:
    CttsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~CttsBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

typedef struct _stsc_record{
    uint32_t first_chunk;
    // samples per chunk
    uint32_t spc;
    // sample description index
    uint32_t sdi;
} StscRecord;

class StscBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t count;
    std::vector<StscRecord> stsc_records;
public:
    StscBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~StscBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class StszBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t constant_size;
    uint32_t size_count;
    std::vector<uint32_t>size_table;
public:
    StszBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~StszBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class StcoBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t offset_count;
    std::vector<uint32_t>offsets;
public:
    StcoBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~StcoBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class Co64Box : public F4vBox
{
public:

public:
    Co64Box(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~Co64Box();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class StssBox : public F4vBox
{
public:
    uint32_t version;
    uint32_t flags;
    uint32_t sync_count;
    std::vector<uint32_t>sync_table;
public:
    StssBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~StssBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class SdtpBox : public F4vBox
{
public:

public:
    SdtpBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~SdtpBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class MvexBox : public F4vBox
{
public:
public:
    MvexBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MvexBox();
public:
    virtual int initialize(FILE** fp);
    virtual int read(FILE** fp, uint64_t start, uint64_t end);
    virtual int add(F4vBox* fb);
    virtual void display();
};

class MehdBox : public F4vBox
{
public:

public:
    MehdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MehdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class TrexBox : public F4vBox
{
public:

public:
    TrexBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~TrexBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class AuthBox : public F4vBox
{
public:

public:
    AuthBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AuthBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class TitlBox : public F4vBox
{
public:

public:
    TitlBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~TitlBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class DscpBox : public F4vBox
{
public:

public:
    DscpBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~DscpBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class CprtBox : public F4vBox
{
public:

public:
    CprtBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~CprtBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class UdtaBox : public F4vBox
{
public:

public:
    UdtaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~UdtaBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class UuidBox : public F4vBox
{
public:

public:
    UuidBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed);
    virtual ~UuidBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class MoofBox : public F4vBox
{
public:
public:
    MoofBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed);
    virtual ~MoofBox();
public:
    int read(FILE** fp, uint64_t start, uint64_t end);
public:
    virtual int initialize(FILE** fp);
    virtual int add(F4vBox* fb);
    virtual void display();
};

class MfhdBox : public F4vBox
{
public:
    
public:
    MfhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MfhdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class TrafBox : public F4vBox
{
public: 
public:
    TrafBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~TrafBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class TfhdBox : public F4vBox
{
public:

public:
    TfhdBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~TfhdBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class TrunBox : public F4vBox
{
public:

public:
    TrunBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~TrunBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class MdatBox : public F4vBox
{
public:

public:
    MdatBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MdatBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class MetaBox : public F4vBox
{
public:

public:
    MetaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MetaBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class IlstBox : public F4vBox
{
public:

public:
    IlstBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~IlstBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class FreeBox : public F4vBox
{
public:

public:
    FreeBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed);
    virtual ~FreeBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class SkipBox : public F4vBox
{
public:

public:
    SkipBox(uint64_t st, uint64_t sz, int32_t ty, uint32_t hs, uint64_t ed);
    virtual ~SkipBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class MfraBox : public F4vBox
{
public:

public:
    MfraBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MfraBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class TfraBox : public F4vBox
{
public:

public:
    TfraBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~TfraBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};

class MfroBox : public F4vBox
{
public:

public:
    MfroBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~MfroBox();
public:
    virtual int initialize(FILE** fp);
    virtual void display();
};


/*
// box in StsdBox
class RtmpBox : public F4vBox
{
public:

public:
    RtmpBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~RtmpBox();
public:
    virtual void display();
};

class AmhpBox : public F4vBox
{
public:

public:
    AmhpBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AmhpBox();
public:
    virtual void display();
};

class AmtoBox : public F4vBox
{
public:

public:
    AmtoBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AmtoBox();
public:
    virtual void display();
};

class EncvBox : public F4vBox
{
public:

public:
    EncvBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~EncvBox();
public:
    virtual void display();
};

class EncaBox : public F4vBox
{
public:

public:
    EncaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~EncaBox();
public:
    virtual void display();
};

class EncrBox : public F4vBox
{
public:

public:
    EncrBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~EncrBox();
public:
    virtual void display();
};

class SinfBox : public F4vBox
{
public:

public:
    SinfBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~SinfBox();
public:
    virtual void display();
};

class FrmaBox : public F4vBox
{
public:

public:
    FrmaBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~FrmaBox();
public:
    virtual void display();
};

class SchmBox : public F4vBox
{
public:

public:
    SchmBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~SchmBox();
public:
    virtual void display();
};

class SchiBox : public F4vBox
{
public:

public:
    SchiBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~SchiBox();
public:
    virtual void display();
};

class AdkmBox : public F4vBox
{
public:

public:
    AdkmBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AdkmBox();
public:
    virtual void display();
};

class AhdrBox : public F4vBox
{
public:

public:
    AhdrBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AhdrBox();
public:
    virtual void display();
};

class AprmBox : public F4vBox
{
public:

public:
    AprmBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AprmBox();
public:
    virtual void display();
};

class AeibBox : public F4vBox
{
public:

public:
    AeibBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AeibBox();
public:
    virtual void display();
};

class AkeyBox : public F4vBox
{
public:

public:
    AkeyBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AkeyBox();
public:
    virtual void display();
};

class ApsBox : public F4vBox
{
public:

public:
    ApsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~ApsBox();
public:
    virtual void display();
};

class FlxsBox : public F4vBox
{
public:

public:
    FlxsBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~FlxsBox();
public:
    virtual void display();
};

class AsigBox : public F4vBox
{
public:

public:
    AsigBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AsigBox();
public:
    virtual void display();
};

class AdafBox : public F4vBox
{
public:

public:
    AdafBox(uint64_t start_position, uint64_t size, int32_t type, uint32_t header_size, uint64_t end_position);
    virtual ~AdafBox();
public:
    virtual void display();
};
*/

#endif //F4V_PARSEER_F4V_BOX_HPP

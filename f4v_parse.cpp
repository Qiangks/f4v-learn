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

#define BOX_HEADER_SIZE       8
#define BOX_EXTENDED_SIZE   8
#define DREF_OFFSET               8

enum BOXTYPE {
    ftyp = 0x66747970, pdin = 0x7064696E, afra = 0x61667261, abst = 0x61627374, asrt = 0x61737274, afrt = 0x61667274,
    moov = 0x6D6F6F76, mvhd = 0x6D766864, trak = 0x7472616B, tkhd = 0x746B6864, edts = 0x65647473, elst = 0x656C7374,
    mdia = 0x6D646961, mdhd = 0x6D646864, hdlr = 0x68646C72, minf = 0x6D696E66, vmhd = 0x766D6864, smhd = 0x736D6864,
    hmhd = 0x686D6864, nmhd = 0x6E6D6864, dinf = 0x64696E66, dref = 0x64726566, url = 0x75726C20,  stbl = 0x7374626C,
    stsd = 0x73747364, stts = 0x73747473, ctts = 0x63747473, stsc = 0x73747363, stsz = 0x7374737A, stco = 0x7374636F,
    co64 = 0x636F3634, stss = 0x73747373, sdtp = 0x73647470, mvex = 0x6D766578, mehd = 0x6D656864, trex = 0x74726578,
    auth = 0x61757468, titl = 0x7469746C, dscp = 0x64736370, cprt = 0x63707274, udta = 0x75647461, uuid = 0x75756964,
    moof = 0x6D6F6F66, mfhd = 0x6D666864, traf = 0x74726166, tfhd = 0x74666864, trun = 0x7472756E, mdat = 0x6D646174,
    meta = 0x6D657461, ilst = 0x696C7374, frde = 0x66726565, skip = 0x736b6970, mfra = 0x6D667261, tfra = 0x74667261,
    mfro = 0x6d66726f, rtmp = 0x72746D70, amhp = 0x616D6870, amto = 0x616D746F, encv = 0x656E6376, enca = 0x656E6361,
    encr = 0x656E6372, sinf = 0x73696E66, frma = 0x66726D61, schm = 0x7363686D, schi = 0x73636869, adkm = 0x61646B6D,
    ahdr = 0x61686472, aprm = 0x6165726D, aeib = 0x61656962, akey = 0x616B6579, aps = 0x61707320, flxs = 0x666C7873,
    asig = 0x61736967, adaf = 0x61646166,
    styl = 0x7374796C, hlit = 0x686C6974, hclr = 0x68636C72, krok = 0x6B726F6B, dlay = 0x646C6179, drpo = 0x6472706F,
    drpt = 0x64727074, href = 0x68726566, tbox = 0x74626F78, 
    avc1 = 0x61766331,
};

IF4vParser::IF4vParser()
{
}

IF4vParser::~IF4vParser()
{
}

F4vFileParser::F4vFileParser(std::string str)
{
    name = str;
    is_eof = false;


    ftypx = NULL;
    pdinx = NULL;
    afrax = NULL;
    abstx = NULL;
    moovx = NULL;
    uuidx = NULL;
    moofx = NULL;
    mdatx = NULL;
    metax = NULL;
    freex = NULL;
    skipx = NULL;
    mfrax = NULL;
}

F4vFileParser::~F4vFileParser()
{
    for(vector<F4vBoxAtom*>::iterator it =f4v_atomes.begin(); it != f4v_atomes.end(); it++) {
        delete *it;
    }
}

int F4vFileParser::initialize()
{
    int ret = ERROR_SUCCESS;
    
    if ((fp = fopen(name.c_str(), "r")) == NULL) {
        ret = ERROR_SYSTEM_OPEN_FAILED;
        f4v_error("open the file %s failed, ret=%d", name.c_str(), ret);
        return ret;
    }

    if ((ret = start()) != ERROR_SUCCESS) {
        f4v_error("start parse file %s error, ret=%d", name.c_str(), ret);
        return ret;
    }

    return ret;
}

int F4vFileParser:: start()
{
    int ret = ERROR_SUCCESS;

    if ((ret = read(0, get_filesize())) != ERROR_SUCCESS) {
        f4v_error("read the file %s error, ret=%d", name.c_str(), ret);
        return ret;
    }

    if ((ret = parse()) != ERROR_SUCCESS) {
        f4v_error("parse the box from the file %s error, ret=%d", name.c_str(), ret);
        return ret;
    }
    
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
        
        F4vBoxAtom* fb = NULL;
        switch(type){
            case ftyp:
                fb = new FtypBox(sp, size, type, header_size, ep, 0, false);
                break;
            case pdin:
                fb = new PdinBox(sp, size, type, header_size, ep, 0, false);
                break;
            case afra:
                fb = new AfraBox(sp, size, type, header_size, ep, 0, false);
                break;
            case abst:
                fb = new AbstBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case asrt:
                fb = new AsrtBox(sp, size, type, header_size, ep, 0, false);
                break;
            case afrt:
                fb = new AfrtBox(sp, size, type, header_size, ep, 0, false);
                break;
            case moov:
                fb = new MoovBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case trak:
                fb = new TrakBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case tkhd:
                fb = new TkhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case edts:
                fb = new EdtsBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case elst:
                fb = new ElstBox(sp, size, type, header_size, ep, 0, false);
                break;
            case mdia:
                fb = new MdiaBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case mdhd:
                fb = new MdhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case hdlr:
                fb = new HdlrBox(sp, size, type, header_size, ep, 0, false);
                break;
            case minf:
                fb = new MinfBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case vmhd:
                fb = new VmhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case smhd:
                fb = new SmhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case hmhd:
                fb = new HmhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case nmhd:
                fb = new NmhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case dinf:
                fb = new DinfBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case dref:
                fb = new DrefBox(sp, size, type, header_size, ep, header_size + DREF_OFFSET, true);
                break;
            case url:
                fb = new UrlBox(sp, size, type, header_size, ep, 0, false);
                break;
            case stbl:
                fb = new StblBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case stsd:
                fb = new StsdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case stts:
                fb = new SttsBox(sp, size, type, header_size, ep, 0, false);
                break;
            case ctts:
                fb = new CttsBox(sp, size, type, header_size, ep, 0, false);
                break;
            case stsc:
                fb = new StscBox(sp, size, type, header_size, ep, 0, false);
                break;
            case stsz:
                fb = new StszBox(sp, size, type, header_size, ep, 0, false);
                break;
            case stco:
                fb = new StcoBox(sp, size, type, header_size, ep, 0, false);
                break;
            case co64:
                fb = new Co64Box(sp, size, type, header_size, ep, 0, false);
                break;
            case stss:
                fb = new StssBox(sp, size, type, header_size, ep, 0, false);
                break;
            case sdtp:
                fb = new SdtpBox(sp, size, type, header_size, ep, 0, false);
                break;
            case mvex:
                fb = new MvexBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case mehd:
                fb = new MehdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case trex:
                fb = new TrexBox(sp, size, type, header_size, ep, 0, false);
                break;
            case auth:
                fb = new AuthBox(sp, size, type, header_size, ep, 0, false);
                break;
            case titl:
                fb = new TitlBox(sp, size, type, header_size, ep, 0, false);
                break;
            case dscp:
                fb = new DscpBox(sp, size, type, header_size, ep, 0, false);
                break;
            case cprt:
                fb = new CprtBox(sp, size, type, header_size, ep, 0, false);
                break;
            case udta:
                fb = new UdtaBox(sp, size, type, header_size, ep, 0, false);
                break;
            case uuid:
                fb = new UuidBox(sp, size, type, header_size, ep, 0, false);
                break;
            case moof:
                fb = new MoofBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case mfhd:
                fb = new MfhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case traf:
                fb = new TrafBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case tfhd:
                fb = new TfhdBox(sp, size, type, header_size, ep, 0, false);
                break;
            case trun:
                fb = new TrunBox(sp, size, type, header_size, ep, 0, false);
                break;
            case mdat:
                fb = new MdatBox(sp, size, type, header_size, ep, 0, false);
                break;
            case meta:
                fb = new MetaBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case ilst:
                fb = new IlstBox(sp, size, type, header_size, ep, 0, false);
                break;
            case frde:
                fb = new FreeBox(sp, size, type, header_size, ep, 0, false);
                break;
            case skip:
                fb = new SkipBox(sp, size, type, header_size, ep, 0, false);
                break;
            case mfra:
                fb  = new MfraBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case tfra:
                fb = new TfraBox(sp, size, type, header_size, ep, 0, false);
                break;
            case mfro:
                fb = new MfroBox(sp, size, type, header_size, ep, 0, false);
                break;
            case rtmp:
                fb = new RtmpBox(sp, size, type, header_size, ep, 0, false);
                break;
            case amhp:
                fb = new AmhpBox(sp, size, type, header_size, ep, 0, false);
                break;
            case amto:
                fb = new AmtoBox(sp, size, type, header_size, ep, 0, false);
                break;
            case encv:
                fb = new EncvBox(sp, size, type, header_size, ep, 0, false);
                break;
            case enca:
                fb = new EncaBox(sp, size, type, header_size, ep, 0, false);
                break;
            case encr:
                fb = new EncrBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case sinf:
                fb = new SinfBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case frma:
                fb = new FrmaBox(sp, size, type, header_size, ep, 0, false);
                break;
            case schm:
                fb = new SchmBox(sp, size, type, header_size, ep, 0, false);
                break;
            case schi:
                fb = new SchiBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case adkm:
                fb = new AdkmBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case ahdr:
                fb = new AhdrBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case aprm:
                fb = new AprmBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case aeib:
                fb = new AeibBox(sp, size, type, header_size, ep, 0, false);
                break;
            case akey:
                fb = new AkeyBox(sp, size, type, header_size, ep, header_size, true);
                break;
            case aps:
                fb = new ApsBox(sp, size, type, header_size, ep, 0, false);
                break;
            case flxs:
                fb = new FlxsBox(sp, size, type, header_size, ep, 0, false);
                break;
            case asig:
                fb = new AsigBox(sp, size, type, header_size, ep, 0, false);
                break;
            case adaf:
                fb = new AdafBox(sp, size, type, header_size, ep, 0, false);
                break;
            default:
                break;
        }

        assert(fb != NULL);

        if (fb->is_container) {
            if (( ret = read(fb->start + fb->offset, fb->end)) != ERROR_SUCCESS) {
                f4v_error("read the box %s error, ret=%d", f4v_int2str(fb->type).c_str(), ret);
                return ret;
            }
        }

        ::fseek(fp, fb->end, SEEK_SET);
    }

    return ret;
}

int F4vFileParser::read2(uint64_t start, uint64_t end)
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
            case ftyp:
                ftypx = new FtypBox(sp, size, type, header_size, ep, 0, false);
                f4v_atomes.push_back(ftypx);
                break;
            case pdin:
                pdinx = new PdinBox(sp, size, type, header_size, ep, 0, false);
                set_vecbox(pdinx);
                break;
            case afra:
                afrax = new AfraBox(sp, size, type, header_size, ep, 0, false);
                set_vecbox(afrax);
                break;
            case abst:
                abstx = new AbstBox(sp, size, type, header_size, ep, header_size, true);
                set_vecbox(abstx);
                break;
            case moov:
                moovx = new MoovBox(sp, size, type, header_size, ep, header_size, true);
                set_vecbox(moovx);
                read_moovx();
                break;
            case uuid:
                uuidx = new UuidBox(sp, size, type, header_size, ep, 0, false);
                set_vecbox(uuidx);
                break;
            case moof:
                moofx = new MoofBox(sp, size, type, header_size, ep, header_size, true);
                set_vecbox(moofx);
                break;
            case mdat:
                mdatx = new MdatBox(sp, size, type, header_size, ep, 0, false);
                set_vecbox(mdatx);
                break;
            case meta:
                metax = new MetaBox(sp, size, type, header_size, ep, header_size, true);
                set_vecbox(metax);
                break;
            case frde:
                freex = new FreeBox(sp, size, type, header_size, ep, 0, false);
                set_vecbox(freex);
                break;
            case skip:
                skipx = new SkipBox(sp, size, type, header_size, ep, 0, false);
                set_vecbox(skipx);
                break;
            case mfra:
                mfrax  = new MfraBox(sp, size, type, header_size, ep, header_size, true);
                set_vecbox(mfrax);
                break;
            default:
                break;
        }

        ::fseek(fp, ep, SEEK_SET);
    }

    return ret;
}

int F4vFileParser::set_vecbox(F4vBoxAtom* fb)
{
    int ret = ERROR_SUCCESS;

    vector<F4vBoxAtom*>::iterator it;
    if((it = ::find(f4v_atomes.begin(), f4v_atomes.end(), fb)) != f4v_atomes.end()) {
        return ret;
    }
    f4v_atomes.push_back(fb);
    
    return ret;
}

int F4vFileParser::read_ftypx()
{
    int ret = ERROR_SUCCESS;
    
    int size = ftypx->size - ftypx->header_size;
    ::fseek(fp, ftypx->start + ftypx->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    ftypx->major_brand = f4v_bytes_to_uint32(buf, 4);
    ftypx->minor_version = f4v_bytes_to_uint32(&buf[4], 4);

    stringstream ss;
    for(int i = 8; i < size; i++) {
        ss << buf[i];
    }
    ftypx->compatible_brands = ss.str();

    return ret;
}

int F4vFileParser::read_pdinx()
{
    int ret = ERROR_SUCCESS;

    return ret;
}

int F4vFileParser::read_afrax()
{
    int ret = ERROR_SUCCESS;

    return ret;
}

int F4vFileParser::read_abstx()
{
    int ret = ERROR_SUCCESS;

    return ret;
}

int F4vFileParser::read_moovx()
{
    int ret = ERROR_SUCCESS;

    if ((ret = moovx->read(fp, moovx->start + moovx->header_size, moovx->end)) != ERROR_SUCCESS) {
        f4v_error("read the moovx failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int F4vFileParser::read_uuidx()
{
    int ret = ERROR_SUCCESS;
    
    int size = ftypx->size - ftypx->header_size;
    ::fseek(fp, ftypx->start + ftypx->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    ftypx->major_brand = f4v_bytes_to_uint32(buf, 4);
    ftypx->minor_version = f4v_bytes_to_uint32(&buf[4], 4);

    stringstream ss;
    for(int i = 8; i < size; i++) {
        ss << buf[i];
    }
    ftypx->compatible_brands = ss.str();

    return ret;
}

int F4vFileParser::read_moofx()
{
    int ret = ERROR_SUCCESS;

    if ((ret = moofx->read(fp, moofx->start + moofx->header_size, moofx->end)) != ERROR_SUCCESS) {
        f4v_error("read the moofx failed. ret=%d", ret);
        return ret;
    }

    return ret;
}

int F4vFileParser::read_mdatx()
{
    int ret = ERROR_SUCCESS;
    
    int size = ftypx->size - ftypx->header_size;
    ::fseek(fp, ftypx->start + ftypx->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    ftypx->major_brand = f4v_bytes_to_uint32(buf, 4);
    ftypx->minor_version = f4v_bytes_to_uint32(&buf[4], 4);

    stringstream ss;
    for(int i = 8; i < size; i++) {
        ss << buf[i];
    }
    ftypx->compatible_brands = ss.str();

    return ret;
}

int F4vFileParser::read_metax()
{
    int ret = ERROR_SUCCESS;
    
    int size = ftypx->size - ftypx->header_size;
    ::fseek(fp, ftypx->start + ftypx->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    ftypx->major_brand = f4v_bytes_to_uint32(buf, 4);
    ftypx->minor_version = f4v_bytes_to_uint32(&buf[4], 4);

    stringstream ss;
    for(int i = 8; i < size; i++) {
        ss << buf[i];
    }
    ftypx->compatible_brands = ss.str();

    return ret;
}

int F4vFileParser::read_freex()
{
    int ret = ERROR_SUCCESS;
    
    int size = ftypx->size - ftypx->header_size;
    ::fseek(fp, ftypx->start + ftypx->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    ftypx->major_brand = f4v_bytes_to_uint32(buf, 4);
    ftypx->minor_version = f4v_bytes_to_uint32(&buf[4], 4);

    stringstream ss;
    for(int i = 8; i < size; i++) {
        ss << buf[i];
    }
    ftypx->compatible_brands = ss.str();

    return ret;
}

int F4vFileParser::read_skipx()
{
    int ret = ERROR_SUCCESS;
    
    int size = ftypx->size - ftypx->header_size;
    ::fseek(fp, ftypx->start + ftypx->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    ftypx->major_brand = f4v_bytes_to_uint32(buf, 4);
    ftypx->minor_version = f4v_bytes_to_uint32(&buf[4], 4);

    stringstream ss;
    for(int i = 8; i < size; i++) {
        ss << buf[i];
    }
    ftypx->compatible_brands = ss.str();

    return ret;
}

int F4vFileParser::read_mfrax()
{
    int ret = ERROR_SUCCESS;
    
    int size = ftypx->size - ftypx->header_size;
    ::fseek(fp, ftypx->start + ftypx->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    ftypx->major_brand = f4v_bytes_to_uint32(buf, 4);
    ftypx->minor_version = f4v_bytes_to_uint32(&buf[4], 4);

    stringstream ss;
    for(int i = 8; i < size; i++) {
        ss << buf[i];
    }
    ftypx->compatible_brands = ss.str();

    return ret;
}

int F4vFileParser::parse()
{
    int ret = ERROR_SUCCESS;
    vector<F4vBoxAtom*>::iterator it;
    for (it = f4v_atomes.begin(); it != f4v_atomes.end(); it++) {
        F4vBoxAtom* fm = *it;
        switch (fm->type) {
            case ftyp:
                parse_ftyp(&fm);
                break;
            case moov:
                parse_moov(&fm);
                break;
            case mvhd:
                parse_mvhd(&fm);
                break;
            case trak:
                parse_trak(&fm);
                break;
            case tkhd:
                parse_tkhd(&fm);
                break;
            case mdia:
                parse_mdia(&fm);
                break;
            case mdhd:
                parse_mdhd(&fm);
                break;
            case hdlr:
                parse_hdlr(&fm);
                break;
            case minf:
                parse_minf(&fm);
                break;
            case vmhd:
                parse_vmhd(&fm);
                break;
            case dinf:
                parse_dinf(&fm);
                break;
            case dref:
                parse_dref(&fm);
                break;
            case url:
                parse_url(&fm);
                break;
            case stbl:
                parse_stbl(&fm);
                break;
            case stsd:
                parse_stsd(&fm);
                break;
            case smhd:
                parse_smhd(&fm);
                break;
            default:
                break;
        }
    }
        

    if ((ret = parse_sample()) != ERROR_SUCCESS) {
        f4v_error("parse sample failed, ret=%d", ret);
        return ret;
    }

    return ret;
}

int F4vFileParser::show()
{
    int ret = ERROR_SUCCESS;
    
    vector<F4vBoxAtom*>::iterator it;
    for (it = f4v_atomes.begin(); it != f4v_atomes.end(); it++) {
        F4vBoxAtom* fm = *it;
        fm->display();
    }

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

int F4vFileParser::parse_sample()
{
    int ret = ERROR_SUCCESS;
/*
    std::vector<F4vSample> f4v_samples;

    uint32_t total_chunk = stcob->offset_count;

    F4vChunk f4v_chunk[total_chunk];

    // restore the chunk---->sample list
    uint32_t last_chk_no = total_chunk + 1;
    for (int i = stscb->count - 1; i >= 0; --i) {
        uint32_t beg_real_chkno = stscb->stsc_records[i].first_chunk;
        
        for (uint32_t chk_i = beg_real_chkno - 1; chk_i < last_chk_no - 1; chk_i++) {
            f4v_chunk[chk_i].sample_count = stscb->stsc_records[i].spc;      
            f4v_chunk[chk_i].sdi = stscb->stsc_records[i].sdi;
        }
        last_chk_no = beg_real_chkno; 
    }

    // restore the sample---->chunk list
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

    // get each sample's size, duration
    for(uint32_t i = 0; i < stszb->size_count; i++) {
        f4v_samples[i].size = stszb->size_table[i];
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
    int record_no = 0;
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
*/    
    return ret;
}

void F4vFileParser::parse_ftyp(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);
    
    FtypBox* ft = dynamic_cast<FtypBox*>(pfb);
    ft->major_brand = f4v_bytes_to_uint32(buf, 4);
    ft->minor_version = f4v_bytes_to_uint32(&buf[4], 4);

    stringstream ss;
    for(int i = 8; i < size; i++) {
        ss << buf[i];
    }
    ft->compatible_brands = ss.str();
}

void F4vFileParser::parse_moov(F4vBoxAtom** ppfb)
{
}

void F4vFileParser::parse_mvhd(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    MvhdBox* mb = dynamic_cast<MvhdBox*>(pfb);
    mb->version = f4v_bytes_to_uint32(buf, 1);
    mb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    if (mb->version == 0) {
        mb->creation_time = f4v_bytes_to_uint32(&buf[4], 4);
        mb->modification_time = f4v_bytes_to_uint32(&buf[8], 4);
        mb->timescale = f4v_bytes_to_uint32(&buf[12], 4);
        mb->duration = f4v_bytes_to_uint32(&buf[16], 4);
        mb->rate = f4v_bytes_to_uint32(&buf[20], 2) + (float)f4v_bytes_to_uint32(&buf[22], 2)/10;
    } else {
        mb->creation_time = f4v_bytes_to_uint32(&buf[4], 8);
        mb->modification_time = f4v_bytes_to_uint32(&buf[12], 8);
        mb->timescale = f4v_bytes_to_uint32(&buf[20], 4);
        mb->duration = f4v_bytes_to_uint32(&buf[24], 8);
        mb->rate = f4v_bytes_to_uint32(&buf[32], 2) + (float)f4v_bytes_to_uint32(&buf[34], 2)/10;
    }
}

void F4vFileParser::parse_trak(F4vBoxAtom** ppfb)
{
}

void F4vFileParser::parse_tkhd(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    TkhdBox* tb = dynamic_cast<TkhdBox*>(pfb);
    tb->version = f4v_bytes_to_uint32(buf, 1);
    if (tb->version == 0) {
        tb->creation_time = f4v_bytes_to_uint32(&buf[4], 4);
        tb->modification_time = f4v_bytes_to_uint32(&buf[8], 4);
        tb->trak_id= f4v_bytes_to_uint32(&buf[12], 4);
        tb->duration = f4v_bytes_to_uint32(&buf[20], 4);
    } else {
        tb->creation_time = f4v_bytes_to_uint32(&buf[4], 8);
        tb->modification_time = f4v_bytes_to_uint32(&buf[12], 8);
        tb->trak_id = f4v_bytes_to_uint32(&buf[20], 4);
        tb->duration = f4v_bytes_to_uint32(&buf[28], 8);
    }
    
}

void F4vFileParser::parse_mdia(F4vBoxAtom** ppfb)
{
}

void F4vFileParser::parse_mdhd(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    MdhdBox* mb = dynamic_cast<MdhdBox*>(pfb);
    mb->version = f4v_bytes_to_uint32(buf, 1);
    if (mb->version == 0) {
        mb->creation_time = f4v_bytes_to_uint32(&buf[4], 4);
        mb->modification_time = f4v_bytes_to_uint32(&buf[8], 4);
        mb->timescale = f4v_bytes_to_uint32(&buf[12], 4);
        mb->duration = f4v_bytes_to_uint32(&buf[16], 4);
    } else {
        mb->creation_time = f4v_bytes_to_uint32(&buf[4], 8);
        mb->modification_time = f4v_bytes_to_uint32(&buf[12], 8);
        mb->timescale = f4v_bytes_to_uint32(&buf[20], 4);
        mb->duration = f4v_bytes_to_uint32(&buf[24], 8);
    }
}

void F4vFileParser::parse_hdlr(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    HdlrBox* hb = dynamic_cast<HdlrBox*>(pfb);
    hb->version  = f4v_bytes_to_uint32(buf, 1);
    hb->handler_type = f4v_bytes_to_uint32(&buf[8], 4);
}

void F4vFileParser::parse_minf(F4vBoxAtom** ppfb)
{
}

void F4vFileParser::parse_vmhd(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    VmhdBox* vb = dynamic_cast<VmhdBox*>(pfb);
    vb->version = f4v_bytes_to_uint32(buf, 1);
    vb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    vb->graphic_mode = f4v_bytes_to_uint32(&buf[4], 2);
    vb->op_color[0] = f4v_bytes_to_uint32(&buf[6], 2);
    vb->op_color[1] = f4v_bytes_to_uint32(&buf[8], 2);
    vb->op_color[2] = f4v_bytes_to_uint32(&buf[10], 2);
}

void F4vFileParser::parse_dinf(F4vBoxAtom** ppfb)
{
}

void F4vFileParser::parse_dref(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    DrefBox* db = dynamic_cast<DrefBox*>(pfb);
    db->version = f4v_bytes_to_uint32(buf, 1);
    db->flags = f4v_bytes_to_uint32(&buf[1], 3);
    db->entry_count = f4v_bytes_to_uint32(&buf[4], 4);
}

void F4vFileParser::parse_url(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    UrlBox* ub = dynamic_cast<UrlBox*>(pfb);
    ub->version = f4v_bytes_to_uint32(buf, 1);
    ub->flags = f4v_bytes_to_uint32(&buf[1], 3);
}

void F4vFileParser::parse_stbl(F4vBoxAtom** ppfb)
{
}

void F4vFileParser::parse_stsd(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    // pfb buf 
    StsdBox* sb = dynamic_cast<StsdBox*>(pfb);
    sb->version = f4v_bytes_to_uint32(buf, 1);
    sb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    sb->count = f4v_bytes_to_uint32(&buf[4], 4);
    
    for (int i = 0; i < sb->count; i++) {
        
    }
}

SttsBox* F4vFileParser::parse_stts(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    SttsBox* sb = dynamic_cast<SttsBox*>(pfb);
    sb->version = f4v_bytes_to_uint32(buf, 1);
    sb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    sb->count = f4v_bytes_to_uint32(&buf[4], 4);
    for (uint32_t i = 0; i < sb->count; i++) {
        SttsRecord sr;
        sr.sample_count =  f4v_bytes_to_uint32(&buf[8+i*8], 4);
        sr.sample_delta = f4v_bytes_to_uint32(&buf[8+i*8+4], 4);
        sb->stts_records.push_back(sr);
    }

    return sb;
}

CttsBox* F4vFileParser::parse_ctts(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    CttsBox* cb = dynamic_cast<CttsBox*>(pfb);
    cb->version = f4v_bytes_to_uint32(buf, 1);
    cb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    cb->count = f4v_bytes_to_uint32(&buf[4], 4);
    for(uint32_t i = 0; i < cb->count; i++) {
        CttsRecord cr;
        cr.sample_count = f4v_bytes_to_uint32(&buf[8+i*8], 4);
        cr.sample_offset = f4v_bytes_to_uint32(&buf[12+i*8], 4);
        cb->ctts_records.push_back(cr);
    }

    return cb;
}

StscBox* F4vFileParser::parse_stsc(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    StscBox* sb = dynamic_cast<StscBox*>(pfb);
    sb->version = f4v_bytes_to_uint32(buf, 1);
    sb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    sb->count = f4v_bytes_to_uint32(&buf[4], 4);
    for (int i = 0; i< sb->count; i++) {
        StscRecord sr;
        sr.first_chunk = f4v_bytes_to_uint32(&buf[8 +i*12], 4);
        sr.spc = f4v_bytes_to_uint32(&buf[12+i*12], 4);
        sr.sdi = f4v_bytes_to_uint32(&buf[16+i*12], 4);
        sb->stsc_records.push_back(sr);
    }

    return sb;
}

StszBox* F4vFileParser::parse_stsz(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    StszBox* sb = dynamic_cast<StszBox*>(pfb);
    sb->version = f4v_bytes_to_uint32(buf, 1);
    sb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    sb->constant_size = f4v_bytes_to_uint32(&buf[4], 4);
    sb->size_count = f4v_bytes_to_uint32(&buf[8], 4);
    for (uint32_t i = 0; i < sb->size_count; i++) {
        uint32_t st = f4v_bytes_to_uint32(&buf[8+i*4], 4);
        sb->size_table.push_back(st);
    }

    return sb;
}

StcoBox* F4vFileParser::parse_stco(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    StcoBox* sb = dynamic_cast<StcoBox*>(pfb);
    sb->version = f4v_bytes_to_uint32(buf, 1);
    sb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    sb->offset_count = f4v_bytes_to_uint32(&buf[4], 4);
    for (uint32_t i = 0; i < sb->offset_count; i++) {
        uint32_t offset = f4v_bytes_to_uint32(&buf[8+i*4], 4);
        sb->offsets.push_back(offset);
    }
    
    return sb;
}

StssBox* F4vFileParser::parse_stss(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    StssBox* sb = dynamic_cast<StssBox*>(pfb);
    sb->version = f4v_bytes_to_uint32(buf, 1);
    sb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    sb->sync_count = f4v_bytes_to_uint32(&buf[4], 4);
    for (uint32_t i = 0; i < sb->sync_count; i++) {
        uint32_t st = f4v_bytes_to_uint32(&buf[8+i*4], 4);
        sb->sync_table.push_back(st);
    }

    return sb;
}

void F4vFileParser::parse_smhd(F4vBoxAtom** ppfb)
{
    F4vBoxAtom* pfb = *ppfb;
    int size = pfb->size - pfb->header_size;
    ::fseek(fp, pfb->start + pfb->header_size, SEEK_SET);
    unsigned char buf[size];
    ::fread(buf, 1, size, fp);

    uint32_t version;
    uint32_t flags;
    float balance;
    
    SmhdBox* sb = dynamic_cast<SmhdBox*>(pfb);
    sb->version = f4v_bytes_to_uint32(buf, 1);
    sb->flags = f4v_bytes_to_uint32(&buf[1], 3);
    sb->balance = (float)f4v_bytes_to_uint32(&buf[4], 1) + (float)f4v_bytes_to_uint32(&buf[5], 1)/10;
}

void F4vFileParser::parse_free(F4vBoxAtom** ppfb)
{
}

void F4vFileParser::parse_mdat(F4vBoxAtom** ppfb)
{
}
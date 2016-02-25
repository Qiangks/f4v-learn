//
// Created by chnvideo on 2016/2/2.
//

#ifndef F4V_PARSEER_F4V_TYPE_HPP
#define F4V_PARSEER_F4V_TYPE_HPP

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

#define ERROR_SUCCESS       0

#define ERROR_SYSTEM_OPEN_FAILED                1001
#define ERROR_READ_BOX_HEADER_FAILED        1002
#define ERROR_END_POSITION                 1003
#define ERROR_SYSTEM_READ_FAILED                               1004

#endif //F4V_PARSEER_F4V_TYPE_HPP

#ifndef DECODEINFO_MP3
#define DECODEINFO_MP3

#include "stdio.h"
#include "util.h"

struct ID3v2Header //��ǩͷ��10���ֽڣ�ͨ�����������Ϣ���ǿ���֪��һ��MP3�ļ��ǲ�����ID3v2��Ϣ����������Ǿ�֪����ID3v2����������ܳ��ȡ�
{
    char Identify[3];       // ID3v2�̶���־��ID3
    char Ver;               // ���汾�ţ�ID3v2����3
    char Rever;             // ���汾�ţ�һ�㶼Ϊ0
    char Flag;              // ��־λ��һ��Ϊ0������Ϊabc00000
                            //  a-��ʾ�Ƿ�ʹ��Unsynchronisation
                            //  b-��ʾ�Ƿ�����չͷ����һ��û�У�����һ�㲻����
                            //  c-��ʾ�Ƿ�Ϊ���Ա�ǩ��99.9%�ı�ǩ�����ǲ����õģ�����һ�㲻���ã�

    unsigned char Size[4];           // ��ǩ��С��һ���ĸ��ֽڣ���ÿ���ֽ�ֻʹ��7λ�����λ��ʹ�ú�Ϊ0�����Ը�ʽ���£�
                            // 0xxxxxxx 0xxxxxxx 0xxxxxxx 0xxxxxxx�������СʱҪ��0ȥ�����õ�һ��28λ�Ķ������������Ǳ�ǩ�Ĵ�С.
                            // ID3size =(Size[0]&0x7F)*0x200000 +(Size[1]&0x7F)*0x400 +(Size[2]&0x7F)*0x80 +(Size[3]&0x7F);
};

/* FrameID��־����
 *
 * TEXT�� �������       TENC�� ����            WXXX�� URL����(URL)            TCOP�� ��Ȩ(Copyright)     TOPE�� ԭ������
 * TCOM�� ������        TDAT�� ����            TPE3�� ָ����               TPE2�� �ֶ�                    TPE1�� �������൱��ID3v1��Artist
 * TPE4�� ���루��¼Ա���޸�Ա��            TYER�� ��ID3v1��Year       USLT�� ���                    TSIZ�� ��С
 * TALB�� ר���൱��ID3v1��Album           TIT1�� ����������         TIT2�� �����൱��ID3v1��Title
 * TIT3�� ������        TCON�� ���ɣ�����൱��ID3v1��Genre          AENC�� ��Ƶ���ܼ���            TSSE�� ����ʹ�õ������Ӳ�����ã�
 * TBPM�� ÿ���ӽ����� COMM�� ע���൱��ID3v1��Comment                    TDLY�� �����б�¼            TRCK�� ���죨���ţ��൱��ID3v1��Track
 * TFLT�� �ļ�����       TIME�� ʱ�䡡       TKEY�� ����ؼ���         TLAN�� ����                    TLEN�� ����
 * TMED�� ý������       TOAL�� ԭ��Ƭ��      TOFN�� ԭ�ļ���              TOLY�� ԭ�������         TORY�� ����������
 * TOWM�� �ļ������ߣ����֤�ߣ�            TPOS�� ��Ʒ������         TPUB�� ������               TRDA�� ¼������
 * TRSN�� Intenet��̨����                    TRSO�� Intenet��̨������  UFID�� Ψһ���ļ���ʶ�� ��    TSRC�� ISRC�����ʵı�׼��¼���룩                �� ��
 */
struct ID3v2Frame //��ǩ֡��10���ֽ�
{
    char FrameID[4];        // ��־���շ�����TEXT��TOPE��TDAT....
    unsigned char Size[4];           // ֡��Ĵ�С������������8λ�洢�ģ�FSize = Size[0]*0x100000000 + Size[1]*0x10000 + Size[2]*0x100 + Size[3];

    char Flag[2];           // ��ű�־������10λΪ0�����󲿷ֵ������16λ��Ϊ0�Ϳ����ˣ���ʽ���£�
                            //  a-��ǩ������־������ʱ��Ϊ��֡����
                            //  b-�ļ�������־������ʱ��Ϊ��֡����
                            //  c-ֻ����־������ʱ��Ϊ��֡�����޸ģ�Ŀǰ����û�п�������
                            //  i-ѹ����־������ʱһ���ֽڴ������BCD���ʾ����
                            //  j-���ܱ�־������̫ʵ�ã�
                            //  k-���־������ʱ˵����֡��������ĳ֡��һ�顣
};

struct IDV1Frame
{
    char Header[3];    /* ��ǩͷ������"TAG"������Ϊû�б�ǩ */
    char Title[30];    /* ���� */
    char Artist[30];   /* ���� */
    char Album[30];    /* ר�� */
    char Year[4];      /* ��Ʒ��� */
    char Comment[28];  /* ��ע */
    char reserve;      /* ���� */
    char track;;
    char Genre;        /* ���� */
};

enum enIDV1Type
{
    TITLE,   /* ���� */
    ARTIST,  /* ���� */
    ALBUM,   /* ר�� */
    YEAR,    /* ��Ʒ��� */
    COMMENT, /* ��ע */
    TRACK,   /* ���� */
    GENRE    /* ���� */
};

enum enIDV2Type
{
    TEXT,
    TENC,
    WXXX,
    TCOP,
    TOPE,

    TIT2
};

class IDV2Type
{
public:
    static char strIDV2Type[][5];
    //static char strIDV2Encode[][15];
public:
    IDV2Type(){};
    ~IDV2Type(){};
    static const char *getStrType(enIDV2Type type)
    {
        return strIDV2Type[type];
    };
};

class GetMp3Info
{
    FILE *pf;
    char *oldPath;
    char *err;
    ID3v2Header mID3V2Header;
    IDV1Frame mIDV1Frame;
    int ID3V2_frame_size;
    int encodeType;

public:
    GetMp3Info(char *pfile);
    ~GetMp3Info();
    //�����ⲿ��ȡ����Ϣ�ڴ涼��Ҫ���ⲿ�ͷ�
    //��ȡ��Ӧ�ı�����Ϣ
    char* getTitle(int *pDataLen, enCodeType *encode);

    //��ȡ��Ӧ���ݳ�����Ϣ
    char* getTOPE(){return 0;}

    char *getError();

    static enCodeType GetUnicodeTypeByBeg(char *src, int len);

private:
    char* getInfofromIDV2(enIDV2Type type, int *dataLen);

    char* getInfofromIDV1(enIDV1Type type);

};

#endif

/*
 * LuckDrawPrizeBase.h
 * Description: ����ת��Ʒ���� 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-4-10 9:56:59
 */

#ifndef __LUCKDRAWPRIZEBASE_H__
#define __LUCKDRAWPRIZEBASE_H__

class CParamPool;
class CFileStream;

enum ELuckDrawPrizeType
{
    ELuckDrawPrizeType_Item = 0,    // ��Ʒ��Ʒ 
    ELuckDrawPrizeType_Money,       // ��Ǯ��Ʒ 

    ELuckDrawPrizeType_Max
};

enum ELuckDrawPrizeLevel
{
    ELuckDrawPrizeLevel_White = 0,  // ��ɫ 
    ELuckDrawPrizeLevel_Green,      // ��ɫ 
    ELuckDrawPrizeLevel_Blue,       // ��ɫ 
    ELuckDrawPrizeLevel_Purple,     // ��ɫ 
    ELuckDrawPrizeLevel_Orange,     // ��ɫ 

    ELuckDrawPrizeLevel_Max
};

/**
* CLuckDrawPrizeBase:
* 
*/

class CLuckDrawPrizeBase
{
public:
    virtual ~CLuckDrawPrizeBase() {}

protected:
    CLuckDrawPrizeBase(unsigned char nType);
    CLuckDrawPrizeBase(const CLuckDrawPrizeBase &ref);
    CLuckDrawPrizeBase(unsigned char nType, unsigned char nLevel, unsigned char nSexNeed, bool bAnnounce);

public:
    unsigned char GetType() const { return m_nType; }
    unsigned char GetLevel() const { return m_nLevel; }
    unsigned char GetSex() const { return m_nSexNeed; }
    bool IsAnnounce() const { return m_bAnnounce; }

    void SetLevel(unsigned char nLevel) { m_nLevel = nLevel; }
    void SetSex(unsigned char nSex) { m_nSexNeed = nSex; }
    void SetAnnounce(bool bAnnounce) { m_bAnnounce = bAnnounce; }

public:
    virtual void LoadFromFile(CFileStream &file);

    virtual void doEncodeToClient(CParamPool &IOBuff) const;
    virtual void doDecodeFromClient(CParamPool &IOBuff);

    virtual void doEncodeToServer(CParamPool &IOBuff) const;
    virtual void doDecodeFromServer(CParamPool &IOBuff);

private:
    unsigned char   m_nType;        // ��Ʒ����
    unsigned char   m_nLevel;       // ��Ʒ�ȼ�
    unsigned char   m_nSexNeed;     // �Ա�����
    bool            m_bAnnounce;    // �Ƿ�㲥
};

#endif // __LUCKDRAWPRIZEBASE_H__


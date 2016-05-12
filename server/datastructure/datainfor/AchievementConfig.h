/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\AmuseRoomConfig.h
    author:     

    purpose:    achievement.bytes���������Ӧ�����ݽṹ
*********************************************************************/

#ifndef     __ACHIEVEMENT_H__
#define     __ACHIEVEMENT_H__

#include <string>
#include <list>
#include "../DataStruct_Base.h"

class CFileStream;

// �ɾ���Ʒ����
class AchievementItemReward
{
public:
    AchievementItemReward(itemtype_t nItemID = 0, unsigned short nItemCount = 0);
    ~AchievementItemReward();

public:
    void Read(CFileStream &file);
    void Write(CFileStream  &file);

    bool IsValid() const;
    itemtype_t GetItemID();
    unsigned short GetItemCount();

public:
    itemtype_t          m_nItemID;
    unsigned short      m_nItemCount;
};

//�ɾ��������ݽṹ
class AchievementConfig
{
public:
    AchievementConfig();
    ~AchievementConfig();

public:
    unsigned short      m_nAchievementID;       // �ɾͱ��
    unsigned short      m_nClassID;             // �ɾͷ�����
    unsigned short      m_nSeriesID;            // ϵ�б��
    std::string         m_strAchievementName;   // �ɾ�����
    unsigned char       m_nLevel;               // ���
    unsigned short      m_nTitleID;             // �ɾͳƺ�
    std::string         m_strDetail;            // �ɾͽ���
    std::string         m_strIcon;              // Icon
    unsigned short      m_nHandlerID;           // �ɾʹ�����ID
    unsigned int        m_nCount;               // ������ֵ
    unsigned int        m_nScore;               // �ɾͻ���
    unsigned int        m_nRewardMoney;         // ��Ǯ����
    std::list<AchievementItemReward>   m_listRewardItem;  // �ɾ���Ʒ����
    bool                m_bNotify;              // �Ƿ�ȫ������

public:
    // ��ȡ�ɾ�ID
    unsigned short	GetAchievementID();

    // ��ȡ�ɾ�����
    const char * GetAchievementName() const;

    // ��ȡ�ɾͳƺ�ID
    unsigned short GetRewardTitleID() const;

    // ��ȡ�ɾͽ���
    const char * GetDetail() const;

    // ��ȡ�ɾͻ���
    unsigned int GetScore();

    // ��ȡ��Ǯ����
    unsigned int GetRewardMoney();

    // ��ȡ�ɾʹ�����ID
    unsigned short GetHanlerID();

    // ��ȡ�ɾͷ�����
    unsigned short GetClassID();

    // ��ȡϵ�б��
    unsigned short GetSeriesID();

    // ��ȡ��ɵ���ֵ
    unsigned int GetRequireValue();

    // ��ȡ�ɾͽ�����Ʒ
    std::list<AchievementItemReward> & GetRewardItemList();

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);
};

#endif



//end file



/** Counter.h
* Description: new member ceremony room class.
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: liguanghao	
* Create time: 2015-08-10 11:48:00
*/

#ifndef __CEREMONYROOMHEAD_H__
#define __CEREMONYROOMHEAD_H__

#include <map>
#include <vector>
#include <string>


#define GROUPDANCE_CEREMONY_MAX_INTRODUCE				1024

class CParamPool;

enum ECeremonyErrType
{
    ECeremonyErrType_OK = 0,				// �޴���
    ECeremonyErrType_RightNotEnough,		// ��Ȩִ�в���
    ECeremonyErrType_ResNotEnough,			// ��Դ����
    ECeremonyErrType_NotMember,				// �������ų�Ա
    ECeremonyErrType_OnApply,				// ����������ʽ�ٰ�
    ECeremonyErrType_Running,				// ��ʽ���ڽ���
    ECeremonyErrType_Internal,				// ����������
    ECeremonyErrType_NotOpen,				// ����δ����
    ECeremonyErrType_StateErr,				// ����״̬����
    ECeremonyErrType_NotMengxin,			// ����С����
    ECeremonyErrType_SequenceRedumplicate,	// ����˳���ظ�
    ECeremonyErrType_SequenceError,         // ����˳�򲻴���
    ECeremonyErrType_NotNewMan,             // δָ��Ϊ����
    ECeremonyErrType_NoNewMan,              // û������
    ECeremonyErrType_NotInRoom,             // ���ڷ�����
    ECeremonyErrType_SequenceErr,           // ���˱�Ŵ���
    ECeremonyErrType_InOtherRoom,           // ������������
    ECeremonyErrType_CreatorNotInRoom,      // �������ڷ�����
    ECeremonyErrType_CreatorIsKicked,       // �����˱��������
    ECeremonyErrType_GroupDismissed,        // �����Ѿ������ɢ
    ECeremonyErrType_PraiseCD,              // ������ʽ������ȴʱ��
};


enum ECeremonyPlayerType
{
    ECeremonyPlayerType_None,			//��ͨ����
    ECeremonyPlayerType_Applicant,		//��ʽ������
    ECeremonyPlayerType_Rookie,			//��ʽ����
};


enum ECeremonyRoomQuitType
{
    ECeremonyRoomQuitType_Quit = 0,
    ECeremonyRoomQuitType_Kicked,
    ECeremonyRoomQuitType_RoomClose,
    ECeremonyRoomQuitType_ExitDanceGroup,
    ECeremonyRoomQuitType_KickedFromDanceGroup,
};

enum ECeremonyRoomQuitTo
{
    ECeremonyRoomQuitTo_World = 0,
    ECeremonyRoomQuitTo_Dungeon,
};

enum ECeremonyState
{
	ECeremonyState_None,		//δ������ʽ
	ECeremonyState_Prepare,	//��ʽ������-׼����ʱ�� �ȴ�����д���ҽ���
	ECeremonyState_Start,		//��ʽ�ѿ�ʼ-���Ŷ��� ����ʱ��
};


enum ECeremonyRoomCloseReason
{
	ECeremonyRoomCloseReason_ByCreator = 0,		// �����߹ر�
	ECeremonyRoomCloseReason_TimeEnd,			// ʱ�����
	ECeremonyRoomCloseReason_Empty,				// ����û����
};

enum ECeremonyNewMemberSeq
{
    ECeremonyNewMemberSeq_Min                   = 1,    // ��ʽ���˵�������Сֵ 
    ECeremonyNewMemberSeq_Max                   = 8     // ��ʽ������������ֵ
};


// ������ʽ��־���俪����Ϣ
class CeremonyLogRoomInfo
{
public:
    CeremonyLogRoomInfo();
    ~CeremonyLogRoomInfo();

    void DoDecode(CParamPool &IOBuff);
    void DoEncode(CParamPool &IOBuff);

    unsigned int m_nLogID;                 // ��ĳһ�εķ����Ӧ
    unsigned int m_nCreatorID;
    std::string m_strCreatorName;
    unsigned char m_nCreatorVip;
    unsigned int m_logTime;                 
};

// ������ʽ��־������Ϣ���ݿ��Ӧ��ϵ
class CeremonyLogInfoDb
{
public:
    unsigned int m_nGroupID;
    CeremonyLogRoomInfo m_info;
};

// ������ʽ��־������Ϣ
class CeremonyLogNewerInfo
{
public:
    CeremonyLogNewerInfo();
    ~CeremonyLogNewerInfo();

    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

// private:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    unsigned char m_nVipLv;
};

// ������ʽ��־ͶƱ�˵���ͶƱ��Ϣ
class CeremonyLogVoterInfo
{
public:
    CeremonyLogVoterInfo();
    ~CeremonyLogVoterInfo();

    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);


    void Set(unsigned int nPraiserID, std::string strName, unsigned char vipLev, unsigned short count, unsigned int now);
    unsigned int m_nPraiserID;                  // �����˵�ID
    std::string m_strPraiserName;               // �����˵�����
    unsigned char m_nPraiserVipLv;              // �����˵�vip�ȼ�(��ʱ��)
    unsigned short m_nPraiserCount;             // ���޵ĸ���
    unsigned int m_logTime;                     // 
};

// ������ʽ��־���η��俪���У�ĳ�����˱�ͶƱ��Ϣ����
class CeremonyPraiseLogInfo
{
public:
    CeremonyPraiseLogInfo();
    ~CeremonyPraiseLogInfo();

    // ��������£��������
    void AddVoterInfo(CeremonyLogVoterInfo &voterInfo);

    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

    CeremonyLogNewerInfo m_newerInfo;     // 
    std::vector<CeremonyLogVoterInfo> m_vecVoterInfo; // 
};

// ������ʽ��־ ����ͶƱ��Ϣ���ݿ��¼
class CeremonyPraiseLogInfoDb
{
public:
    unsigned int m_nLogID;                      // �������Ӧ
    CeremonyLogNewerInfo m_newerInfo;     // 
    CeremonyLogVoterInfo m_voterInfo;     //     
};

// ����������֯��Log������ĳ����ʽ���������˵ı�ͶƱ��¼
class DanceGroupCeremonyLog
{
public:
    DanceGroupCeremonyLog();
    ~DanceGroupCeremonyLog();

    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

    CeremonyLogRoomInfo m_roomBaseLog;             // 
    std::vector<CeremonyPraiseLogInfo> m_voteInfo; // 
};

// ĳ�����ţ����еļ�¼
class DanceGroupCeremonyLogs
{
public:
    DanceGroupCeremonyLogs();
    ~DanceGroupCeremonyLogs();

    unsigned int m_nGroupID;
    typedef std::vector<DanceGroupCeremonyLog> DanceGroupCeremonyLogVec;
    // ���µ�����ǰ��
    DanceGroupCeremonyLogVec m_vecLog; 
};
// <dancegroupID, logs>
typedef std::map<unsigned int, DanceGroupCeremonyLogs> DanceGroupCeremonyLogMap;

class DanceGroupCeremonyLogsProcessor
{
public:
    DanceGroupCeremonyLogsProcessor();
    ~DanceGroupCeremonyLogsProcessor();

    // ��һ����־��ӵ�����map��
    static void InsertCeremonyLog(unsigned int groupID, DanceGroupCeremonyLog &log, 
        std::map<unsigned, DanceGroupCeremonyLogs> &logMap); 
    // ��������ʽ��־������һ��ͶƱ��Ϣ
    static void InsertCeremonyPraiseItem(DanceGroupCeremonyLog &roomLog, CeremonyPraiseLogInfoDb &info); 

};

// ����
class CeremonyNewMember
{
public:
    CeremonyNewMember();
    ~CeremonyNewMember();

    unsigned int m_nRoleID;			// player id.
    unsigned char m_nSeq;			// ��������
    unsigned short m_nPraisedNum;	// �����޴���

    CeremonyPraiseLogInfo m_logs;   // ĳ���˱����޵ļ�¼
};

// ����������
class CeremonyNormalMember
{
public:
    CeremonyNormalMember();
    ~CeremonyNormalMember();

    unsigned int m_nRoleID;
    unsigned short m_nPraiseNum;		// �Ѿ�ʹ�õĵ��޴��������Σ�
};




#endif // __CEREMONYROOMHEAD_H__






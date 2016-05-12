/** Counter.h
* Description: new member ceremony room class.
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: liguanghao	
* Create time: 2015-08-10 11:48:00
*/

#ifndef __CEREMONYROOM_H__
#define __CEREMONYROOM_H__

#include <map>
#include <string>
#include "ceremonyRoomHead.h"
#include "GameMsg_CeremonyRoom.h"
#include "GameMsg_CeremonyRoomSC.h"


class CRoleEntity;

// ��ʽ�г�Ա��Ϣ
// ��־ͳ�Ƶ�ʱ�򣬳�Ա�����Ѿ������ߣ������Ҫ����ʽ������һ�̣������г�Ա����Ϣ����������
// ����Ҫ��ʱ��ֱ�Ӵ�����ȡ
class CeremonyMemberLogInfo
{
public:
    unsigned int m_nRoleID;
    std::string m_strName;
    unsigned char m_nVipLev;
};

class CeremonyRoom
{
public:
	CeremonyRoom(unsigned int danceGroupID, CRoleEntity *pRole);
	~CeremonyRoom();

    bool IsCreator(unsigned nRoleID);
    bool IsNewMember(unsigned nRoleID);
    bool IsNormalMember(unsigned nRoleID);
    bool IsNewMemberAtStart(unsigned nRoleID);
    bool IsNormalMemberAtStart(unsigned nRoleID);
    // �����Ƿ��Ѿ������ˡ�
    bool IsRoomStart();

	// �����߲���
	int SetNewMember(unsigned int nRoleID, int seq, bool bNeedMengxin);
	int CancelNewMember(unsigned int nRoleID); // ȡ��ָ��Ϊ����

	int KickPerson(unsigned int nRoleID); // �ӷ�������

	int Play(std::vector<CeremonyRookieInfo> &vecRookieInfo); // �ͻ��˿�ʼ���ţ������������

	// 
	int EnterRoom(unsigned int nRoleID, CeremonyRoomInfo& roomInfo, CeremonyPlayerShowInfo &selfInfo); // ���뷿��
	int LeaveRoom(unsigned int nRoleID); // �뿪����

	int SaveIntroduce(unsigned int nRoleID, std::string strIntroduce); // д���ҽ�����

	unsigned char APraiseB(unsigned int nRoleA, unsigned int nRoleNewer, unsigned char nNewerIndex, int count, int &nNewCount); // A��B����

	void GetRoomInfo(CeremonyRoomInfo& roomInfo); // ȡ�÷�����Ϣ
	bool GetRoomPlayerShowInfo(unsigned int nRoleID, CeremonyPlayerShowInfo &info);

	// �㲥�����ˣ�����һ����
	void RoomBroadcastExcept(unsigned int nRoleID, GameMsg_Base &msg);
    // �㲥������
    void RoomBroadcast(GameMsg_Base &msg);

    unsigned int GetCreator(); // ȡ����ʽ������
    
    bool HasMember(unsigned int nRoleID);

    unsigned int GetApplyTime();
    unsigned int GetStartTime();
    ECeremonyState GetState();

    // �ύ������־
    void ComitRoomLog();
    // �����ʼ�����
    void MailRoomPrize();

    void CloseRoom();

protected:
    void FromNormalMemberToNewMember(unsigned int nRoleID, unsigned seq);
    void FromNewMemberToNormalMember(unsigned int nRoleID);

    void AllMember(std::vector<unsigned int> &allMember);

    
    CeremonyNormalMember* GetNormalMember(unsigned int nRoleID);
    CeremonyNewMember* GetNewerMember(unsigned int nRoleID);
    CeremonyNormalMember* GetNormalMemberAtStart(unsigned int nRoleID);
    CeremonyNewMember* GetNewerMemberAtStart(unsigned int nRoleID);
    CeremonyMemberLogInfo *GetMemberLogInfo(unsigned int nRoleID);

    std::string GetIntroduce(unsigned int roleID);
    void SetIntroduct(unsigned int roleID, std::string strIntroduce);

protected:
    typedef std::map<unsigned int, CeremonyNormalMember> CeremonyNormalMemberMap;
    typedef std::map<unsigned int, CeremonyNewMember> CeremonyNewMemberMap; // 
    typedef std::map<unsigned int, CeremonyMemberLogInfo> CeremonyMemberLogInfoMap;
    typedef std::map<unsigned int, std::string> StringMap;
	unsigned int m_nGroupID;						// ��������ID
	unsigned int m_nCreater;						// ����Ĵ�����
    CeremonyLogRoomInfo m_logRoomInfo;              // ����Ĵ�����Ϣ
    
    CeremonyNormalMemberMap m_mapNormalMember;	    // �����ˣ������ߣ����ڣ����ˣ�
	CeremonyNewMemberMap m_mapNewMember;            // ��������
    CeremonyMemberLogInfoMap m_mapMemberLogInfo;    // ��־��صĸ�����Ϣ
    StringMap m_mapIntroduce;                       // �������ҽ���

    // ��ʽ����֮�󣬲����߼�ʹ�˳�����Ҳ�ǿ��Ի�ý����������Ҫ������Щ�ˣ�����������
    CeremonyNormalMemberMap m_mapNormalMemberAtStart; // ��ʽ����ʱ��������
    CeremonyNewMemberMap m_mapNewMemberAtStart;   // ��ʽ����ʱ�������

	ECeremonyState m_crsState;		    			// ����״̬

	// ������ʱ����Ҫ������ʱ
	unsigned int m_nApplyTime;						// ���俪��ʱ�� ������ʱһ��Сʱ��������ʱ����֮��ǿ�ƿ�����
	unsigned int m_nStartTime;						// ����������֮���ʱ�� ������ʱ����֮�󣬷���ǿ�ƹرգ�

};


struct PraiseSort
{
    unsigned int nRoleID;
    int nSeq;
    int nPraise;
    static bool Great(const PraiseSort& s1, const PraiseSort &s2);
};

#endif // __CEREMONYROOM_H__






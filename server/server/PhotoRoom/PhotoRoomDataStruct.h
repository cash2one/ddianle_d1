#ifndef		__PHOTOROOMDATASTRUCT_H__
#define		__PHOTOROOMDATASTRUCT_H__

#include <string>
#include <list>
#include "PhotoRoomMacroDefine.h"

class CParamPool;
class CEntityItemComponent;
class CEntityGeneComponent;
class PetComponent;
class CEntityMedalComponent;

class CPhotoRoomPlayerInfo
{
public:
    CPhotoRoomPlayerInfo();
    ~CPhotoRoomPlayerInfo();

    void doEncode(CParamPool & IOBuff);

public:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    unsigned char m_eRoleSex;
    char m_nRoleSkin;
    std::string m_strDanceGroupName;
    unsigned char m_nGroupTitle;
    unsigned short m_nDanceGroupBadge;
    unsigned short m_nDanceGroupEffect;
    bool m_bIsHost;
    EPhotoRoomPlayerState m_ePlayerState;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    unsigned short m_nTransFormID;

    CEntityItemComponent* m_pRoleItem;
    CEntityGeneComponent* m_pRoleGene;
    PetComponent*         m_pRolePet;
    CEntityMedalComponent   *m_pRoleMedal;
};

class CPhotoRoomNPCInfo
{
public:
	CPhotoRoomNPCInfo();
	~CPhotoRoomNPCInfo();
public:
	void doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nSexType;
	unsigned char m_nSkin;
	std::string m_strName;
	std::string m_strHeadwear;
	std::string m_strHair;
	std::string m_strFace;
	std::string m_strShoulders;
	std::string m_strGloves;
	std::string m_strRighthand;
	std::string m_strLefthand;
	std::string m_strCloth;
	std::string m_strShoes;
	std::string m_strSuit;
	std::string m_strLeg;
};

///////////////////////////////////////////////////////
class CPhotoRoomWholeInfo
{
public:
	CPhotoRoomWholeInfo();
	~CPhotoRoomWholeInfo();
public:
	void doEncode(CParamPool & IOBufff);
public:
	unsigned int m_nRoomID;
	std::string m_strRoomName;
	unsigned int m_nRoomType;
	int m_nPhotoRoomTimeRemaind;
	unsigned short m_nSelectedBGID;
	std::list<unsigned short> m_listOwnedBG;
	std::list<unsigned short> m_listOwnedGroupAction;

	std::list<CPhotoRoomPlayerInfo> m_listPlayers;
	std::list<CPhotoRoomNPCInfo> m_listNPCs;
};

class CPhotoRoomNotifiationTimer
{
public:
	CPhotoRoomNotifiationTimer();	
	~CPhotoRoomNotifiationTimer();
public:
	int m_nLevel;
	int m_nTimeBeforeExpired;
	int m_nNotifcationType;
};

//-----------------------CInvitPhotoRoomInfo
class CInvitPhotoRoomInfo
{
public:
	CInvitPhotoRoomInfo(unsigned int nRoomID, int nRoomCreatedTime, int nTimeRemain);
	~CInvitPhotoRoomInfo();
public:
	unsigned int m_nRoomID;
	int m_nRoomCreatedTime;		//单位:s
	int m_nTimeRemain;			//单位:ms
};




#endif		




#ifndef __AMUSEROOM_DATAMGR_H__
#define __AMUSEROOM_DATAMGR_H__

#include <string>
#include <map>
#include <vector>
#include <set>
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../share/ServerDataStruct.h"

///////////////////////////////////////////////////////
class CAmuseDeviceData
{
public:
	CAmuseDeviceData();
	~CAmuseDeviceData();

public:
	int m_nDeviceID;

	int m_nCapacity;
	int m_nInviteType;
};


///////////////////////////////////////////////////////
class CAmuseCarrouselData
{
public:
	CAmuseCarrouselData();
	~CAmuseCarrouselData();

	std::vector<std::string>& GetStartSongs();

public:
	bool m_bIsLoop;
	int m_nInterval;

	std::vector<std::string> m_vecStartSong;
};


///////////////////////////////////////////////////////
class CAmuseExpData
{
public:
	CAmuseExpData();
	~CAmuseExpData();

public:
	bool m_bIsLoop;
	int m_nInterval;
	int m_nAddExp;
};


///////////////////////////////////////////////////////
class CAmuseSwingData
{
public:
	CAmuseSwingData();
	~CAmuseSwingData();

public:
	int m_nDeviceID;
	int m_nPushTime;
	int m_nSwingTime;
};


///////////////////////////////////////////////////////
class CAmuseBoatData
{
public:
	CAmuseBoatData();
	~CAmuseBoatData();

public:
	bool m_bIsLoop;
	int m_nInterval;
	int m_nBoatDock;
};

///////////////////////////////////////////////////////
typedef std::map<int, CAmuseDeviceData> AmuseDeviceDataMap;
typedef std::map<int, AmuseDeviceDataMap> AmuseSceneDeviceMap;
typedef std::map<int, CAmuseCarrouselData> AmuseCarrouselDataMap;
typedef std::map<int, CAmuseExpData> AmuseExpDataMap;
typedef std::map<int, CAmuseSwingData> AmuseSwingDataMap;
typedef std::map<int, CAmuseBoatData> AmuseBoatDataMap;
typedef std::map<int, std::set<int> > AmuseEggDataMap;

class CAmuseRoomDataManager : public GameMsg_Processor
{
protected:
	CAmuseRoomDataManager();
	~CAmuseRoomDataManager();

public:
	static CAmuseRoomDataManager& Instance();

	void Init();

	void AddDeviceData(int nSceneID, int nDeviceID, int nCapacity, int nInviteType);
	AmuseDeviceDataMap& GetDeviceData(int nSceneID);

	void AddCarrouselData(int nSceneID, bool bIsLoop, int nInterval, std::vector<std::string>& vecStartSong);
	bool GetCarrouselData(int nSceneID, CAmuseCarrouselData& rCarrouselData);

	void AddExpData(int nSceneID, bool bIsLoop, int nInterval, int nAddExp);
	bool GetExpData(int nSceneID, CAmuseExpData& rExpData);

	void AddSwingData(int nSceneID, int nDeviceID, int nPushTime, int nSwingTime);
	bool GetSwingData(int nSceneID, CAmuseSwingData& rSwingData);

	void AddBoatData(int nSceneID, bool bIsLoop, int nInterval, int nBoatDock);
	bool GetBoatData(int nSceneID, CAmuseBoatData& rBoatData);

	void AddEggData(int nSceneID, int nDeviceID);
	bool GetEggData(int nSceneID, int nDeviceID);
	bool GetEggData(int nSceneID, std::set<int>& rEggData);

	bool ParseSetting(unsigned int nTimeEggOpenDuration, unsigned int nTimeEggDestroyDuration, 
		const string& strCostItem, const string& strGiftItem, 
		const string& strTogetherMaleItem, const string& strTogetherFemaleItem, const string& strMaleItem, const string& strFemaleItem, 
		int nFixedMoneyReward, int nExtraMoneyReward, int nGiftDiscount, const string& strHelpTitle, const string& strHelpContent);

	TimeEggSetting& GetTimeEggSetting(){return m_timeEggSetting;}

	void OnGroupServerConnected();

private:
	void _CheckMallItem(std::list<CItem>& listItem);

    void RegNetMsgMap();

    void OnDBOpenTimeEgg( QueryBase &rQuery );

    void OpenTimeEggSuccess( OpenTimeEggEntry *pInfo ) const;
    void OpenTimeEggFailed( const OpenTimeEggEntry *pInfo ) const;

protected:
	AmuseSceneDeviceMap m_mapDeviceData;
	AmuseCarrouselDataMap m_mapCarrouselData;
	AmuseExpDataMap m_mapExpData;
	AmuseSwingDataMap m_mapSwingData;
	AmuseBoatDataMap m_mapBoatData;
	AmuseEggDataMap m_mapEggData;
	TimeEggSetting m_timeEggSetting;
	bool m_bSentTimeEggSetting;
	bool m_bInited;
	bool m_bConnectedGroup;
};

#endif

//file end


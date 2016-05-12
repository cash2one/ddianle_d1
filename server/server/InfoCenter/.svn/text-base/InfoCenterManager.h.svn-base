#ifndef _INFOCENTER_MANAGER_H_
#define _INFOCENTER_MANAGER_H_

#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_DB.h"
#include <deque>

struct InfoCenterItem;

class InfoCenterManager : public GameMsg_Processor
{
protected:
	InfoCenterManager();
	~InfoCenterManager();

public:
	static InfoCenterManager & Instance();

public:
    void Initialize();
    void RegMessageMap();

private:
	void LoadDbInfoAll(); // 启动时，加载数据库

private:
	void OnGMGetInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMAddInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMUpdateInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMRemoveInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);

	// 加载数据库信息回调
	void OnLoadAllInfo(QueryBase &query);
	void OnAddInfo(QueryBase &query);
private:
    
	// 该数据在启动的时候从数据库加载，然后同步到gameServer中去
	// 之后，该数据的更新将会传递到gameserver和数据库中区
	typedef std::map<unsigned int, InfoCenterItem*> InfoCenterPtrMap;
	InfoCenterPtrMap m_infos; 


};

#endif //_INFOCENTER_MANAGER_H_


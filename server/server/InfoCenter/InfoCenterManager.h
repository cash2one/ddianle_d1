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
	void LoadDbInfoAll(); // ����ʱ���������ݿ�

private:
	void OnGMGetInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMAddInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMUpdateInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMRemoveInfoCenterRes(GameMsg_Base & msg, CSlotPeer & slotPeer);

	// �������ݿ���Ϣ�ص�
	void OnLoadAllInfo(QueryBase &query);
	void OnAddInfo(QueryBase &query);
private:
    
	// ��������������ʱ������ݿ���أ�Ȼ��ͬ����gameServer��ȥ
	// ֮�󣬸����ݵĸ��½��ᴫ�ݵ�gameserver�����ݿ�����
	typedef std::map<unsigned int, InfoCenterItem*> InfoCenterPtrMap;
	InfoCenterPtrMap m_infos; 


};

#endif //_INFOCENTER_MANAGER_H_


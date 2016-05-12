#ifndef	__ENTITY_ANTI_ADDICTION_COMPONENT_H__
#define __ENTITY_ANTI_ADDICTION_COMPONENT_H__
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include <queue>
#include <string>
#include "AntiAddictionDataMgr.h"
#include "../../datastructure/LogStructDef.h"
#include "../../datastructure/Macro_Define.h"
using namespace std;
class CEntityTimerComponent;
class CEntityNetComponent;
class GameMsg_Base;

class CEntityAntiAddictonComponent :
	public CEntityComponent
{

public:
	CEntityAntiAddictonComponent ();
	~CEntityAntiAddictonComponent ();

public:
	virtual void  Start();
	virtual void  SerializeComponent(CParamPool &IOBuff);
	virtual const char* SerializeName(){ return "AntiAddicton"; };
	virtual void  OnEvent(CComponentEvent& refEvent);
	virtual bool  CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
	virtual bool  PacketToDB(ROLEINFO_DB *pRoleInforDB) const;
	virtual void  OnUpdate(const unsigned long &nTimeElapsed);
	virtual void  OnLogin();
	virtual void  OnLogout();
	virtual void  SendExtraStaticData();
	virtual void  PacketBuff(std::list<BuffData>& listBuff){}
public:
	void RegComponentNetMsgMap();
	void OnSynAdult( GameMsg_Base & pMsg ,CSlotPeer & SlotPeer );
	bool IsAdult() const;
	void SetAdult(bool val);
	bool IsActive() const;
	bool CheckActive();//检查组件是否应当激活
	bool CreateNextTimer(unsigned int nNowTime, unsigned int nOnlineTime);
	//ReCalcEffectData返回值表示是否对data造成了影响
	template<typename T>
	bool ReCalcEffectData(eAddicEffect type, T& data);
	int GetAddicEffectByExpCause(CRoleExpLog::EAddExpCause eCause);
	int GetAddicEffectByMoneyCause(EChangeMoneyCause eCause);
private:
	AntiAddictionConfigItem* GetConfigItem();
	void CreateNextTipTimer(unsigned int nNowTime);
	void SendTipMsg(const string& strText);
	void SendPlayerMsg(GameMsg_Base *pMsg);
private:
	CEntityNetComponent * m_pRoleNet;//网络组件指针
	CEntityTimerComponent* m_pRoleTimer;//计时器组件指针
	AntiAddictionConfigItem* m_pCurConfigItem;//配置项
	int m_nConfigItemIndex;//配置项索引
	unsigned short m_nPID;//平台id
	unsigned int m_nAddictionTime;//累计沉迷时间，秒
	unsigned int m_nAddictionOfflineTime;//下线累计时间，秒
	unsigned int m_nTipCount;//提示次数
	bool m_bAdult;//是否成年
	bool m_bActive;//此组件是否处于激活状态
	bool m_bAddictionLogin;//登陆时是否处于沉迷状态
};

typedef CRoleComponentImpl<CEntityAntiAddictonComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleAntiAddiction;

template<typename T>
bool CEntityAntiAddictonComponent::ReCalcEffectData(eAddicEffect type, T& data)
{
	if (type < eAddicEffect_Max && 
		m_pCurConfigItem != NULL && 
		m_pCurConfigItem->decrPercent[type] > 0.0f)
	{
		data = (T)(data * (1.0f-m_pCurConfigItem->decrPercent[type]));
		return true;
	}
	else
	{
		return false;
	}
}

#endif

//end file


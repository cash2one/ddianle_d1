#ifndef __COUPLE_PROCESSOR_H__
#define __COUPLE_PROCESSOR_H__

#include <map>

#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/DataStruct_Couple.h"
#include "../group/Global.h"


#include "CoupleManager.h"
class CBlessingValueRankMgr;

class CCoupleProcessor :
	public GameMsg_Processor
{
public :
	CCoupleProcessor();	
	virtual ~CCoupleProcessor();

	static CCoupleProcessor & Instance();

public :
	// 初始化
	void Initialize();

private:
	void _RegisterMessage();
	void _OnAcceptCoupleRequest(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnCoupleSeparated(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnGetMarried(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnDivorced(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnWeddingRoomStartNofication(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnAutoStartWeddingRoomFail(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnUpdateCoupleIntimacy(GameMsg_Base & msg, CSlotPeer & slotPeer);

public :
	void OnPlayerLogin(CGroupRoleEntity & role);

    void OtLoadRankInfoSuc( QueryBase &pQuery );
	void OnLoadOfflineCoupleInfoSuc( QueryBase &pQuery );

	// 获取婚礼记录
	void OnGetWeddingRecordList( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 添加结婚记录
	void OnAddWeddingRecord( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 更新结婚记录是否进行中标示
	void OnUpdateWeddingRecordRunningState( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 获取祝福值排行列表
	void OnGetBlessingRankList( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 更新祝福值
	void OnUpdateWeddingRecord( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 祝福夫妇
	void OnBlessingCouple( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 红包
	void OnRedEnvelope( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 获取coupleInfo
	void OnGetCoupleInfo( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 获取记录里的祝福值
	void OnGetBlessingValue( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 获取角色离线形象信息
	void OnGetPersonInfo( GameMsg_Base & msg, CSlotPeer & slotPeer );

	// 获取离婚动画离线形象
	void OnGetPersonInfoFromDivorce( GameMsg_Base & msg, CSlotPeer & slotPeer );

	void OnGetOnlinePersonInfo( GameMsg_Base & msg, CSlotPeer & slotPeer ); 

	void OnWeddingNotice( GameMsg_Base & msg, CSlotPeer & slotPeer );

	void OnAddRedEnvelopeBlessingValue(GameMsg_Base & msg, CSlotPeer & slotPeer);

	// 离婚行为通知
	void OnRefreshDivorceAction(GameMsg_Base & msg, CSlotPeer & slotPeer);

	// 修改昵称同步
	void SyncChangeNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickname );

private :
	// 更新信息到排行榜
	void OnUpdateBlessingRank( GameMsg_Base &msg, CSlotPeer & slotPeer );

	// 获取排行榜信息
	void OnGetBlessingRankInfo( GameMsg_Base &msg, CSlotPeer & slotPeer );

	void AddOfflineCoupleInfo( const COfflineCoupleInfo & coupleinfo );
	void DeleteOfflineCoupleInfo( unsigned int nPairID );

	void UpdateCoupleInfoPlayerNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickName );
	void UpdateCoupleInfoMarriageData( unsigned int nPairID, unsigned int nMarriageData );
	void UpdateCoupleInfoIntimacy( unsigned int nPairID, unsigned int nIntimacy );
	void UpdateCoupleInfoBlessingValue( unsigned int nPairID, unsigned int nBlessingValue );
private :
    CBlessingValueRankMgr             m_BlessingValueRankMgr;
	CWeddingRecord                    m_WeddingRecord;
	std::map<unsigned int,COfflineCoupleInfo> m_OfflineCoupleInfoTable;
	bool	m_bLoadOfflineCoupleInfoSuc;

};




#endif


#include "InitLua.h"
//#include "../../liblog/LogType.h"
#include "../logic/PlayerManager.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/ItemErrorInfo.h"
#include "../../datastructure/RoleEntity.h"
#include "../item/EntityItemComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../room/RoomComponent.h"
#include "../room/RoomManager.h"
#include "../Dungeon/EntityDungeonComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"

int  lua_ModifyRoleMoneyGain( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1) && lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	int nValue = (int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}
	CEntityAttributeComponent *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
	if (pRoleAttr != NULL){
		pRoleAttr->ModifyMoneyGain( nValue );
	}
	return 0;
}

int  lua_ModifyRoleExpGain( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1) && lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	int nValue = (int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}
	CEntityAttributeComponent *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
	if (pRoleAttr != NULL){
		pRoleAttr->ModifyExpGain( nValue );
	}
	return 0;
}

int  lua_AddRingIntimacy( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 3 && lua_isnumber(L,1) && lua_isnumber(L,2) && lua_isnumber(L,3));
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

// 	int iEntity = (int)lua_tonumber( L , 1 );
// 	int nValue = (int)lua_tonumber( L , 2 );
// 	unsigned short nItemID = (unsigned short)lua_tonumber(L ,3);
// 	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
// 	if( pRole == NULL )
// 	{
// 		PARAM_ERROR;
// 		return 0;
// 	}
// 	
// 	CEntityCoupleComponent *pRoleCouple = pRole->GetComponent<CRoleCouple>();
// 	if (pRoleCouple != NULL)
// 	{
// 		//pRoleCouple->AddWeddingRingGeneEffect( nValue, nItemID );
// 	}

	return 0;
}

int  lua_DelRingIntimacy( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 3 && lua_isnumber(L,1) && lua_isnumber(L,2) && lua_isnumber(L,3));
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int iEntity = (int)lua_tonumber( L , 1 );
	//int nValue = (int)lua_tonumber( L , 2 );
	//int nItemID = (int)lua_tonumber(L ,3);
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( pRole == NULL )
	{
		PARAM_ERROR;
		return 0;
	}

	CEntityCoupleComponent *pRoleCouple = pRole->GetComponent<CRoleCouple>();
	if (pRoleCouple != NULL)
	{
		//pRoleCouple->DelWeddingRingGeneEffect();
	}

	return 0;
}

int  lua_ModifyRoleHotGain( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1) && lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	int nValue = (int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}
	CEntityAttributeComponent *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
	if (pRoleAttr != NULL){
		pRoleAttr->ModifyPreGain( nValue );
	}
	return 0;
}

int  lua_ModifySetIgnoreMis( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1) && lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	int nValue = (int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}
	CEntityAttributeComponent *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
	if (pRoleAttr != NULL){
		pRoleAttr->ModifyIgnoreMis( nValue );
	}
	return 0;
}

int  lua_ModifySetVip( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 3 && lua_isnumber(L,1) && lua_isnumber(L,2) && lua_isnumber(L,3) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	unsigned int nAddExp = (unsigned int)lua_tonumber( L , 2 );
	unsigned int nAddTime = (unsigned int)lua_tonumber(L,3);
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}

	CEntityVIPComponent *pRoleVIP = pRole->GetComponent<CRoleVIP>();	
	if (pRoleVIP != NULL)
	{
		pRoleVIP->OnUpdateVIPInfo(nAddExp, nAddTime);
	}

	return 0;
}

int  lua_FindRoleGene( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1) && lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int iEntity = (int)lua_tonumber( L , 1 );
	unsigned short Geneid = (unsigned short)lua_tonumber( L , 2 );

	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole ){
		PARAM_ERROR;
		return 0;
	}

	CEntityGeneComponent *pRoleGene = pRole->GetComponent<CRoleGene>();
	if (pRoleGene != NULL){
		GeneData * pGene = pRoleGene->FindGene( Geneid );
		if( pGene )
		{
			lua_pushnumber(L,pGene->nGeneId);
			lua_pushnumber(L,pGene->nParam1);
			lua_pushnumber(L,pGene->nParam2);
		}
		else
		{
			lua_pushnil(L);
			lua_pushnil(L);
			lua_pushnil(L);
		}
	}
	else{
		PARAM_ERROR;
		return 0;
	}
	return 3;
}

int  lua_RoleAddGene( lua_State * L )
{
	bool bRresult = false;
	bool bValid  =  ( lua_gettop( L ) == 6 && lua_isnumber(L,1) &&lua_isnumber(L,2) && lua_isnumber(L,3) 
		&& lua_isnumber(L,4) && lua_isnumber(L,5) &&lua_isstring(L,6) );
	if( !bValid ){
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	unsigned short geneid = (unsigned short)lua_tonumber( L , 2 );
	//unsigned char ntype = (unsigned char)lua_tonumber( L , 3 );
	int param1 = (int)lua_tonumber( L , 4 );;
	int param2 = (int)lua_tonumber( L , 5 );
	const char *szparam = lua_tostring(L,6);

	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( pRole )
	{
		CEntityGeneComponent *pRoleGene = pRole->GetComponent<CRoleGene>();
		if (pRoleGene != NULL){
			bRresult = pRoleGene->AddGene(geneid,param1,param2,szparam);
		}
	}
	else{
		PARAM_ERROR;
	}
	lua_pushboolean(L,bRresult);
	return 1;
}

int lua_RoleSetCurChatColor(  lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1)&&lua_isnumber(L,2));
	if( !bValid ){
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	unsigned short nColor = (unsigned short)lua_tonumber(L,2);
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}

	CEntityChatComponent *pChat = pRole->GetComponent<CRoleChat>();	
	if (pChat != NULL) {
		pChat->SetCurFontColor(nColor);
	}

	return 0;
}

int lua_ModifyTitle(  lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 3 && lua_isnumber(L,1)&&lua_isnumber(L,2)&&lua_isnumber(L,3) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	//unsigned char  bIsAdd = (unsigned char)lua_tonumber(L,2);
	//unsigned short nTitleID = (unsigned short)lua_tonumber(L,2);

	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}

	//pRole->SetChatColor( nColor1,nColor2 );
	return 0;
}

int lua_AddItemToRole(  lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 5 && lua_isnumber(L,1) && lua_isnumber(L,2) && lua_isnumber(L,3) && lua_isnumber(L,4) && lua_isnumber(L, 5));
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int iEntity = (int)lua_tonumber( L , 1 );
	itemtype_t nItemType = (itemtype_t)lua_tonumber( L , 2 );
	unsigned short nCount = (unsigned short)lua_tonumber( L , 3 );
	int nValidTime = (int)lua_tonumber( L , 4 );
	unsigned short nPacketID = (unsigned short)lua_tonumber(L, 5);

	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole )
    {
		WriteLog(LOGLEVEL_ERROR, "[%s]无法找到INDEX为%d的玩家\n", __FUNCTION__, iEntity);
		return 0;
	}

	CEntityItemComponent *pRoleItem = pRole->GetComponent<CRoleItem>();
	if (pRoleItem == NULL)
    {
		WriteLog(LOGLEVEL_ERROR, "[%s]无法找到INDEX为%d的CRoleComponent玩家\n", __FUNCTION__, iEntity);
		return 0;
	}

	if( EItemError_Success == pRoleItem->AddItem(nItemType, nCount, nValidTime, EItemAction_Add_Packet, nPacketID, false, false) )
    {
		lua_pushboolean(L,TRUE);
	}
    else
    {
		lua_pushboolean(L,FALSE);
	}

	return 1;
}

int lua_CanAddPacketItem(lua_State * L)
{
	if (lua_gettop(L) != 2 || !lua_isnumber(L, 1) || !lua_isnumber(L, 2) ){
		PARAM_ERROR;
		return 0;
	}

	int iEntity = (int)lua_tonumber(L, 1);
	itemtype_t nPacketID = (itemtype_t)lua_tonumber(L,2);

	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole ){
		WriteLog(LOGLEVEL_ERROR, "[%s]无法找到INDEX为%d的玩家\n", __FUNCTION__,  iEntity);;
		return 0;
	}

	CRoleItem *pRoleItem = pRole->GetComponent<CRoleItem>();
	if (pRoleItem == NULL) {
		WriteLog(LOGLEVEL_ERROR, "[%s]无法找到INDEX为%d的CRoleComponent玩家\n", __FUNCTION__, iEntity);
		return 0;
	}

	list<CItem> listItem;
    std::map<int, int> mapCurrency;
    pRoleItem->GetPacketItemInfo((unsigned short)iEntity, nPacketID, listItem, mapCurrency);

	int b = pRoleItem->CanAddAllItem(listItem);

	bool nRes = (b == EItemError_Success) ? 1 : 0 ;

	lua_pushnumber(L,nRes);
	return nRes;
}

int lua_GetRoleSex( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 1 && lua_isnumber(L, 1)  );
	if( !bValid ){
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}

	lua_pushnumber(L,pRole->GetSex());
	return 1;
}

int lua_ModifyRoleMoney( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1)&& lua_isnumber(L,2));
	if( !bValid ){
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	int nMoney = (int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole ){
		PARAM_ERROR;
		return 0;
	}
	CEntityAttributeComponent *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
	if (pRoleAttr != NULL){
		pRoleAttr->ChangeMoney(nMoney, EChangeMoneyCause_Add_Packet, 0);
	}
	return 0;
}

int lua_ModifyRoleBindMCoin(lua_State *L)
{
    bool bValid = (lua_gettop(L) == 2 && lua_isnumber(L,1) && lua_isnumber(L,2));
    if (!bValid)
    {
        PARAM_ERROR;
        return 0;
    }

    int iEntity = (int)lua_tonumber(L , 1);
    int nBindPoint = (int)lua_tonumber(L , 2);
    CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer(iEntity);
    if (NULL == pRole)
    {
        PARAM_ERROR;
        return 0;
    }

    CEntityAttributeComponent *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
    if (pRoleAttr != NULL && nBindPoint > 0)
    {
        pRoleAttr->ChangeBindBill(nBindPoint, EChangeBindBillCause_Add_Packet);
    }

    return 0;
}

int lua_AddRoleExp( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1)&& lua_isnumber(L,2)  );
	if( !bValid ){
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	unsigned int nExp = (unsigned int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole ){
		PARAM_ERROR;
		return 0;
	}
	CEntityAttributeComponent *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
	if (pRoleAttr != NULL){
		pRoleAttr->AddAbsExp(nExp, CRoleExpLog::EAddExpCause_Item, 0);
		ROLEINFO_DB *pInfo = new ROLEINFO_DB;
		pRole->PacketToDB(pInfo);

		CDelHelper delHelper(pInfo);
		g_pQueryMgr->AddQuery(QUERY_UpdateRole, 0, pInfo, 0, NULL, &delHelper);
	}
	return 0;
}

int lua_AddRoleHot( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1)&& lua_isnumber(L,2)  );
	if( !bValid ){
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	unsigned int nHot = (unsigned int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole ){
		PARAM_ERROR;
		return 0;
	}
	CEntityAttributeComponent *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
	if (pRoleAttr != NULL){
		pRoleAttr->SetHot(pRoleAttr->GetHot() + nHot, 0 , 0);
	}
	return 0;
}


int lua_AddVipValue( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 3 && lua_isnumber(L,1)&& lua_isnumber(L,2) && lua_isnumber(L,3) );
	if( !bValid ){
		PARAM_ERROR;
		return 0;
	}
	int iEntity  = (int)lua_tonumber( L , 1 );
	int nExp   = (int)lua_tonumber( L , 2 );
	int duration = (int)lua_tonumber( L , 3 );

	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole ){
		PARAM_ERROR;
		return 0;
	}
	CEntityVIPComponent *pRoleVIP = pRole->GetComponent<CRoleVIP>();
	if (pRoleVIP != NULL)
	{
		pRoleVIP->OnUpdateVIPInfo(nExp, duration);
	}
	return 0;
}

int lua_AddIntimacyValue( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1)&& lua_isnumber(L,2) );
	if( !bValid ){
		PARAM_ERROR;
		return 0;
	}
	int iEntity  = (int)lua_tonumber( L , 1 );
	int nValue   = (int)lua_tonumber( L , 2 );

	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole ){
		PARAM_ERROR;
		return 0;
	}
	CRoleCouple *pRoleCouple = pRole->GetComponent<CRoleCouple>();	
	if (pRoleCouple != NULL)
	{
		pRoleCouple->ChangeIntimacy(nValue, EChangeIntimacyCause_Item, 0);
	}
	return 0;
}


int lua_GetItemCount( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1)&&lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	itemtype_t nItemType = (itemtype_t)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	int nCount = 0;
	if( NULL == pRole ){
		PARAM_ERROR;
		return 0;
	}
	CEntityItemComponent *pRoleItem = pRole->GetComponent<CRoleItem>();
	if (pRoleItem != NULL){
		nCount = pRoleItem->GetItemCount(nItemType,true,true);
	}
	lua_pushnumber( L , nCount );
	return 1;
}

int lua_GetItemInfo( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 1 && lua_isnumber(L,1) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	itemtype_t nItemTypeID = (itemtype_t)lua_tonumber( L , 1 );
	ItemConfig* pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemTypeID);
	if(pItemInfo != NULL)
	{
		lua_pushboolean(L,TRUE);
		lua_pushstring( L,pItemInfo->m_strName.c_str() );
	}
	else{
		lua_pushboolean(L,FALSE);
		lua_pushnil(L);
	}
	return 2;
}

int lua_RemoveItemByType( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 3 && lua_isnumber(L,1)&&lua_isnumber(L,2)&&lua_isnumber(L,3) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	itemtype_t nItemType = (itemtype_t)lua_tonumber( L , 2 );
	unsigned short nCount = (unsigned short)lua_tonumber( L , 3 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole ){
		PARAM_ERROR;
		return 0;
	}
	CEntityItemComponent *pRoleItem = pRole->GetComponent<CRoleItem>();
	if (pRoleItem != NULL){
		pRoleItem->RemoveItemByType(nItemType, nCount, EItemAction_Del_LuaRecall);
	}
	return 0;
}

int lua_GetEmptyGridNumber( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1)&&lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

// 	int iEntity = (int)lua_tonumber( L , 1 );
// 	unsigned short nColumn = (unsigned short)lua_tonumber( L , 2 );
// 	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
// 	if( NULL == pRole )
// 	{
// 		PARAM_ERROR;
// 		return 0;
// 	}
// 
// 	CEntityItemComponent *pRoleItem = pRole->GetComponent<CRoleItem>();
// 	if (pRoleItem != NULL)
//     {
// 		unsigned short nEmptyGrimNumber = 0;/*pRoleItem->GetColumnEmptySlot((EItemColumn)nColumn)*/;
// 		lua_pushnumber(L,nEmptyGrimNumber);
// 		return 1;
// 	}
	return 0;
}

int lua_AllLineAnnounce( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1)&&lua_isstring(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	//const char * pChatContent = lua_tostring( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity );
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}
	//pRole->OnAllLineAnnounce( pChatContent );
	return 0;
}

int lua_SetRoomColor( lua_State *L )	// gene 7004
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1) && lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	int nValue = (int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}

	CRoomComponent *pRoleRoom = pRole->GetComponent<CRoleRoom>();
	if (pRoleRoom != NULL) {
		pRoleRoom->SetRoomColor(nValue);
	}

	return 0;
}

int  lua_AddRoomTop( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1) && lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	//int nValue = (int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}

	CRoomComponent *pRoomCOM = pRole->GetComponent<CRoleRoom>();
	if ( pRoomCOM != NULL )
	{
		bool bRoomTopState = ( pRoomCOM->Room() == NULL ? false : pRoomCOM->Room()->RoomTop() );
		pRoomCOM->SetRoomTop( true );

		if ( pRoomCOM->IsHost() )
		{
			CRoomManager::Instance().HandleTopRoom( bRoomTopState, pRoomCOM->Room() );
		}
	}

	return 0;
}

int  lua_DeleteRoomTop( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 2 && lua_isnumber(L,1) && lua_isnumber(L,2) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	//int nValue = (int)lua_tonumber( L , 2 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}

	CRoomComponent *pRoomCOM = pRole->GetComponent<CRoleRoom>();
	if ( pRoomCOM != NULL )
	{
		bool bRoomTopState = ( pRoomCOM->Room() == NULL ? false : pRoomCOM->Room()->RoomTop() );
		pRoomCOM->SetRoomTop( false );

		if ( pRoomCOM->IsHost() )
		{
			CRoomManager::Instance().HandleTopRoom( bRoomTopState, pRoomCOM->Room() );
		}
	}

	return 0;
}

int  lua_Transform( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 3 && lua_isnumber(L,1) && lua_isnumber(L,2) && lua_isnumber(L,3) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	unsigned int nTransformID = (int)lua_tonumber( L , 2 );
	unsigned int nDuration = (int)lua_tonumber( L , 3 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}
	if ( nTransformID == pRole->TransformID() )
	{
		return 0;
	}
	CItemTransformInfo* pTransformInfo = 
		ConfigManager::Instance().GetItemConfigManager().GetTransformByID(nTransformID);
	if ( NULL == pTransformInfo )
	{
		PARAM_ERROR;
		return 0;
	}

	CEntityItemComponent *pItemCOM = pRole->GetComponent<CRoleItem>();
	if ( pItemCOM != NULL )
	{
		//存储装备部位
		std::map<int, itemtype_t> mapClothPos;
		//ItemCloth_Type_All与skin外的其他部位互斥
		if ( pTransformInfo->m_szItemCloth[eItemClothType_All] > 0 )
		{
			if ( NULL != ConfigManager::Instance().GetItemConfigManager().GetByID(pTransformInfo->m_szItemCloth[eItemClothType_All]) )
			{
				mapClothPos[eItemClothType_All] = pTransformInfo->m_szItemCloth[eItemClothType_All];
			}
			else
			{
				WriteLog(LOGLEVEL_ERROR, "transform %u cloth(%d) do not exist", nTransformID, (int)pTransformInfo->m_szItemCloth[eItemClothType_All]);
			}
			if ( NULL != ConfigManager::Instance().GetItemConfigManager().GetByID(pTransformInfo->m_szItemCloth[eItemClothType_Skin]) )
			{
				mapClothPos[eItemClothType_Skin] = pTransformInfo->m_szItemCloth[eItemClothType_Skin];
			}
			else
			{
				WriteLog(LOGLEVEL_ERROR, "transform %u cloth(%d) do not exist", nTransformID, (int)pTransformInfo->m_szItemCloth[eItemClothType_Skin]);
			}
		}
		else
		{
			for (int i = eItemClothType_Hair; i < eItemClothType_Max; ++i)
			{
				if ( pTransformInfo->m_szItemCloth[i] > 0 )
				{
					if ( NULL != ConfigManager::Instance().GetItemConfigManager().GetByID(pTransformInfo->m_szItemCloth[i]) )
					{
						mapClothPos[i] = pTransformInfo->m_szItemCloth[i];
						//ItemCloth_Type_Suit与ItemCloth_Type_Body、ItemCloth_Type_Leg互斥
						if ( i == eItemClothType_Suit )
						{
							mapClothPos.erase(eItemClothType_Body);
							mapClothPos.erase(eItemClothType_Leg);
						}
					}
					else
					{
						WriteLog(LOGLEVEL_ERROR, "transform %u cloth(%d) do not exist", nTransformID, (int)pTransformInfo->m_szItemCloth[i]);
					}
				}
			}
		}
		pItemCOM->ApplyTransform(mapClothPos, nDuration);
		pRole->SetTransformIDAndNotify(nTransformID);
	}

	return 0;
}

int  lua_UnTransform( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 1 && lua_isnumber(L,1) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int iEntity = (int)lua_tonumber( L , 1 );
	CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer( iEntity ) ;
	if( NULL == pRole )
	{
		PARAM_ERROR;
		return 0;
	}
	if ( pRole->TransformID() > 0 )
	{
		CEntityItemComponent *pItemCOM = pRole->GetComponent<CRoleItem>();
		if ( pItemCOM != NULL )
		{
			pItemCOM->UnApplyTransform();
			pRole->SetTransformIDAndNotify(0);
		}
	}
	return 0;
}

int  lua_AddDanceGroupContribution(lua_State * L)
{
    bool bValid = (lua_gettop(L) == 2 && lua_isnumber(L, 1) && lua_isnumber(L, 2));
    if (!bValid)
    {
        PARAM_ERROR;
        return 0;
    }

    int iEntity = (int)lua_tonumber(L, 1);
    CRoleEntity * pRole = CPlayerManager::Instance().GetPlayer(iEntity);
    if (NULL == pRole)
    {
        PARAM_ERROR;
        return 0;
    }

    unsigned int nContribution = (unsigned int)lua_tonumber(L, 2);
    CEntityDanceGroupComponent * pRoleDanceGroup = pRole->GetComponent<CRoleDanceGroup>();
    if (pRoleDanceGroup != NULL)
    {
        pRoleDanceGroup->ChangeDanceGroupContribution(nContribution);
    }
    return 0;
}

void CInitLua::RegLuaRoleFuction()
{
	REGFN( ModifyRoleMoneyGain );
	REGFN( ModifyRoleExpGain );
	REGFN( ModifyRoleHotGain );
	REGFN( ModifySetIgnoreMis );

	REGFN( FindRoleGene );
	REGFN( RoleAddGene );
	REGFN( ModifySetVip );
	REGFN( RoleSetCurChatColor );
	REGFN( ModifyTitle );

	REGFN( AddItemToRole );
	REGFN( GetRoleSex );

	REGFN( ModifyRoleMoney );
	REGFN( ModifyRoleBindMCoin );
	REGFN( AddRoleExp );
	REGFN( AddRoleHot );
	REGFN( AddIntimacyValue );
	REGFN( AddVipValue );
	
	REGFN( GetItemInfo );
	REGFN( GetItemCount );
	REGFN( RemoveItemByType );
	REGFN( GetEmptyGridNumber );
	REGFN( AllLineAnnounce );
	REGFN( CanAddPacketItem );
	REGFN( Transform );
	REGFN( UnTransform );

	REGFN( SetRoomColor );
	REGFN( AddRoomTop );
	REGFN( DeleteRoomTop );

	REGFN( AddRingIntimacy );
	REGFN( DelRingIntimacy );
    REGFN(AddDanceGroupContribution);
}




//end file 




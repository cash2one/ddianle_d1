#include "ClothEffect.h"
#include "../util/CommonFunc.h"
#include "../../datastructure/datainfor/ItemConfigManager.h"
#include "../../datastructure/datainfor/EffectConfigManager.h"
#include "../../datastructure/datainfor/EffectConfig.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../libServerFrame/QueryManager.h"
#include "../../datastructure/RoleEntity.h"
#include "ItemMsgDefine.h"
#include "../item/EntityItemComponent.h"
#include "../../socket/Formatter.h"
#include "../util/Split.h"
#include "../roomLib/GameMsgRoomBroadcast.h"
#include "../roomLib/GameMsgNewRoom.h"

#ifdef WIN32
#pragma warning(disable:4061)
#endif

extern CQueryManager* g_pQueryMgr;

HandbookProgress::HandbookProgress(unsigned int nRoleID, unsigned short nSuitID)
    :m_nRoleID(nRoleID)
    ,m_nSuitID(nSuitID)
    ,m_nProgress(0)
{

}

HandbookProgress::~HandbookProgress()
{

}

void HandbookProgress::Initialize(const std::map<unsigned char,std::string> & handbookProgressData)
{
    std::map<unsigned char,std::string>::const_iterator it = handbookProgressData.begin();
    for (; it != handbookProgressData.end(); ++it)
    {
        std::set<unsigned int> setEffectID;
        if (Split(it->second, ":", setEffectID))
        {
            m_mapClothPosEffect[(EItemClothType)it->first] = setEffectID;
        }
    }

    m_nProgress = CalcProgress();
}

void HandbookProgress::SaveToMap( std::map<unsigned char, std::string> &rHandbookProgress ) const
{
    for ( std::map<EItemClothType, std::set<unsigned int> >::const_iterator itr = m_mapClothPosEffect.begin();
        itr != m_mapClothPosEffect.end(); ++itr )
    {
        std::string data;
        ConcatToString( itr->second, ":", data );

        if ( !data.empty() )
            rHandbookProgress.insert( std::make_pair( (unsigned char)itr->first, data) );
    }
}

bool HandbookProgress::IsCollectFinish()
{
    return m_nProgress == 100;
}

void HandbookProgress::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nProgress);
}

void HandbookProgress::UpdateProgress(unsigned int nEffectID)
{
    ClothEffectConfig * pEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(nEffectID);
    if (pEffectConfig == NULL)
    {
        return;
    }

    bool bFindProgress = false;
    unsigned int nMaxLevelEffectID = 0;
    std::map<EItemClothType, std::set<unsigned int> >::iterator it = m_mapClothPosEffect.find((EItemClothType)pEffectConfig->m_nClothPos);
    if (it != m_mapClothPosEffect.end())
    {
        std::set<unsigned int>::iterator iter = it->second.find(nEffectID);
        if (iter != it->second.end())
        {
            return;
        }

        bFindProgress = true;
        nMaxLevelEffectID = GetMaxLevelEffectID(it->second);
    }

    // 更新进度
    std::set<unsigned int> setAddEffectID;
    ConfigManager::Instance().GetEffectConfigMgr().GetBetweenEffectID(m_nSuitID, pEffectConfig->m_nClothPos, nMaxLevelEffectID, nEffectID, setAddEffectID);
    m_mapClothPosEffect[(EItemClothType)pEffectConfig->m_nClothPos].insert(setAddEffectID.begin(), setAddEffectID.end());
    m_nProgress  = CalcProgress();

    
    // sync to db
    if (bFindProgress)
    {
        SyncToDB((EItemClothType)pEffectConfig->m_nClothPos, ESyncToDBType_Update);
    }
    else
    {
        SyncToDB((EItemClothType)pEffectConfig->m_nClothPos, ESyncToDBType_Insert);
    }

    // sync to client
    GameMsg_S2C_RefreshHandbookProgress refreshmsg;
    refreshmsg.m_pHandbookProgress = this;
    refreshmsg.m_nSuitID = m_nSuitID;
    SendPlayerMsg(&refreshmsg);

    // log
    LogHandbookProgress((EItemClothType)pEffectConfig->m_nClothPos, setAddEffectID);
}

unsigned int HandbookProgress::GetMaxLevelEffectID(const std::set<unsigned int> & setEffectID)
{
    std::set<unsigned int>::const_reverse_iterator it = setEffectID.rbegin();
    if (it != setEffectID.rend())
    {
        return *it;
    }
    
    return 0;
}

void HandbookProgress::SyncToDB(EItemClothType eClothType, ESyncToDBType eSyncType)
{
    std::map<EItemClothType, std::set<unsigned int> >::iterator it = m_mapClothPosEffect.find(eClothType);
    if (it == m_mapClothPosEffect.end())
    {
        return;
    }

    std::string strEffectIDList;
    ConcatToString(it->second, std::string(":"), strEffectIDList);

    ClothEffectHandbookDB * pDBInfo = new ClothEffectHandbookDB();
    pDBInfo->m_nSuitID = m_nSuitID;
    pDBInfo->m_nParam1 = (unsigned short)it->first;
    pDBInfo->m_strParam2 = strEffectIDList;
    CDelHelper delHelper(pDBInfo);

    switch(eSyncType)
    {
    case ESyncToDBType_Insert:
        {
            g_pQueryMgr->AddQuery(QUERY_InsertHandbookProgress, m_nRoleID, pDBInfo, 0, NULL, &delHelper);
        }
        break;
    case ESyncToDBType_Update:
        {
            g_pQueryMgr->AddQuery(QUERY_UpdateHandbookProgress, m_nRoleID, pDBInfo, 0, NULL, &delHelper);
        }
        break;
    default:break;
    }
}

void HandbookProgress::SendPlayerMsg(GameMsg_Base * pMsg)
{
    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_nRoleID);
    if (pRoleEntity != NULL)
    {
        pRoleEntity->SendPlayerMsg(pMsg);
    }
}

void HandbookProgress::LogHandbookProgress(EItemClothType eItemType, std::set<unsigned int> & setAddEffectID)
{
    ClothEffectProgressLog * pLog = new ClothEffectProgressLog();
    pLog->m_nRoleID = m_nRoleID;
    pLog->m_eAction = ClothEffectProgressLog::EClothEffectProgressAction_Handbook;
    pLog->m_nSuitID = m_nSuitID;
    pLog->m_nParam1 = (unsigned int)eItemType;
    ConcatToString(setAddEffectID, std::string(":"), pLog->m_strParam2);

    g_pQueryMgr->AddQuery(QUERY_LogRoleClothEffectProgress, 0, pLog);
}

unsigned int HandbookProgress::CalcProgress()
{
    unsigned int nBodyLegTotalProgress = 0;
    unsigned int nSuitTotalProgress = 0;
    std::set<EItemClothType> setConfigPos;
    ConfigManager::Instance().GetEffectConfigMgr().GetSuitClothPos(m_nSuitID, setConfigPos);
    std::set<EItemClothType>::iterator itr = setConfigPos.begin();
    for (; itr != setConfigPos.end(); ++itr)
    {
        if (*itr == eItemClothType_Suit)
        {
            nSuitTotalProgress += 2*ConfigManager::Instance().GetEffectConfigMgr().GetSuitEffectNum(m_nSuitID, (unsigned short)*itr);
        }
        else if (*itr == eItemClothType_Body || *itr == eItemClothType_Leg)
        {
            nBodyLegTotalProgress += ConfigManager::Instance().GetEffectConfigMgr().GetSuitEffectNum(m_nSuitID, (unsigned short)*itr);
        }
        else
        {
            nBodyLegTotalProgress += ConfigManager::Instance().GetEffectConfigMgr().GetSuitEffectNum(m_nSuitID, (unsigned short)*itr);
            nSuitTotalProgress += ConfigManager::Instance().GetEffectConfigMgr().GetSuitEffectNum(m_nSuitID, (unsigned short)*itr);
        }
    }

    if (nBodyLegTotalProgress == 0 && nSuitTotalProgress == 0)
    {
        return 0;
    }

    unsigned int nBodyLegProgress = 0;
    unsigned int nSuitProgress = 0;
    std::map<EItemClothType, std::set<unsigned int> >::iterator it = m_mapClothPosEffect.begin();
    for (; it != m_mapClothPosEffect.end(); ++it)
    {
        if (it->first == eItemClothType_Suit)
        {
            nSuitProgress += 2 * it->second.size();
        }
        else if (it->first == eItemClothType_Body || it->first == eItemClothType_Leg)
        {
            nBodyLegProgress += it->second.size();
        }
        else
        {
            nSuitProgress += it->second.size();
            nBodyLegProgress += it->second.size();
        }
    }

    if (nBodyLegProgress >= nSuitProgress)
    {
        return (nBodyLegProgress*100)/nBodyLegTotalProgress;
    }
    else
    {
        return (nSuitProgress*100)/nSuitTotalProgress;
    }
}

SevenColorProgress::SevenColorProgress(unsigned int nRoleID, unsigned short nSuitID)
    :m_nRoleID(nRoleID)
    ,m_nSuitID(nSuitID)
    ,m_nBodyLegCount(0)
    ,m_nSuitCount(0)
{
    InitializeClothPos();
}

SevenColorProgress::~SevenColorProgress()
{

}

void SevenColorProgress::Initialize( const std::map<unsigned char, std::string> & colorProgressData )
{
    std::map<unsigned char, std::string>::const_iterator it = colorProgressData.begin();
    for (; it != colorProgressData.end(); ++it)
    {
        std::set<int> setClothPos;
        if (Split(it->second, ":", setClothPos))
        {
            std::set<int>::iterator iter = setClothPos.begin();
            for (; iter != setClothPos.end(); ++iter)
            {
                m_mapColorClothPos[(ESevenColorType)it->first].insert((EItemClothType)*iter);
            }
        }

        if (GetColorUpgress((ESevenColorType)it->first) == 100)
        {
            m_setColorProgress.insert((ESevenColorType)it->first);
        }
    }
}

void SevenColorProgress::SaveToMap( std::map<unsigned char, std::string> &rColorProgress ) const
{
    for ( std::map<ESevenColorType, std::set<EItemClothType> >::const_iterator itr = m_mapColorClothPos.begin();
        itr != m_mapColorClothPos.end(); ++itr )
    {
        std::string data;
        ConcatToString( itr->second, ":", data );

        if ( !data.empty() )
            rColorProgress.insert( std::make_pair( (unsigned char)itr->first, data) );
    }
}

void SevenColorProgress ::PackItemForOther(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_setColorProgress.size());
    std::set<ESevenColorType>::iterator it = m_setColorProgress.begin();
    for (; it != m_setColorProgress.end(); ++it)
    {
        IOBuff.AddUChar((unsigned char)*it);
    }
}

void SevenColorProgress::PackItemForOther(std::map<unsigned short,std::set<ESevenColorType> > & mapSevenColorProgress)
{
    if (!m_setColorProgress.empty())
    {
        mapSevenColorProgress[m_nSuitID].insert(m_setColorProgress.begin(), m_setColorProgress.end());
    }
}

void SevenColorProgress::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_mapColorClothPos.size());
    std::map<ESevenColorType,std::set<EItemClothType> >::iterator it = m_mapColorClothPos.begin();
    for (; it != m_mapColorClothPos.end(); ++it)
    {
        IOBuff.AddUChar((unsigned char)it->first);
        IOBuff.AddShort((short)GetColorUpgress(it->first));
    }
}

void SevenColorProgress::doEncodeToBuff( std::string &rBuff ) const
{
    char szBuff[256] = { 0 };
    CParamPool buffPool( szBuff, sizeof( szBuff ) );

    buffPool.AddUShort( (unsigned short)m_mapColorClothPos.size() );
    rBuff.append( buffPool.GetData(), buffPool.GetValidDataSize() );

    for ( std::map<ESevenColorType, std::set<EItemClothType> >::const_iterator itr = m_mapColorClothPos.begin();
        itr != m_mapColorClothPos.end(); ++itr )
    {
        buffPool.ClearData();
        buffPool.AddUChar( (unsigned char)itr->first );
        buffPool.AddShort( (short)GetColorUpgress( itr->first ) );

        rBuff.append( buffPool.GetData(), buffPool.GetValidDataSize() );
    }
}

int SevenColorProgress::GetColorUpgress(ESevenColorType eColorType) const
{
    if (m_setClothPos.empty())
    {
        return 0;
    }

    std::map<ESevenColorType,std::set<EItemClothType> >::const_iterator it = m_mapColorClothPos.find(eColorType);
    if (it == m_mapColorClothPos.end())
    {
        return 0;
    }

    int nSuitProgress = 0;
    int nBodyLegProgress = 0;
    std::set<EItemClothType>::iterator itr = it->second.begin();
    for (; itr != it->second.end(); ++itr)
    {
        if (*itr == eItemClothType_Suit)
        {
            nSuitProgress += 2;
        }
        else if (*itr == eItemClothType_Leg || *itr == eItemClothType_Body)
        {
            nBodyLegProgress++;
        }
        else
        {
            nSuitProgress++;
            nBodyLegProgress++;
        }
    }

    if (nBodyLegProgress >= nSuitProgress)
    {
        return (nBodyLegProgress*100)/(m_setClothPos.size() - m_nSuitCount);
    }
    else
    {
        return ((nSuitProgress - 1)*100)/(m_setClothPos.size() - m_nBodyLegCount);
    }
}

void SevenColorProgress::UpdateProgress(unsigned int nEffectID, itemtype_t nRelatedItemType)
{
    if (nRelatedItemType == 0 || nEffectID == 0)
    {
        return;
    }

    ClothEffectConfig * pEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(nEffectID);
    if (pEffectConfig == NULL)
    {
        return;
    }

    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nRelatedItemType);
    if (pItemConfig == NULL || pItemConfig->m_nClothColor <= eSevenColorType_None || pItemConfig->m_nClothColor >= eSevenColorType_Max)
    {
        return;
    }

    bool bFindProgress = false;
    std::map<ESevenColorType,std::set<EItemClothType> >::iterator it = m_mapColorClothPos.find((ESevenColorType)pItemConfig->m_nClothColor);
    if (it != m_mapColorClothPos.end())
    {
        std::set<EItemClothType>::iterator iter = it->second.find((EItemClothType)pEffectConfig->m_nClothPos);
        if (iter != it->second.end())
        {
            return;
        }

        bFindProgress = true;
    }

    m_mapColorClothPos[(ESevenColorType)pItemConfig->m_nClothColor].insert((EItemClothType)pEffectConfig->m_nClothPos);

    // sync progress
    if (GetColorUpgress((ESevenColorType)pItemConfig->m_nClothColor) == 100)
    {
        m_setColorProgress.insert(it->first);
    }

    // sync to db
    if (bFindProgress)
    {
        SyncToDB((ESevenColorType)pItemConfig->m_nClothColor, ESyncToDBType_Update);
    }
    else
    {
        SyncToDB((ESevenColorType)pItemConfig->m_nClothColor, ESyncToDBType_Insert);
    }
    
    // sync to client
    GameMsg_S2C_RefreshColorProgress refreshmsg;
    refreshmsg.m_pSevenColorProgress = this;
    refreshmsg.m_nSuitID = m_nSuitID;
    SendPlayerMsg(&refreshmsg);

    if (GetColorUpgress((ESevenColorType)pItemConfig->m_nClothColor) == 100)
    {
        // sync to others
        GameMsg_S2C_RefreshColorProgressToOthers othersmsg;
        othersmsg.m_nSuitID = m_nSuitID;
        othersmsg.m_nColor = pItemConfig->m_nClothColor;
        SendPlayerMsgToOthers(&othersmsg);

        GameMsg_S2G_RefreshSevenColor sgMsg;
        sgMsg.m_nSuitID = m_nSuitID;
        sgMsg.m_nColor = pItemConfig->m_nClothColor;
        SendMsg2GroupServer(&sgMsg);
    }

    // log
    LogColorProgress((ESevenColorType)pItemConfig->m_nClothColor, pEffectConfig->m_nClothPos);
}

int SevenColorProgress::CalcProgressRate(const std::set<EItemClothType> & setClothPos)
{
    if (setClothPos.empty() || m_setClothPos.empty())
    {
        return 0;
    }

    if (m_setClothPos.size() < setClothPos.size())
    {
        return 0;
    }

    return (int)((setClothPos.size()*100)/m_setClothPos.size());
}

void SevenColorProgress::SendPlayerMsg(GameMsg_Base * pMsg)
{
    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_nRoleID);
    if (pRoleEntity != NULL)
    {
        pRoleEntity->SendPlayerMsg(pMsg);
    }
}

void SevenColorProgress::SendPlayerMsgToOthers(GameMsg_Base * pMsg)
{
    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_nRoleID);
    if (pRoleEntity != NULL)
    {
        pRoleEntity->GetRoleItem()->SendPlayerMsgToOthers(pMsg);
    }
}

void SevenColorProgress::SendMsg2GroupServer(GameMsg_Base * pMsg)
{
    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(m_nRoleID);
    if (pRoleEntity != NULL)
    {
        pRoleEntity->SendMsg2GroupServer(pMsg);
    }
}


void SevenColorProgress::LogColorProgress(ESevenColorType eColorType, unsigned short nClothPos)
{
    ClothEffectProgressLog * pLog = new ClothEffectProgressLog();
    pLog->m_nRoleID = m_nRoleID;
    pLog->m_eAction = ClothEffectProgressLog::EClothEffectProgressAction_Color;
    pLog->m_nSuitID = m_nSuitID;
    pLog->m_nParam1 = (unsigned int)eColorType;
    Formatter(pLog->m_strParam2) << (unsigned int)nClothPos;

    g_pQueryMgr->AddQuery(QUERY_LogRoleClothEffectProgress, 0, pLog);
}

void SevenColorProgress::InitializeClothPos()
{
    ConfigManager::Instance().GetEffectConfigMgr().GetSuitClothPos(m_nSuitID, m_setClothPos);

    std::set<EItemClothType>::iterator it = m_setClothPos.find(eItemClothType_Body);
    if (it != m_setClothPos.end())
    {
        m_nBodyLegCount++;
    }

    it = m_setClothPos.find(eItemClothType_Leg);
    if (it != m_setClothPos.end())
    {
        m_nBodyLegCount++;
    }

    it = m_setClothPos.find(eItemClothType_Suit);
    if (it != m_setClothPos.end())
    {
        m_nSuitCount++;
    }
}

void SevenColorProgress::SyncToDB(ESevenColorType eColorType, ESyncToDBType eSyncType)
{
    std::map<ESevenColorType,std::set<EItemClothType> >::iterator it = m_mapColorClothPos.find(eColorType);
    if (it == m_mapColorClothPos.end())
    {
        return;
    }

    std::string strEffectIDList;
    ConcatToString(it->second, std::string(":"), strEffectIDList);

    ClothEffectHandbookDB * pDBInfo = new ClothEffectHandbookDB();
    pDBInfo->m_nSuitID = m_nSuitID;
    pDBInfo->m_nParam1 = (unsigned short)it->first;
    pDBInfo->m_strParam2 = strEffectIDList;
    CDelHelper delHelper(pDBInfo);

    switch(eSyncType)
    {
    case ESyncToDBType_Insert:
        {
            g_pQueryMgr->AddQuery(QUERY_InsertColorProgress, m_nRoleID, pDBInfo, 0, NULL, &delHelper);
        }
        break;
    case ESyncToDBType_Update:
        {
            g_pQueryMgr->AddQuery(QUERY_UpdateColorProgress, m_nRoleID, pDBInfo, 0, NULL, &delHelper);
        }
        break;
    default:break;
    }
}

ClothEffectHandbook::ClothEffectHandbook()
    :m_nRoleID(0)
{

}

ClothEffectHandbook::~ClothEffectHandbook()
{

}

void ClothEffectHandbook::Initialize(unsigned int nRoleID, const std::map<unsigned short,std::map<unsigned char,std::string> > & handbookProgressData,
    const std::map<unsigned short, std::map<unsigned char, std::string> > & colorProgressData )
{
    m_nRoleID = nRoleID;

    std::map<unsigned short,std::map<unsigned char,std::string> >::const_iterator it = handbookProgressData.begin();
    for (; it != handbookProgressData.end(); ++it)
    {
        std::map<unsigned short, HandbookProgress*>::iterator itr = m_mapHandbookProgress.find(it->first);
        if (itr == m_mapHandbookProgress.end())
        {
            HandbookProgress *pProgress = new HandbookProgress(nRoleID, it->first);
            if (pProgress != NULL)
            {
                pProgress->Initialize(it->second);
                m_mapHandbookProgress[it->first] = pProgress;
            }
        }
    }

    std::map<unsigned short, std::map<unsigned char, std::string> >::const_iterator iter = colorProgressData.begin();
    for (; iter != colorProgressData.end(); ++iter)
    {
        std::map<unsigned short, SevenColorProgress*>::iterator itr = m_mapSevenColorProgress.find(iter->first);
        if (itr == m_mapSevenColorProgress.end())
        {
            SevenColorProgress *pProgress = new SevenColorProgress(nRoleID, iter->first);
            if (pProgress != NULL)
            {
                pProgress->Initialize(iter->second);
                m_mapSevenColorProgress[iter->first] = pProgress;
            }
        }
    }
}

void ClothEffectHandbook::PacketToCache( std::map<unsigned short, std::map<unsigned char, std::string> > &rHandbookProgress,
    std::map<unsigned short, std::map<unsigned char, std::string> > &rColorProgress ) const
{
    for ( std::map<unsigned short, HandbookProgress *>::const_iterator handbookItr = m_mapHandbookProgress.begin();
        handbookItr != m_mapHandbookProgress.end(); ++handbookItr )
    {
        if ( handbookItr->second == NULL )
            continue;

        std::map<unsigned char, std::string> progress;
        handbookItr->second->SaveToMap( progress );

        if ( !progress.empty() )
            rHandbookProgress.insert( std::make_pair( handbookItr->first, progress ) );
    }

    for ( std::map<unsigned short, SevenColorProgress *>::const_iterator colorItr = m_mapSevenColorProgress.begin();
        colorItr != m_mapSevenColorProgress.end(); ++colorItr )
    {
        if ( colorItr->second == NULL )
            continue;

        std::map<unsigned char, std::string> progress;
        colorItr->second->SaveToMap( progress );

        if ( !progress.empty() )
            rColorProgress.insert( std::make_pair( colorItr->first, progress ) );
    }
}

void ClothEffectHandbook::Destory()
{
    std::map<unsigned short,HandbookProgress*>::iterator it = m_mapHandbookProgress.begin();
    for (; it != m_mapHandbookProgress.end(); ++it)
    {
        if (it->second != NULL)
        {
            delete it->second;it->second = NULL;
        }
    }
    m_mapHandbookProgress.clear();

    std::map<unsigned short,SevenColorProgress*>::iterator iter = m_mapSevenColorProgress.begin();
    for (; iter != m_mapSevenColorProgress.end(); ++iter)
    {
        if (iter->second != NULL)
        {
            delete iter->second;iter->second = NULL;
        }
    }
    m_mapSevenColorProgress.clear();
}

void ClothEffectHandbook::SerializeComponent(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_mapHandbookProgress.size());
    std::map<unsigned short,HandbookProgress*>::iterator it = m_mapHandbookProgress.begin();
    for (; it != m_mapHandbookProgress.end(); ++it)
    {
        IOBuff.AddUShort(it->first);
        it->second->doEncode(IOBuff);
    }

    IOBuff.AddUShort((unsigned short)m_mapSevenColorProgress.size());
    std::map<unsigned short,SevenColorProgress*>::iterator iter = m_mapSevenColorProgress.begin();
    for (; iter != m_mapSevenColorProgress.end(); ++iter)
    {
        IOBuff.AddUShort(iter->first);
        iter->second->doEncode(IOBuff);
    }
}

void ClothEffectHandbook::SerializeToBuff( std::string &rBuff ) const
{
    char szBuff[256] = { 0 };
    CParamPool buffPool( szBuff, sizeof( szBuff ) );

    buffPool.AddUShort( (unsigned short)m_mapHandbookProgress.size() );
    rBuff.append( buffPool.GetData(), buffPool.GetValidDataSize() );

    for ( std::map<unsigned short, HandbookProgress *>::const_iterator itrHandbook = m_mapHandbookProgress.begin();
        itrHandbook != m_mapHandbookProgress.end(); ++itrHandbook )
    {
        if ( itrHandbook->second == NULL )
            continue;

        buffPool.ClearData();
        buffPool.AddUShort( itrHandbook->first );
        itrHandbook->second->doEncode( buffPool );

        rBuff.append( buffPool.GetData(), buffPool.GetValidDataSize() );
    }

    buffPool.ClearData();
    buffPool.AddUShort( (unsigned short)m_mapSevenColorProgress.size() );
    rBuff.append( buffPool.GetData(), buffPool.GetValidDataSize() );

    for ( std::map<unsigned short, SevenColorProgress *>::const_iterator itrSevenColor = m_mapSevenColorProgress.begin();
        itrSevenColor != m_mapSevenColorProgress.end(); ++itrSevenColor )
    {
        if ( itrSevenColor->second == NULL )
            continue;

        buffPool.ClearData();
        buffPool.AddUShort( itrSevenColor->first );
        rBuff.append( buffPool.GetData(), buffPool.GetValidDataSize() );

        itrSevenColor->second->doEncodeToBuff( rBuff );
    }
}

void ClothEffectHandbook::UpdateProgress(unsigned int nEffectID, itemtype_t nRelatedItemType)
{
    ClothEffectConfig * pEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(nEffectID);
    if (pEffectConfig == NULL)
    {
        return;
    }

    UpdateHandbookProgress(pEffectConfig->m_nSuitID, nEffectID);

    UpdateColorProgress(pEffectConfig->m_nSuitID, pEffectConfig->m_nClothPos, nEffectID, nRelatedItemType);
}

void ClothEffectHandbook::UpdateHandbookProgress(unsigned short nSuitID, unsigned int nEffectID)
{
    std::map<unsigned short,HandbookProgress*>::iterator it = m_mapHandbookProgress.find(nSuitID);
    if (it != m_mapHandbookProgress.end())
    {
        it->second->UpdateProgress(nEffectID);
    }
    else
    {
        HandbookProgress * pHandbook = new HandbookProgress(m_nRoleID, nSuitID);
        if (pHandbook != NULL)
        {
            m_mapHandbookProgress[nSuitID] = pHandbook;
            pHandbook->UpdateProgress(nEffectID);
        }
    }
}

void ClothEffectHandbook::PackItemForOther(CParamPool & IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_mapSevenColorProgress.size());
    std::map<unsigned short,SevenColorProgress*>::iterator it = m_mapSevenColorProgress.begin();
    for (; it != m_mapSevenColorProgress.end(); ++it)
    {
        IOBuff.AddUShort(it->first);
        it->second->PackItemForOther(IOBuff);
    }
}

void ClothEffectHandbook::PackItemForOther(std::map<unsigned short,std::set<ESevenColorType> > & mapSevenColorProgress)
{
    std::map<unsigned short,SevenColorProgress*>::iterator it = m_mapSevenColorProgress.begin();
    for (; it != m_mapSevenColorProgress.end(); ++it)
    {
        it->second->PackItemForOther(mapSevenColorProgress);
    }
}

unsigned int ClothEffectHandbook::GetFinishCount()
{
    unsigned int nFinishCount = 0;
    std::map<unsigned short,HandbookProgress*>::iterator it = m_mapHandbookProgress.begin();
    for (; it != m_mapHandbookProgress.end(); ++it)
    {
        if (it->second->IsCollectFinish())
        {
            nFinishCount++;
        }
    }

    return nFinishCount;
}

bool ClothEffectHandbook::IsSuitCollectFinish(unsigned short nSuitID)
{
    std::map<unsigned short,HandbookProgress*>::iterator it = m_mapHandbookProgress.find(nSuitID);
    if (it != m_mapHandbookProgress.end())
    {
        return it->second->IsCollectFinish();
    }

    return false;
}

void ClothEffectHandbook::UpdateColorProgress(unsigned short nSuitID, unsigned short nClothPos, unsigned int nEffectID, itemtype_t nRelatedItemType)
{
    unsigned int nMaxLevelEffectID = ConfigManager::Instance().GetEffectConfigMgr().GetMaxLevelEffectID(nSuitID, nClothPos);
    if (nMaxLevelEffectID == nEffectID && nRelatedItemType > 0)
    {
        std::map<unsigned short,SevenColorProgress*>::iterator iter = m_mapSevenColorProgress.find(nSuitID);
        if (iter != m_mapSevenColorProgress.end())
        {
            iter->second->UpdateProgress(nEffectID, nRelatedItemType);
        }
        else
        {
            SevenColorProgress * pSevenColor = new SevenColorProgress(m_nRoleID, nSuitID);
            if (pSevenColor != NULL)
            {
                m_mapSevenColorProgress[nSuitID] = pSevenColor;
                pSevenColor->UpdateProgress(nEffectID, nRelatedItemType);
            }
        }
    }
}

ClothEffect::ClothEffect()
{

}

ClothEffect::~ClothEffect()
{

}

void ClothEffect::CreateFromDB(unsigned int nRoleID, const std::set<unsigned int> & setRoleEffect, const std::map<unsigned short,std::map<unsigned char,std::string> > & handbookProgressData,
    const std::map<unsigned short, std::map<unsigned char, std::string> > & colorProgressData )
{
    m_nRoleID = nRoleID;
    m_setRoleClothEffect = setRoleEffect;
    m_ClothEffectHandbook.Initialize(nRoleID, handbookProgressData, colorProgressData);
}

void ClothEffect::PacketToCache( std::set<unsigned int> &rRoleEffect, 
    std::map<unsigned short, std::map<unsigned char, std::string> > &rHandbookProgressData,
    std::map<unsigned short, std::map<unsigned char, std::string> > &rColorProgressData ) const
{
    rRoleEffect = m_setRoleClothEffect;
    m_ClothEffectHandbook.PacketToCache( rHandbookProgressData, rColorProgressData );
}

void ClothEffect::SerializeToBuff( std::string &rBuff ) const
{
    m_ClothEffectHandbook.SerializeToBuff( rBuff );
}

void ClothEffect::Destory()
{
    m_ClothEffectHandbook.Destory();
    m_setRoleClothEffect.clear();
}

void ClothEffect::EffectChange(unsigned int nEffectID, itemtype_t nRelatedItemType)
{
    if (!IsInRoleEffect(nEffectID))
    {
        AddRoleEffect(nEffectID);
    }

    // 更新特效进度
    m_ClothEffectHandbook.UpdateProgress(nEffectID, nRelatedItemType);
}

void ClothEffect::AddRoleEffect(unsigned int nEffectID)
{
    std::set<unsigned int>::iterator it = m_setRoleClothEffect.find(nEffectID);
    if (it != m_setRoleClothEffect.end())
    {
        return;
    }

    m_setRoleClothEffect.insert(nEffectID);

    // sync to db
    g_pQueryMgr->AddQuery(QUERY_InsertRoleEffect, m_nRoleID, 0, nEffectID);
}

bool ClothEffect::IsInRoleEffect(unsigned int nEffectID)
{
    std::set<unsigned int>::iterator it = m_setRoleClothEffect.find(nEffectID);
    if (it != m_setRoleClothEffect.end())
    {
        return true;
    }

    return false;
}

bool ClothEffect::IsSuitCollectFinish(unsigned short nSuitID)
{
    return m_ClothEffectHandbook.IsSuitCollectFinish(nSuitID);
}

void ClothEffect::PackItemForOther(CParamPool & IOBuff)
{
    m_ClothEffectHandbook.PackItemForOther(IOBuff);
}

void ClothEffect::PackItemForOther(std::map<unsigned short,std::set<ESevenColorType> > & mapSevenColorProgress)
{
    m_ClothEffectHandbook.PackItemForOther(mapSevenColorProgress);
}

void ClothEffect::CheckEffectListExist(const std::list<unsigned int> & listCheck, std::list<unsigned int> & listReturn)
{
    std::list<unsigned int>::const_iterator it = listCheck.begin();
    for (; it != listCheck.end(); ++it)
    {
        std::set<unsigned int>::iterator iter = m_setRoleClothEffect.find(*it);
        if (iter != m_setRoleClothEffect.end())
        {
            listReturn.push_back(*iter);
        }
    }
}

unsigned int ClothEffect::GetHandbookFinishCount()
{
    return m_ClothEffectHandbook.GetFinishCount();
}



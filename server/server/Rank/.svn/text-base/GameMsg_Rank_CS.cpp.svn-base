#include "GameMsg_Rank_CS.h"
#include "../share/ServerMsgDef.h"

#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"

#include "../../datastructure/datainfor/ConfigManager.h"

#include "../Pet/PetService.h"
#include "../Pet/PetComponent.h"


//////////////////////////////////////////////////////////////////////////

CRankPlayerInfo::CRankPlayerInfo()
    : m_nTransformID(0)
    , m_pRoleItem(NULL)
    , m_pRoleGene(NULL)
    , m_pRolePet(NULL)
{
}

void CRankPlayerInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_OfflineData.m_nRoleID);
    IOBuff.AddUChar(m_OfflineData.m_nSex);
    IOBuff.AddUChar(m_OfflineData.m_nSkin);
    IOBuff.AddBool(m_OfflineData.m_bVIP);
    IOBuff.AddUShort(m_OfflineData.m_nVIPLevel);
    IOBuff.AddUShort(m_nTransformID);

    if (NULL != m_pRoleItem)
    {
        m_pRoleItem->PackItemForOther(IOBuff);
    }
    else
    {
        int nOldPos1 = IOBuff.GetCurPosition();
        IOBuff.AddUShort(0);

        int nOldPos2 = IOBuff.GetCurPosition();
        IOBuff.AddUShort(0);

#define ITEM_BUFF_SIZE  256
        char szItemBuff[ITEM_BUFF_SIZE] = {0};
        short nCount = 0;
        ItemConfig *pItemInfo = NULL;

        for (std::vector<DBItem>::iterator itemItr = m_OfflineData.m_vecRoleItem.begin(); 
             itemItr != m_OfflineData.m_vecRoleItem.end(); ++itemItr)
        {
            IOBuff.AddUInt(itemItr->m_nTypeID);
            IOBuff.AddUShort(itemItr->m_nColumn);

            pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(itemItr->m_nTypeID);
            if (NULL != pItemInfo)
                itemItr->m_nIndex = pItemInfo->GetClothPos();

            IOBuff.AddUShort(itemItr->m_nIndex);

            memset(szItemBuff, 0, sizeof(szItemBuff) );
            CParamPool itemBuff(szItemBuff, sizeof(szItemBuff) );

            ItemEquip equipItem;
            equipItem.OnEquip();
            equipItem.SetLeftUseTimes(0);
            equipItem.SetCount(1);
            equipItem.SetDueTime(0);

            // 衣服特效
            std::map<itemid_t,unsigned int>::iterator it = m_OfflineData.m_mapClothEffect.find(itemItr->m_nItemID);
            if (it != m_OfflineData.m_mapClothEffect.end())
            {
                equipItem.SetEffectID(it->second);
            }
            else
            {
                equipItem.SetEffectID(0);
            }

            equipItem.Encode(&itemBuff);

            unsigned int nItemSize = itemBuff.GetValidDataSize();
            ASSERT(nItemSize == itemBuff.GetCurPosition() );

            IOBuff.AddUChar((unsigned char)nItemSize);
            IOBuff.AddMemory(itemBuff.GetData(), (unsigned short)nItemSize);

            ++nCount;
        }

        int nFinalPos = IOBuff.GetCurPosition();
        IOBuff.SetCurPosition(nOldPos2);
        IOBuff.ReplaceShort(nCount);
        IOBuff.SetCurPosition(nFinalPos);

        // 七色彩虹信息
        IOBuff.AddUShort((unsigned short)m_OfflineData.m_mapClothEffectSevenColor.size());
        for (std::map<unsigned short,std::set<ESevenColorType> >::iterator it = m_OfflineData.m_mapClothEffectSevenColor.begin();
            it != m_OfflineData.m_mapClothEffectSevenColor.end(); ++it)
        {
            IOBuff.AddUShort(it->first);
            IOBuff.AddUShort((unsigned short)it->second.size());
            for (std::set<ESevenColorType>::iterator itColor = it->second.begin();
                itColor != it->second.end(); ++itColor)
            {
                IOBuff.AddUChar((unsigned char)(*itColor));
            }
        }

        if (IOBuff.GetCurPosition() - nOldPos1 - sizeof(unsigned short) != 0)
        {
            nFinalPos = IOBuff.GetCurPosition();
            IOBuff.SetCurPosition(nOldPos1);
            IOBuff.ReplaceShort((short)(nFinalPos - nOldPos1 - sizeof(unsigned short) ) );
            IOBuff.SetCurPosition(nFinalPos);
        }
    }

    if (NULL != m_pRoleGene)
        m_pRoleGene->PackGeneForOther(IOBuff);
    else
        IOBuff.AddUShort(0);

    if (m_pRolePet != NULL)
    {
        Pet *pet = m_pRolePet->GetCarriedPet();
        IOBuff.AddBool(pet != NULL ? true : false);
        if (pet != NULL)
        {
            PlayerPet petInfo;
            PetService::Instance().EncodePetBrief(*pet, petInfo);
            petInfo.doEncode(IOBuff);
        }
    }
    else
    {
        IOBuff.AddBool( m_OfflineData.m_bHasCarriedPet );

        if ( m_OfflineData.m_bHasCarriedPet )
        {
            m_OfflineData.m_playerPet.doEncode( IOBuff );
        }
    }
}

//////////////////////////////////////////////////////////////////////////

CRankRowInfo::CRankRowInfo()
    : m_nRank(0)
    , m_nSmallRoleID(0)
{
}

void CRankRowInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nRank);

    IOBuff.AddUChar((unsigned char)m_listOtherInfo.size() );
    for (std::list<std::string>::const_iterator itr = m_listOtherInfo.begin(); 
         itr != m_listOtherInfo.end(); ++itr)
    {
        IOBuff.AddStringW(*itr);
    }

    IOBuff.AddUInt(m_nSmallRoleID);

    IOBuff.AddUChar((unsigned char)m_listPlayersInfo.size() );
    for (std::list<CRankPlayerInfo>::iterator itr = m_listPlayersInfo.begin(); 
         itr != m_listPlayersInfo.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_GetRankHall::GameMsg_C2S_GetRankHall()
    : GameMsg_Base(MSG_C2S_Rank_GetHall)
    , m_nCurrentPage(0)
    , m_nCount(0)
{
}

bool GameMsg_C2S_GetRankHall::doDecode(CParamPool &IOBuff)
{
    m_nCurrentPage = IOBuff.GetUShort();
    m_nCount = IOBuff.GetUChar();

    return true;
}

bool GameMsg_C2S_GetRankHall::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nCurrentPage);
    IOBuff.AddUChar(m_nCount);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_GetRankHallResult::GameMsg_S2C_GetRankHallResult()
    : GameMsg_Base(MSG_S2C_Rank_GetHallResult)
    , m_nCurrentPage(0)
    , m_nTotal(0)
{
}

bool GameMsg_S2C_GetRankHallResult::doDecode(CParamPool &IOBuff)
{
    m_nCurrentPage = IOBuff.GetUShort();
    m_nTotal = IOBuff.GetUShort();

    unsigned char count = IOBuff.GetUChar();

    for (unsigned char i = 0; i < count; ++i)
    {
        CRankTopOneInfo topOneInfo;

        topOneInfo.doDecode(IOBuff);

        m_listTopOneInfo.push_back(topOneInfo);
    }

    return true;
}

bool GameMsg_S2C_GetRankHallResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nCurrentPage);
    IOBuff.AddUShort(m_nTotal);

    IOBuff.AddUChar((unsigned char)m_listTopOneInfo.size() );
    for (std::list<CRankTopOneInfo>::const_iterator itr = m_listTopOneInfo.begin(); 
         itr != m_listTopOneInfo.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_GetRankDatas::GameMsg_C2S_GetRankDatas()
    : GameMsg_Base(MSG_C2S_Rank_GetDatas)
    , m_nType(0)
    , m_nDetailType(0)
    , m_nCurrentPage(0)
    , m_nCount(0)
{
}

bool GameMsg_C2S_GetRankDatas::doDecode(CParamPool &IOBuff)
{
    m_nType = IOBuff.GetUChar();
    m_nDetailType = IOBuff.GetUChar();
    m_nCurrentPage = IOBuff.GetUShort();
    m_nCount = IOBuff.GetUChar();

    return true;
}

bool GameMsg_C2S_GetRankDatas::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nType);
    IOBuff.AddUChar(m_nDetailType);
    IOBuff.AddUShort(m_nCurrentPage);
    IOBuff.AddUChar(m_nCount);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_GetRankDatasResult::GameMsg_S2C_GetRankDatasResult()
    : GameMsg_Base(MSG_S2C_Rank_GetDatasResult)
    , m_nMyRank(0)
    , m_nCurrentPage(0)
    , m_nTotal(0)
{
}

bool GameMsg_S2C_GetRankDatasResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nMyRank);
    IOBuff.AddUShort(m_nCurrentPage);
    IOBuff.AddUShort(m_nTotal);

    IOBuff.AddUChar((unsigned char)m_listRowInfo.size() );
    for (std::list<CRankRowInfo>::iterator itr = m_listRowInfo.begin(); 
         itr != m_listRowInfo.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_GetRankRewardRule::GameMsg_C2S_GetRankRewardRule()
    : GameMsg_Base( MSG_C2S_Rank_GetRewardRule )
    , m_nDataType( 0 )
{
}

GameMsg_C2S_GetRankRewardRule::~GameMsg_C2S_GetRankRewardRule()
{
}

bool GameMsg_C2S_GetRankRewardRule::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nDataType;

    return true;
}

//////////////////////////////////////////////////////////////////////////

RankRewardsInfo::RankRewardsInfo()
    : m_nBindMCoin( 0 )
    , m_nMoney( 0 )
    , m_nRangeBegin( 1 )
    , m_nRangeEnd( 1 )
{
}

bool RankRewardsInfo::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nBindMCoin << m_nMoney << m_nRangeBegin << m_nRangeEnd << m_listItems;

    return true;
}

GameMsg_S2C_GetRankRewardRuleResult::GameMsg_S2C_GetRankRewardRuleResult()
    : GameMsg_Base( MSG_S2C_Rank_GetRewardRuleResult )
{
}

GameMsg_S2C_GetRankRewardRuleResult::~GameMsg_S2C_GetRankRewardRuleResult()
{
}

bool GameMsg_S2C_GetRankRewardRuleResult::doEncode( CParamPool &IOBuff )
{
    IOBuff << stringw(m_strRewardRule) << stringw(m_strScoreRule) << m_listRewards;

    return true;
}


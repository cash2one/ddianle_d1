#include "DungeonDataMgr.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"
#include <algorithm>

CDungeonDataMgr & CDungeonDataMgr::instance()
{
	static CDungeonDataMgr s_instance;
	return s_instance;
}

CDungeonDataMgr::CDungeonDataMgr()
:m_nParcloseFailExp(0)
,m_nParcloseWinBaseExp(0)
,m_nNextDayResetTime(0)
,m_nOnceGiveVitNum(0)
,m_nDailyGetVitMaxNum(0)
,m_bQuickFinishOpenToNotVip(false)
{

}

// void CDungeonDataMgr::AddDungeonListConfig( const DungeonInfo & dungeonInfo )
// {
// 	DungeonInfoTable::iterator it = m_DungeonListConfig.find( dungeonInfo.nID );
// 	if ( it == m_DungeonListConfig.end() )
// 	{
// 		m_DungeonListConfig.insert( std::make_pair( dungeonInfo.nID, dungeonInfo) );
// 	}
// }

// void CDungeonDataMgr::AddParcloseInfoConfig( const ParcloseInfo & parcloseInfo )
// {
// 	ParcloseInfoTable::iterator it = m_ParcloseListInfo.find( parcloseInfo.nID );
// 	if ( it == m_ParcloseListInfo.end() )
// 	{
// 		m_ParcloseListInfo.insert( std::make_pair( parcloseInfo.nID, parcloseInfo) );
// 	}
// }


void CDungeonDataMgr::AddStarRewardConfig( const CStarRewardConfig & rewardInfo )
{
	m_StarRewardConfig[ rewardInfo.m_nID ] = rewardInfo;
}

CStarRewardConfig * CDungeonDataMgr::GetStarRewardConfig( unsigned int nID )
{
	StarRewardInfoTable::iterator it = m_StarRewardConfig.find( nID );
	if ( it != m_StarRewardConfig.end() )
	{
		return &it->second;
	}

	return NULL;
}

void CDungeonDataMgr::AddDustRewardConfig( const CDustRewardConfig & reward )
{
	m_DustRewardConfig = reward;
}

CDustRewardConfig * CDungeonDataMgr::GetDustRewardConfig()
{
	return &m_DustRewardConfig;
}

// void CDungeonDataMgr::AddTargetInfoConfig( const ParcloseTargetInfo & target )
// {
// 	m_TargetListConfig[ target.nID ] = target;
// }

void CDungeonDataMgr::AddParcloseItemList( const std::list<CItem> & items )
{
	m_ParcloseItemList = items;
}

void CDungeonDataMgr::GetThemeParcloseDrop( unsigned int nThemeID, unsigned int nParcloseID, std::vector<CItem> & maleitems, std::vector<CItem> & femaleitems, std::vector<DebrisReward> & debrisreward )
{
    CThemeParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeParcloseConfig( nThemeID, nParcloseID);
    if ( pParcloseConfig == NULL )
    {
        return;
    }

    CThemeDropConfig *  pDropInfo = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeDrop( nThemeID, nParcloseID );
    if ( pDropInfo == NULL)
    {
        return; 
    }

    if ( pDropInfo->m_nMustDropNum > 0  )
    {
        std::vector<CDropItemInfo>::iterator it = pDropInfo->m_MustMaleDrop.begin();
        for ( ; it != pDropInfo->m_MustMaleDrop.end(); ++it )
        {
            maleitems.push_back( it->m_itemInfo );
        }

        std::vector<CDropItemInfo>::iterator iter = pDropInfo->m_MustFemaleDrop.begin();
        for ( ; iter != pDropInfo->m_MustFemaleDrop.end(); ++iter )
        {
            femaleitems.push_back( iter->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator itDebris = pDropInfo->m_MustDebrisDrop.begin();
        for ( ; itDebris != pDropInfo->m_MustDebrisDrop.end(); ++itDebris )
        {
            DebrisReward reward;
            reward.nDebrisID = itDebris->m_nDebrisID;
            reward.nDebrisCount = itDebris->m_nDebrisCount;

            debrisreward.push_back( reward );
        }
    }

    if ( pDropInfo->m_nOptionalNum > 0  )
    {
        std::vector<CDropItemInfo>::iterator it = pDropInfo->m_OptionalMaleDrop.begin();
        for ( ; it != pDropInfo->m_OptionalMaleDrop.end(); ++it )
        {
            maleitems.push_back( it->m_itemInfo );
        }

        std::vector<CDropItemInfo>::iterator iter = pDropInfo->m_OptionalFemaleDrop.begin();
        for ( ; iter != pDropInfo->m_OptionalFemaleDrop.end(); ++iter )
        {
            femaleitems.push_back( iter->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator itDebris = pDropInfo->m_OptionalDebrisDrop.begin();
        for ( ; itDebris != pDropInfo->m_OptionalDebrisDrop.end(); ++itDebris )
        {
            DebrisReward reward;
            reward.nDebrisID = itDebris->m_nDebrisID;
            reward.nDebrisCount = itDebris->m_nDebrisCount;

            debrisreward.push_back( reward );
        }

        // À¬»ø
        std::map<unsigned int,CRubbishDropConfig*> & rubbish = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineRubbishDrop();
        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbish.begin();
        for ( ; itRubbish != rubbish.end(); ++itRubbish )
        {
            std::vector<CDropItemInfo>::iterator itMaleDrop = itRubbish->second->m_MaleDrop.begin();
            for ( ; itMaleDrop != itRubbish->second->m_MaleDrop.end(); ++itMaleDrop )
            {
                maleitems.push_back( itMaleDrop->m_itemInfo );
            }

            std::vector<CDropItemInfo>::iterator itFemaleDrop = itRubbish->second->m_FemaleDrop.begin();
            for ( ; itFemaleDrop != itRubbish->second->m_FemaleDrop.end(); ++itFemaleDrop )
            {
                femaleitems.push_back( itFemaleDrop->m_itemInfo );
            }
        }
    }
}

void CDungeonDataMgr::GetMainParcloseDrop( unsigned int nParcloseID, std::vector<CItem> & maleitems, std::vector<CItem> & femaleitems, std::vector<DebrisReward> & debrisreward )
{
	CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineParcloseConfig(nParcloseID);
	if ( pParcloseConfig == NULL )
	{
		return;
	}

    CMainlineDropConfig *  pDropInfo = ConfigManager::Instance().GetMagicLampConfigMgr().GetParcloseDropInfo( nParcloseID );
    if ( pDropInfo == NULL)
    {
        return; 
    }

	if ( pDropInfo->m_nMustDropNum > 0  )
	{
        std::vector<CDropItemInfo>::iterator it = pDropInfo->m_MustMaleDrop.begin();
        for ( ; it != pDropInfo->m_MustMaleDrop.end(); ++it )
        {
            maleitems.push_back( it->m_itemInfo );
        }

        std::vector<CDropItemInfo>::iterator iter = pDropInfo->m_MustFemaleDrop.begin();
        for ( ; iter != pDropInfo->m_MustFemaleDrop.end(); ++iter )
        {
            femaleitems.push_back( iter->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator itDebris = pDropInfo->m_MustDebrisDrop.begin();
        for ( ; itDebris != pDropInfo->m_MustDebrisDrop.end(); ++itDebris )
        {
            DebrisReward reward;
            reward.nDebrisID = itDebris->m_nDebrisID;
            reward.nDebrisCount = itDebris->m_nDebrisCount;

            debrisreward.push_back( reward );
        }
	}

    if ( pDropInfo->m_nOptionalNum > 0  )
    {
        std::vector<CDropItemInfo>::iterator it = pDropInfo->m_OptionalMaleDrop.begin();
        for ( ; it != pDropInfo->m_OptionalMaleDrop.end(); ++it )
        {
            maleitems.push_back( it->m_itemInfo );
        }

        std::vector<CDropItemInfo>::iterator iter = pDropInfo->m_OptionalFemaleDrop.begin();
        for ( ; iter != pDropInfo->m_OptionalFemaleDrop.end(); ++iter )
        {
            femaleitems.push_back( iter->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator itDebris = pDropInfo->m_OptionalDebrisDrop.begin();
        for ( ; itDebris != pDropInfo->m_OptionalDebrisDrop.end(); ++itDebris )
        {
            DebrisReward reward;
            reward.nDebrisID = itDebris->m_nDebrisID;
            reward.nDebrisCount = itDebris->m_nDebrisCount;

            debrisreward.push_back( reward );
        }

        // À¬»ø
        std::map<unsigned int,CRubbishDropConfig*> & rubbish = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineRubbishDrop();
        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbish.begin();
        for ( ; itRubbish != rubbish.end(); ++itRubbish )
        {
            std::vector<CDropItemInfo>::iterator itMaleDrop = itRubbish->second->m_MaleDrop.begin();
            for ( ; itMaleDrop != itRubbish->second->m_MaleDrop.end(); ++itMaleDrop )
            {
                maleitems.push_back( itMaleDrop->m_itemInfo );
            }

            std::vector<CDropItemInfo>::iterator itFemaleDrop = itRubbish->second->m_FemaleDrop.begin();
            for ( ; itFemaleDrop != itRubbish->second->m_FemaleDrop.end(); ++itFemaleDrop )
            {
                femaleitems.push_back( itFemaleDrop->m_itemInfo );
            }
        }
    }
}

void CDungeonDataMgr::CalcMainlineFemaleParcloseDrop( unsigned int nParcloseID, std::list<CItem> & listFemaleItem, std::list<DebrisReward> & listDebris )
{
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineParcloseConfig(nParcloseID);
    if ( pParcloseConfig == NULL )
    {
        return;
    }

    CMainlineDropConfig * pDrop = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineDrop( nParcloseID );
    if ( pDrop == NULL )
    {
        return;
    }

    unsigned int nMustDropItemCount = pDrop->m_MustFemaleDrop.size() + pDrop->m_MustDebrisDrop.size();
    if ( pDrop->m_nMustDropNum > 0 && pDrop->m_nMustDropNum >= nMustDropItemCount )
    {
        std::vector<CDropItemInfo>::iterator itIn = pDrop->m_MustFemaleDrop.begin();
        for ( ; itIn != pDrop->m_MustFemaleDrop.end(); ++itIn )
        {
            listFemaleItem.push_back( itIn->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator it = pDrop->m_MustDebrisDrop.begin();
        for ( ; it != pDrop->m_MustDebrisDrop.end(); ++it )
        {
            DebrisReward reward;
            reward.nDebrisID = it->m_nDebrisID;
            reward.nDebrisCount = it->m_nDebrisCount;

            listDebris.push_back( reward );
        }
    }
    else if ( pDrop->m_nMustDropNum > 0 && pDrop->m_nMustDropNum < nMustDropItemCount && nMustDropItemCount > 0 )
    {
        std::vector<CDropItemInfo> tempMustDropList( pDrop->m_MustFemaleDrop );
        std::vector<CDebrisDropInfo> tempDebrisDroplist( pDrop->m_MustDebrisDrop ); 
        for ( unsigned int i = 0; i < pDrop->m_nMustDropNum; ++ i )
        {
            std::vector<int> randomVec;
            std::vector<CItem> mustList;

            std::vector<CDropItemInfo>::iterator itItem = tempMustDropList.begin();
            for ( ; itItem != tempMustDropList.end(); ++itItem )
            {
                randomVec.push_back( itItem->m_nRate );
            }

            std::vector<CDebrisDropInfo>::iterator itDebris = tempDebrisDroplist.begin();
            for ( ; itDebris != tempDebrisDroplist.end(); ++itDebris )
            {
                randomVec.push_back( itDebris->m_nDropRate );
            }

            if ( !randomVec.empty() )
            {
                int nRandomIndex = RateRandom( randomVec );
                int nMiddleIndex = tempMustDropList.size() -1;
                if ( nRandomIndex <= nMiddleIndex )
                {
                    listFemaleItem.push_back( tempMustDropList[nRandomIndex].m_itemInfo );
                    tempMustDropList.erase(std::find(tempMustDropList.begin(), tempMustDropList.end(), tempMustDropList[nRandomIndex]));
                }
                else
                {
                    DebrisReward reward;
                    reward.nDebrisID = tempDebrisDroplist[nRandomIndex-1-nMiddleIndex].m_nDebrisID;
                    reward.nDebrisCount = tempDebrisDroplist[nRandomIndex-1-nMiddleIndex].m_nDebrisCount;
                    listDebris.push_back( reward);
                    tempDebrisDroplist.erase(std::find(tempDebrisDroplist.begin(), tempDebrisDroplist.end(), tempDebrisDroplist[nRandomIndex-1-nMiddleIndex]) );
                }
            }
        }
    }

    // ·Ç±ØÐëµôÂä
    // ÊÇ·ñµôÂä
    unsigned int nIsDropRand = Random(1, 100);
    if ( nIsDropRand >  pDrop->m_nOptionalRate )
    {
        return;
    }

    std::map<unsigned int,CRubbishDropConfig*> & rubbishdrop = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineRubbishDrop();

    unsigned int notMustCount = pDrop->m_OptionalFemaleDrop.size() + pDrop->m_OptionalDebrisDrop.size() + rubbishdrop.size();
    if ( pDrop->m_nOptionalNum > 0 && pDrop->m_nOptionalNum >= notMustCount )
    {
        std::vector<CDropItemInfo>::iterator it = pDrop->m_OptionalFemaleDrop.begin();
        for ( ; it != pDrop->m_OptionalFemaleDrop.end(); ++it )
        {
            listFemaleItem.push_back( it->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator iter = pDrop->m_OptionalDebrisDrop.begin();
        for ( ; iter != pDrop->m_OptionalDebrisDrop.end(); ++iter )
        {
            DebrisReward reward;
            reward.nDebrisID = iter->m_nDebrisID;
            reward.nDebrisCount = iter->m_nDebrisCount;

            listDebris.push_back( reward );
        }

        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbishdrop.begin();
        for ( ; itRubbish != rubbishdrop.end(); ++itRubbish )
        {
            std::vector<CDropItemInfo>::iterator itItem = itRubbish->second->m_FemaleDrop.begin();
            for ( ; itItem != itRubbish->second->m_FemaleDrop.end(); ++itItem )
            {
                listFemaleItem.push_back( itItem->m_itemInfo );
            }
        }
    }

    if ( pDrop->m_nOptionalNum > 0 && pDrop->m_nOptionalNum <  notMustCount && notMustCount > 0 )
    {
        std::vector<CDropItemInfo> tempitemlist( pDrop->m_OptionalFemaleDrop );
        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbishdrop.begin();
        for ( ; itRubbish != rubbishdrop.end(); ++itRubbish )
        {
            tempitemlist.insert( tempitemlist.end(),itRubbish->second->m_FemaleDrop.begin(), itRubbish->second->m_FemaleDrop.end() );
        }

        std::vector<CDebrisDropInfo> tempdebrislist( pDrop->m_OptionalDebrisDrop );

        for ( unsigned int i = 0; i < pDrop->m_nOptionalNum; ++ i )
        {
            std::vector<int> randomVec;

            std::vector<CDropItemInfo>::iterator itItem = tempitemlist.begin();
            for ( ; itItem != tempitemlist.end(); ++itItem )
            {
                randomVec.push_back( itItem->m_nRate  );
            }

            std::vector<CDebrisDropInfo>::iterator itDebris = tempdebrislist.begin();
            for ( ; itDebris != tempdebrislist.end(); ++itDebris )
            {
                randomVec.push_back( itDebris->m_nDropRate );
            }

            if ( !randomVec.empty() )
            {
                int nRandomIndex = RateRandom( randomVec );
                int nMiddleIndex = tempitemlist.size() -1;
                if ( nRandomIndex <= nMiddleIndex )
                {
                    listFemaleItem.push_back( tempitemlist[nRandomIndex].m_itemInfo );
                    tempitemlist.erase(std::find(tempitemlist.begin(), tempitemlist.end(), tempitemlist[nRandomIndex]));
                }
                else
                {
                    DebrisReward reward;
                    reward.nDebrisID = tempdebrislist[nRandomIndex-1-nMiddleIndex].m_nDebrisID;
                    reward.nDebrisCount = tempdebrislist[nRandomIndex-1-nMiddleIndex].m_nDebrisCount;
                    listDebris.push_back( reward);
                    tempdebrislist.erase(std::find(tempdebrislist.begin(), tempdebrislist.end(), tempdebrislist[nRandomIndex-1-nMiddleIndex]) );
                }
            }
        }
    }
}

void CDungeonDataMgr::CalcParcloseDrop( int nSex, EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID, std::list<CItem> & listItem, std::list<DebrisReward> & listDebris)
{
    switch( eType )
    {
    case  EDungeonType_Mainline:
        {
            if ( nSex == ESexType_Male )
            {
                CalcMainlineMaleParcloseDrop( nParcloseID, listItem, listDebris );
            }
            else
            {
                CalcMainlineFemaleParcloseDrop( nParcloseID,listItem, listDebris);
            }   
        }
        break;
    case EDungeonType_Theme:
        {
            if ( nSex == ESexType_Male )
            {
                CalThemeMaleParcloseDrop( nDungeonID, nParcloseID, listItem, listDebris );
            }
            else
            {
                CalThemeFemaleParcloseDrop( nDungeonID, nParcloseID, listItem, listDebris );
            }
        }
        break;  
    }
}

void CDungeonDataMgr::CalThemeFemaleParcloseDrop( unsigned int nThemeID, unsigned int nParcloseID, std::list<CItem> & listFemaleItem, std::list<DebrisReward> & listDebris )
{
    CThemeParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeParcloseConfig( nThemeID, nParcloseID);
    if ( pParcloseConfig == NULL )
    {
        return;
    }

    CThemeDropConfig * pDrop = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeDrop(nThemeID, nParcloseID );
    if ( pDrop == NULL )
    {
        return;
    }

    unsigned int nMustDropItemCount = pDrop->m_MustFemaleDrop.size() + pDrop->m_MustDebrisDrop.size();
    if ( pDrop->m_nMustDropNum > 0 && pDrop->m_nMustDropNum >= nMustDropItemCount )
    {
        std::vector<CDropItemInfo>::iterator itIn = pDrop->m_MustFemaleDrop.begin();
        for ( ; itIn != pDrop->m_MustFemaleDrop.end(); ++itIn )
        {
            listFemaleItem.push_back( itIn->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator it = pDrop->m_MustDebrisDrop.begin();
        for ( ; it != pDrop->m_MustDebrisDrop.end(); ++it )
        {
            DebrisReward reward;
            reward.nDebrisID = it->m_nDebrisID;
            reward.nDebrisCount = it->m_nDebrisCount;

            listDebris.push_back( reward );
        }
    }
    else if ( pDrop->m_nMustDropNum > 0 && pDrop->m_nMustDropNum < nMustDropItemCount && nMustDropItemCount > 0 )
    {
        std::vector<CDropItemInfo> tempMustDropList( pDrop->m_MustFemaleDrop );
        std::vector<CDebrisDropInfo> tempDebrisDroplist( pDrop->m_MustDebrisDrop ); 
        for ( unsigned int i = 0; i < pDrop->m_nMustDropNum; ++ i )
        {
            std::vector<int> randomVec;
            std::vector<CItem> mustList;

            std::vector<CDropItemInfo>::iterator itItem = tempMustDropList.begin();
            for ( ; itItem != tempMustDropList.end(); ++itItem )
            {
                randomVec.push_back( itItem->m_nRate );
            }

            std::vector<CDebrisDropInfo>::iterator itDebris = tempDebrisDroplist.begin();
            for ( ; itDebris != tempDebrisDroplist.end(); ++itDebris )
            {
                randomVec.push_back( itDebris->m_nDropRate );
            }

            if ( !randomVec.empty() )
            {
                int nRandomIndex = RateRandom( randomVec );
                int nMiddleIndex = tempMustDropList.size() -1;
                if ( nRandomIndex <= nMiddleIndex )
                {
                    listFemaleItem.push_back( tempMustDropList[nRandomIndex].m_itemInfo );
                    tempMustDropList.erase(std::find(tempMustDropList.begin(), tempMustDropList.end(), tempMustDropList[nRandomIndex]));
                }
                else
                {
                    DebrisReward reward;
                    reward.nDebrisID = tempDebrisDroplist[nRandomIndex-1-nMiddleIndex].m_nDebrisID;
                    reward.nDebrisCount = tempDebrisDroplist[nRandomIndex-1-nMiddleIndex].m_nDebrisCount;
                    listDebris.push_back( reward);
                    tempDebrisDroplist.erase(std::find(tempDebrisDroplist.begin(), tempDebrisDroplist.end(), tempDebrisDroplist[nRandomIndex-1-nMiddleIndex]) );
                }
            }
        }
    }

    // ·Ç±ØÐëµôÂä
    // ÊÇ·ñµôÂä
    unsigned int nIsDropRand = Random(1, 100);
    if ( nIsDropRand >  pDrop->m_nOptionalRate )
    {
        return;
    }

    std::map<unsigned int,CRubbishDropConfig*> & rubbishdrop = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeRubbishDrop();

    unsigned int notMustCount = pDrop->m_OptionalFemaleDrop.size() + pDrop->m_OptionalDebrisDrop.size() + rubbishdrop.size();
    if ( pDrop->m_nOptionalNum > 0 && pDrop->m_nOptionalNum >= notMustCount )
    {
        std::vector<CDropItemInfo>::iterator it = pDrop->m_OptionalFemaleDrop.begin();
        for ( ; it != pDrop->m_OptionalFemaleDrop.end(); ++it )
        {
            listFemaleItem.push_back( it->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator iter = pDrop->m_OptionalDebrisDrop.begin();
        for ( ; iter != pDrop->m_OptionalDebrisDrop.end(); ++iter )
        {
            DebrisReward reward;
            reward.nDebrisID = iter->m_nDebrisID;
            reward.nDebrisCount = iter->m_nDebrisCount;

            listDebris.push_back( reward );
        }

        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbishdrop.begin();
        for ( ; itRubbish != rubbishdrop.end(); ++itRubbish )
        {
            std::vector<CDropItemInfo>::iterator itItem = itRubbish->second->m_FemaleDrop.begin();
            for ( ; itItem != itRubbish->second->m_FemaleDrop.end(); ++itItem )
            {
                listFemaleItem.push_back( itItem->m_itemInfo );
            }
        }
    }

    if ( pDrop->m_nOptionalNum > 0 && pDrop->m_nOptionalNum <  notMustCount && notMustCount > 0 )
    {
        std::vector<CDropItemInfo> tempitemlist( pDrop->m_OptionalFemaleDrop );
        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbishdrop.begin();
        for ( ; itRubbish != rubbishdrop.end(); ++itRubbish )
        {
            tempitemlist.insert( tempitemlist.end(),itRubbish->second->m_FemaleDrop.begin(), itRubbish->second->m_FemaleDrop.end() );
        }

        std::vector<CDebrisDropInfo> tempdebrislist( pDrop->m_OptionalDebrisDrop );

        for ( unsigned int i = 0; i < pDrop->m_nOptionalNum; ++ i )
        {
            std::vector<int> randomVec;

            std::vector<CDropItemInfo>::iterator itItem = tempitemlist.begin();
            for ( ; itItem != tempitemlist.end(); ++itItem )
            {
                randomVec.push_back( itItem->m_nRate  );
            }

            std::vector<CDebrisDropInfo>::iterator itDebris = tempdebrislist.begin();
            for ( ; itDebris != tempdebrislist.end(); ++itDebris )
            {
                randomVec.push_back( itDebris->m_nDropRate );
            }

            if ( !randomVec.empty() )
            {
                int nRandomIndex = RateRandom( randomVec );
                int nMiddleIndex = tempitemlist.size() -1;
                if ( nRandomIndex <= nMiddleIndex )
                {
                    listFemaleItem.push_back( tempitemlist[nRandomIndex].m_itemInfo );
                    tempitemlist.erase(std::find(tempitemlist.begin(), tempitemlist.end(), tempitemlist[nRandomIndex]));
                }
                else
                {
                    DebrisReward reward;
                    reward.nDebrisID = tempdebrislist[nRandomIndex-1-nMiddleIndex].m_nDebrisID;
                    reward.nDebrisCount = tempdebrislist[nRandomIndex-1-nMiddleIndex].m_nDebrisCount;
                    listDebris.push_back( reward);
                    tempdebrislist.erase(std::find(tempdebrislist.begin(), tempdebrislist.end(), tempdebrislist[nRandomIndex-1-nMiddleIndex]) );
                }
            }
        }
    }
}

void CDungeonDataMgr::CalThemeMaleParcloseDrop( unsigned int nThemeID, unsigned int nParcloseID, std::list<CItem> & listMaleItem, std::list<DebrisReward> & listDebris )
{
    CThemeParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeParcloseConfig( nThemeID, nParcloseID);
    if ( pParcloseConfig == NULL )
    {
        return;
    }

    CThemeDropConfig * pDrop = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeDrop( nThemeID, nParcloseID );
    if ( pDrop == NULL )
    {
        return;
    }

    unsigned int nMustDropItemCount = pDrop->m_MustMaleDrop.size() + pDrop->m_MustDebrisDrop.size();
    if ( pDrop->m_nMustDropNum > 0 && pDrop->m_nMustDropNum >= nMustDropItemCount )
    {
        std::vector<CDropItemInfo>::iterator itIn = pDrop->m_MustMaleDrop.begin();
        for ( ; itIn != pDrop->m_MustMaleDrop.end(); ++itIn )
        {
            listMaleItem.push_back( itIn->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator it = pDrop->m_MustDebrisDrop.begin();
        for ( ; it != pDrop->m_MustDebrisDrop.end(); ++it )
        {
            DebrisReward reward;
            reward.nDebrisID = it->m_nDebrisID;
            reward.nDebrisCount = it->m_nDebrisCount;

            listDebris.push_back( reward );
        }
    }
    else if ( pDrop->m_nMustDropNum > 0 && pDrop->m_nMustDropNum < nMustDropItemCount && nMustDropItemCount > 0 )
    {
        std::vector<CDropItemInfo> tempMustDropList( pDrop->m_MustMaleDrop );
        std::vector<CDebrisDropInfo> tempDebrisDroplist( pDrop->m_MustDebrisDrop ); 
        for ( unsigned int i = 0; i < pDrop->m_nMustDropNum; ++ i )
        {
            std::vector<int> randomVec;
            std::vector<CItem> mustList;

            std::vector<CDropItemInfo>::iterator itItem = tempMustDropList.begin();
            for ( ; itItem != tempMustDropList.end(); ++itItem )
            {
                randomVec.push_back( itItem->m_nRate );
            }

            std::vector<CDebrisDropInfo>::iterator itDebris = tempDebrisDroplist.begin();
            for ( ; itDebris != tempDebrisDroplist.end(); ++itDebris )
            {
                randomVec.push_back( itDebris->m_nDropRate );
            }

            if ( !randomVec.empty() )
            {
                int nRandomIndex = RateRandom( randomVec );
                int nMiddleIndex = tempMustDropList.size() -1;
                if ( nRandomIndex <= nMiddleIndex )
                {
                    listMaleItem.push_back( tempMustDropList[nRandomIndex].m_itemInfo );
                    tempMustDropList.erase(std::find(tempMustDropList.begin(), tempMustDropList.end(), tempMustDropList[nRandomIndex]));
                }
                else
                {
                    DebrisReward reward;
                    reward.nDebrisID = tempDebrisDroplist[nRandomIndex-1-nMiddleIndex].m_nDebrisID;
                    reward.nDebrisCount = tempDebrisDroplist[nRandomIndex-1-nMiddleIndex].m_nDebrisCount;
                    listDebris.push_back( reward);
                    tempDebrisDroplist.erase(std::find(tempDebrisDroplist.begin(), tempDebrisDroplist.end(), tempDebrisDroplist[nRandomIndex-1-nMiddleIndex]) );
                }
            }
        }
    }

    // ·Ç±ØÐëµôÂä
    // ÊÇ·ñµôÂä
    unsigned int nIsDropRand = Random(1, 100);
    if ( nIsDropRand >  pDrop->m_nOptionalRate )
    {
        return;
    }

    std::map<unsigned int,CRubbishDropConfig*> & rubbishdrop = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeRubbishDrop();

    unsigned int notMustCount = pDrop->m_OptionalMaleDrop.size() + pDrop->m_OptionalDebrisDrop.size() + rubbishdrop.size();
    if ( pDrop->m_nOptionalNum > 0 && pDrop->m_nOptionalNum >= notMustCount )
    {
        std::vector<CDropItemInfo>::iterator it = pDrop->m_OptionalMaleDrop.begin();
        for ( ; it != pDrop->m_OptionalMaleDrop.end(); ++it )
        {
            listMaleItem.push_back( it->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator iter = pDrop->m_OptionalDebrisDrop.begin();
        for ( ; iter != pDrop->m_OptionalDebrisDrop.end(); ++iter )
        {
            DebrisReward reward;
            reward.nDebrisID = iter->m_nDebrisID;
            reward.nDebrisCount = iter->m_nDebrisCount;

            listDebris.push_back( reward );
        }

        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbishdrop.begin();
        for ( ; itRubbish != rubbishdrop.end(); ++itRubbish )
        {
            std::vector<CDropItemInfo>::iterator itItem = itRubbish->second->m_MaleDrop.begin();
            for ( ; itItem != itRubbish->second->m_MaleDrop.end(); ++itItem )
            {
                listMaleItem.push_back( itItem->m_itemInfo );
            }
        }
    }

    if ( pDrop->m_nOptionalNum > 0 && pDrop->m_nOptionalNum <  notMustCount && notMustCount > 0 )
    {
        std::vector<CDropItemInfo> tempitemlist( pDrop->m_OptionalMaleDrop );
        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbishdrop.begin();
        for ( ; itRubbish != rubbishdrop.end(); ++itRubbish )
        {
            tempitemlist.insert( tempitemlist.end(),itRubbish->second->m_MaleDrop.begin(), itRubbish->second->m_MaleDrop.end() );
        }

        std::vector<CDebrisDropInfo> tempdebrislist( pDrop->m_OptionalDebrisDrop );

        for ( unsigned int i = 0; i < pDrop->m_nOptionalNum; ++ i )
        {
            std::vector<int> randomVec;

            std::vector<CDropItemInfo>::iterator itItem = tempitemlist.begin();
            for ( ; itItem != tempitemlist.end(); ++itItem )
            {
                randomVec.push_back( itItem->m_nRate  );
            }

            std::vector<CDebrisDropInfo>::iterator itDebris = tempdebrislist.begin();
            for ( ; itDebris != tempdebrislist.end(); ++itDebris )
            {
                randomVec.push_back( itDebris->m_nDropRate );
            }

            if ( !randomVec.empty() )
            {
                int nRandomIndex = RateRandom( randomVec );
                int nMiddleIndex = tempitemlist.size() -1;
                if ( nRandomIndex <= nMiddleIndex )
                {
                    listMaleItem.push_back( tempitemlist[nRandomIndex].m_itemInfo );
                    tempitemlist.erase(std::find(tempitemlist.begin(), tempitemlist.end(), tempitemlist[nRandomIndex]));
                }
                else
                {
                    DebrisReward reward;
                    reward.nDebrisID = tempdebrislist[nRandomIndex-1-nMiddleIndex].m_nDebrisID;
                    reward.nDebrisCount = tempdebrislist[nRandomIndex-1-nMiddleIndex].m_nDebrisCount;
                    listDebris.push_back( reward);
                    tempdebrislist.erase(std::find(tempdebrislist.begin(), tempdebrislist.end(), tempdebrislist[nRandomIndex-1-nMiddleIndex]) );
                }
            }
        }
    }
}

void CDungeonDataMgr::CalcMainlineMaleParcloseDrop(unsigned int nParcloseID, std::list<CItem> & listMaleItem, std::list<DebrisReward> & listDebris)
{
    CParcloseConfig * pParcloseConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineParcloseConfig(nParcloseID);
    if ( pParcloseConfig == NULL )
    {
        return;
    }

    CMainlineDropConfig * pDrop = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineDrop( nParcloseID );
    if ( pDrop == NULL )
    {
        return;
    }

    unsigned int nMustDropItemCount = pDrop->m_MustMaleDrop.size() + pDrop->m_MustDebrisDrop.size();
    if ( pDrop->m_nMustDropNum > 0 && pDrop->m_nMustDropNum >= nMustDropItemCount )
    {
        std::vector<CDropItemInfo>::iterator itIn = pDrop->m_MustMaleDrop.begin();
        for ( ; itIn != pDrop->m_MustMaleDrop.end(); ++itIn )
        {
            listMaleItem.push_back( itIn->m_itemInfo );
        }
        
        std::vector<CDebrisDropInfo>::iterator it = pDrop->m_MustDebrisDrop.begin();
        for ( ; it != pDrop->m_MustDebrisDrop.end(); ++it )
        {
            DebrisReward reward;
            reward.nDebrisID = it->m_nDebrisID;
            reward.nDebrisCount = it->m_nDebrisCount;

            listDebris.push_back( reward );
        }
    }
    else if ( pDrop->m_nMustDropNum > 0 && pDrop->m_nMustDropNum < nMustDropItemCount && nMustDropItemCount > 0 )
    {
        std::vector<CDropItemInfo> tempMustDropList( pDrop->m_MustMaleDrop );
        std::vector<CDebrisDropInfo> tempDebrisDroplist( pDrop->m_MustDebrisDrop ); 
        for ( unsigned int i = 0; i < pDrop->m_nMustDropNum; ++ i )
        {
            std::vector<int> randomVec;
            std::vector<CItem> mustList;

            std::vector<CDropItemInfo>::iterator itItem = tempMustDropList.begin();
            for ( ; itItem != tempMustDropList.end(); ++itItem )
            {
                randomVec.push_back( itItem->m_nRate );
            }

            std::vector<CDebrisDropInfo>::iterator itDebris = tempDebrisDroplist.begin();
            for ( ; itDebris != tempDebrisDroplist.end(); ++itDebris )
            {
                randomVec.push_back( itDebris->m_nDropRate );
            }

            if ( !randomVec.empty() )
            {
                int nRandomIndex = RateRandom( randomVec );
                int nMiddleIndex = tempMustDropList.size() -1;
                if ( nRandomIndex <= nMiddleIndex )
                {
                    listMaleItem.push_back( tempMustDropList[nRandomIndex].m_itemInfo );
                    tempMustDropList.erase(std::find(tempMustDropList.begin(), tempMustDropList.end(), tempMustDropList[nRandomIndex]));
                }
                else
                {
                    DebrisReward reward;
                    reward.nDebrisID = tempDebrisDroplist[nRandomIndex-1-nMiddleIndex].m_nDebrisID;
                    reward.nDebrisCount = tempDebrisDroplist[nRandomIndex-1-nMiddleIndex].m_nDebrisCount;
                    listDebris.push_back( reward);
                    tempDebrisDroplist.erase(std::find(tempDebrisDroplist.begin(), tempDebrisDroplist.end(), tempDebrisDroplist[nRandomIndex-1-nMiddleIndex]) );
                }
            }
        }
    }

    // ·Ç±ØÐëµôÂä
    // ÊÇ·ñµôÂä
    unsigned int nIsDropRand = Random(1, 100);
    if ( nIsDropRand >  pDrop->m_nOptionalRate )
    {
        return;
    }

    std::map<unsigned int,CRubbishDropConfig*> & rubbishdrop = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineRubbishDrop();

    unsigned int notMustCount = pDrop->m_OptionalMaleDrop.size() + pDrop->m_OptionalDebrisDrop.size() + rubbishdrop.size();
    if ( pDrop->m_nOptionalNum > 0 && pDrop->m_nOptionalNum >= notMustCount )
    {
        std::vector<CDropItemInfo>::iterator it = pDrop->m_OptionalMaleDrop.begin();
        for ( ; it != pDrop->m_OptionalMaleDrop.end(); ++it )
        {
            listMaleItem.push_back( it->m_itemInfo );
        }

        std::vector<CDebrisDropInfo>::iterator iter = pDrop->m_OptionalDebrisDrop.begin();
        for ( ; iter != pDrop->m_OptionalDebrisDrop.end(); ++iter )
        {
            DebrisReward reward;
            reward.nDebrisID = iter->m_nDebrisID;
            reward.nDebrisCount = iter->m_nDebrisCount;

            listDebris.push_back( reward );
        }

        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbishdrop.begin();
        for ( ; itRubbish != rubbishdrop.end(); ++itRubbish )
        {
            std::vector<CDropItemInfo>::iterator itItem = itRubbish->second->m_MaleDrop.begin();
            for ( ; itItem != itRubbish->second->m_MaleDrop.end(); ++itItem )
            {
                listMaleItem.push_back( itItem->m_itemInfo );
            }
        }
    }

    if ( pDrop->m_nOptionalNum > 0 && pDrop->m_nOptionalNum <  notMustCount && notMustCount > 0 )
    {
        std::vector<CDropItemInfo> tempitemlist( pDrop->m_OptionalMaleDrop );
        std::map<unsigned int,CRubbishDropConfig*>::iterator itRubbish = rubbishdrop.begin();
        for ( ; itRubbish != rubbishdrop.end(); ++itRubbish )
        {
            tempitemlist.insert( tempitemlist.end(),itRubbish->second->m_MaleDrop.begin(), itRubbish->second->m_MaleDrop.end() );
        }
        
        std::vector<CDebrisDropInfo> tempdebrislist( pDrop->m_OptionalDebrisDrop );

        for ( unsigned int i = 0; i < pDrop->m_nOptionalNum; ++ i )
        {
            std::vector<int> randomVec;
            
            std::vector<CDropItemInfo>::iterator itItem = tempitemlist.begin();
            for ( ; itItem != tempitemlist.end(); ++itItem )
            {
                 randomVec.push_back( itItem->m_nRate  );
            }

            std::vector<CDebrisDropInfo>::iterator itDebris = tempdebrislist.begin();
            for ( ; itDebris != tempdebrislist.end(); ++itDebris )
            {
                randomVec.push_back( itDebris->m_nDropRate );
            }

            if ( !randomVec.empty() )
            {
                int nRandomIndex = RateRandom( randomVec );
                int nMiddleIndex = tempitemlist.size() -1;
                if ( nRandomIndex <= nMiddleIndex )
                {
                    listMaleItem.push_back( tempitemlist[nRandomIndex].m_itemInfo );
                    tempitemlist.erase(std::find(tempitemlist.begin(), tempitemlist.end(), tempitemlist[nRandomIndex]));
                }
                else
                {
                    DebrisReward reward;
                    reward.nDebrisID = tempdebrislist[nRandomIndex-1-nMiddleIndex].m_nDebrisID;
                    reward.nDebrisCount = tempdebrislist[nRandomIndex-1-nMiddleIndex].m_nDebrisCount;
                    listDebris.push_back( reward);
                    tempdebrislist.erase(std::find(tempdebrislist.begin(), tempdebrislist.end(), tempdebrislist[nRandomIndex-1-nMiddleIndex]) );
                }
            }
        }
    }
}

void CDungeonDataMgr::AddParcloseGradeExpConfig( int nMusicLevel, const std::map<int,int> & exp )
{
	std::map<int,std::map<int,int> >::iterator it = m_ParcloseExpConfig.find( nMusicLevel );
	if ( it == m_ParcloseExpConfig.end() )
	{
		m_ParcloseExpConfig[ nMusicLevel ] = exp;
	}
}

void CDungeonDataMgr::AddParcloseBaseExpConfig( unsigned int nFailExp, unsigned int nWinBaseExp )
{
	m_nParcloseFailExp = nFailExp;
	m_nParcloseWinBaseExp = nWinBaseExp;
}

unsigned int CDungeonDataMgr::GetParcloseCalcExp(bool isWin, int nGrade, unsigned int nMusicLevel)
{
    if (isWin)
    {
        return m_nParcloseWinBaseExp + GetDungeonGradeExp(nGrade, nMusicLevel);
    }

    return m_nParcloseFailExp;
}

unsigned int CDungeonDataMgr::GetDungeonGradeExp(int nGrade, unsigned int nMusicLevel)
{
    std::map<int,std::map<int,int> >::iterator it = m_ParcloseExpConfig.find(nMusicLevel);
    if (it != m_ParcloseExpConfig.end())
    {
        std::map<int,int>::iterator iter = it->second.find(nGrade);
        if (iter != it->second.end())
        {
            return iter->second;
        }
    }

    return 0;
}

CBuyCrystalConfig * CDungeonDataMgr::GetBuyCrystalConfig( unsigned int nIndex )
{
	BuyCrystalConfigTable::iterator it = m_BuyCrystalConfig.find( nIndex );
	if ( it != m_BuyCrystalConfig.end() )
	{
		return &it->second;
	}

	return NULL;
}

unsigned int CDungeonDataMgr::GetFavorableBuyVitCount()
{
    return m_CrystalConfig.m_nFavorableBuyVitCount;
}

void CDungeonDataMgr::AddPublicityPictureConfig( CPublicityPictureConfig & picture )
{
    m_PublicityPictureConfig.push_back( picture );
}

unsigned int CDungeonDataMgr::GetNextDayResetTime() const
{
    return m_nNextDayResetTime;
}

unsigned int CDungeonDataMgr::OnceGiveVitNum() const
{
    return m_nOnceGiveVitNum;
}

CItem CDungeonDataMgr::GetQuickFinishItem()
{
    return m_QuickFinishCostItem;
}

unsigned int CDungeonDataMgr::GetDailyGetVitMaxNum() const
{
    return m_nDailyGetVitMaxNum;
}

bool CDungeonDataMgr::QucikFinishOpenToNotVip()
{
    return m_bQuickFinishOpenToNotVip;
}

void CDungeonDataMgr::AddGiveGetVitConfig(unsigned int nOnceGiveVitNum, unsigned int nResetTime, unsigned int nGetVitMaxNum)
{
    m_nOnceGiveVitNum = nOnceGiveVitNum;
    m_nNextDayResetTime = nResetTime;
    m_nDailyGetVitMaxNum = nGetVitMaxNum;
}

void CDungeonDataMgr::AddQuickFinishConfig(bool bOpenToNotVip, const CItem & costItem)
{
    m_bQuickFinishOpenToNotVip = bOpenToNotVip;
    m_QuickFinishCostItem = costItem;
}





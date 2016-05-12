#include "RoomRewardDataMgr.h"
#include "../../socket/Log.h"
#include "../util/CommonFunc.h"


CCountRandCollect::CCountRandCollect()
{

}
CCountRandCollect::~CCountRandCollect()
{

}
//��ӿ������Ʒ
bool CCountRandCollect::AddCountRate(int nCount, int nRate)
{
	if (nRate <= 0 || nCount < 0)
	{
		WriteLog(LOGLEVEL_WARNING, "Warning: CCountRandCollect::AddCountRate. nCount(%d), nRate(%d)", nCount, nRate);
		return false;
	}
	m_CountVec.push_back(nCount);
	m_RateVec.push_back(nRate);
	return true;
}
//��ȡ�����Ʒ
int CCountRandCollect::RandCount()
{
	int nResCount = 0;

	if (!m_RateVec.empty())
	{
		int nPos = RateRandom(m_RateVec);
		if (nPos >= 0 && nPos < (int)m_RateVec.size())
		{
			if (nPos < (int)m_CountVec.size())
			{
				nResCount = m_CountVec[nPos];
			}
			else
			{
				WriteLog(LOGLEVEL_ERROR, "Error: CCountRandCollect::RandCount. Data Size Error. Pos(%d), m_RateVec(%d), m_CountVec(%d)",
					nPos, m_RateVec.size(), m_CountVec.size());
			}
		}
		else
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CCountRandCollect::RandCount. Data Size Error. Pos(%d), m_RateVec(%d), m_CountVec(%d)",
				nPos, m_RateVec.size(), m_CountVec.size());
		}
	}

	return nResCount;
}

CRoomRewardDataMgr::CRoomRewardDataMgr()
{
}

CRoomRewardDataMgr::~CRoomRewardDataMgr()
{
}

CRoomRewardDataMgr & CRoomRewardDataMgr::Instance()
{
	static CRoomRewardDataMgr s_Instance;
	return s_Instance;
}

//��ʼ����������
void CRoomRewardDataMgr::InitBaseData(const char * szTitle, const char * szContent)
{
	m_strTitle = szTitle;
	m_strContent = szContent;
}

//��������Ʒ����
bool CRoomRewardDataMgr::AddRandCount(int nCount, int nRate)
{
	return m_CountRC.AddCountRate(nCount, nRate);
}
//��ӱص������Ʒ
void CRoomRewardDataMgr::AddCertainItem(const char * szMaleReward, const char * szFemaleReward, int nRate)
{
	_AddRandItem( szMaleReward, nRate, m_MaleCertainIRC );
	_AddRandItem( szFemaleReward, nRate, m_FemaleCertainIRC );
}

//�����������Ʒ
void CRoomRewardDataMgr::AddModeRandItem(int nMusicMode, const char * szMaleReward, const char * szFemaleReward, int nRate)
{
	if ( nMusicMode >= MusicMode_None && nMusicMode < MusicMode_Max )
	{
		if ( nMusicMode == MusicMode_None )
		{
			for ( int i = MusicMode_None; i < MusicMode_Max; ++i )
			{
				_AddRandItem( szMaleReward, nRate, m_arMaleRandomIRC[i] );
				_AddRandItem( szFemaleReward, nRate, m_arFemaleRandomIRC[i] );
			}
		}
		else
		{
			_AddRandItem( szMaleReward, nRate, m_arMaleRandomIRC[nMusicMode] );
			_AddRandItem( szFemaleReward, nRate, m_arFemaleRandomIRC[nMusicMode] );
		}
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CRoomRewardDataMgr::AddRandItem. ModeError(%d)(%s)(%s)", nMusicMode, szMaleReward, szFemaleReward );
	}
}

//��ȡ��������Ʒ��
int CRoomRewardDataMgr::GetItemRewardCount()
{
	return m_CountRC.RandCount();
}

//��ȡ�ص���Ʒ�б�
void CRoomRewardDataMgr::GetCertainItem(bool bIsMale, std::list< CItem > & rItemList)
{
	CItem randomItem;
	CItemRandCollect& rIRC = ( bIsMale ? m_MaleCertainIRC : m_FemaleCertainIRC );
	if ( rIRC.RandSingleItem( randomItem ) )
	{
		rItemList.push_back( randomItem );
	}
}

//��ȡ�����Ʒ�б�
void CRoomRewardDataMgr::GetRandomItem(int nMusicMode, bool bIsMale, int nNum, std::list< CItem > & rItemList)
{
	if (nMusicMode >= MusicMode_None && nMusicMode < MusicMode_Max)
	{
		CItemRandCollect& rIRC = ( bIsMale ? m_arMaleRandomIRC[nMusicMode] : m_arFemaleRandomIRC[nMusicMode] );
		rIRC.RandMultiItem( rItemList, nNum );
	}
}

//��ȡ�ʼ�����
const std::string & CRoomRewardDataMgr::GetMailTitle()
{
	return m_strTitle;
}

//��ȡ�ʼ�����
const std::string & CRoomRewardDataMgr::GetMailContent()
{
	return m_strContent;
}

//�����������Ʒ
void CRoomRewardDataMgr::_AddRandItem(const char * szReward, int nRate, CItemRandCollect & rIRC)
{
	if ( nRate > 0 )
	{
		std::list< CItem > rewardItemList;
		if ( TransferItemList( szReward, rewardItemList ) )
		{
			if ( rewardItemList.size() == 1 )
			{
				std::list< CItem >::iterator it = rewardItemList.begin();
				rIRC.AddRandItem( *it, nRate );
			}
			else
			{
				WriteLog(LOGLEVEL_ERROR, "Error: CRoomRewardDataMgr::_AddRandItem. Check item list size error. itemlist=%s", szReward );
			}
		}
		else
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CRoomRewardDataMgr::_AddRandItem. Transfer item list error. itemlist=%s", szReward );
		}
	}
}


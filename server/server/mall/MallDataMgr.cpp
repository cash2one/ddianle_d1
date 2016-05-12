#include "MallDataMgr.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../socket/ParamPool.h"
#include "../../socket/Log.h"
#include "../util/CommonFunc.h"

#include <stdlib.h>

extern void SplitString(std::string str, std::string pattern, std::vector< std::string > &rStrVec);
extern void TrimSpace(string& str);

CRecommendEuipment::CRecommendEuipment()
:m_nIndex(0)
,m_strName("")
{

}

CRecommendEuipment::~CRecommendEuipment()
{

}

void CRecommendEuipment::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddStringW(m_strName.c_str());

	IOBuff.AddUShort((unsigned short)m_listEquipments.size());
	for (std::list<itemtype_t>::iterator itr = m_listEquipments.begin(); itr != m_listEquipments.end(); ++itr) {
		IOBuff.AddUInt(*itr);
	}
}

CMallDataMgr::CMallDataMgr()
	: m_experienceCardTimesPerDay(0)
	, m_dayCanExperienced(0)
	, m_experienceCardItemType(0)
{
}

CMallDataMgr::~CMallDataMgr()
{
	for (RecommendEquiptment_t::iterator itr = m_vecMaleRecommendEquipment.begin(); itr != m_vecMaleRecommendEquipment.end(); ++itr) {
		delete *itr, *itr = NULL;
	}
	m_vecMaleRecommendEquipment.clear();

	for (RecommendEquiptment_t::iterator itr = m_vecFemaleRecommendEquipment.begin(); itr != m_vecFemaleRecommendEquipment.end(); ++itr) {
		delete *itr, *itr = NULL;
	}
	m_vecFemaleRecommendEquipment.clear();
}

CMallDataMgr& CMallDataMgr::Instance()
{
	static CMallDataMgr s_mallDataMgr;

	return s_mallDataMgr;
}

bool CMallDataMgr::Init()
{
	bool bValid = _CheckEquipmentData(m_vecMaleRecommendEquipment);
	if (bValid) {
		return _CheckEquipmentData(m_vecFemaleRecommendEquipment);
	}

	return false;
}

const char * CMallDataMgr::SerializeName()
{
	return "MallDataMgr";
}

void CMallDataMgr::SerializeRecommendEquipments(CParamPool & IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_vecMaleRecommendEquipment.size());
	for (std::vector<CRecommendEuipment *>::iterator itr = m_vecMaleRecommendEquipment.begin(); itr != m_vecMaleRecommendEquipment.end(); ++itr) {
		(*itr)->doEncode(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)m_vecFemaleRecommendEquipment.size());
	for (std::vector<CRecommendEuipment *>::iterator itr = m_vecFemaleRecommendEquipment.begin(); itr != m_vecFemaleRecommendEquipment.end(); ++itr) {
		(*itr)->doEncode(IOBuff);
	}
}

void CMallDataMgr::AddMaleRecommedEquipment(int nIndex, std::string strName, std::string strMaleEquipment)
{
	CRecommendEuipment *pInfo = new CRecommendEuipment;	
	if (pInfo != NULL) {
		pInfo->m_nIndex = nIndex;
		pInfo->m_strName = strName;

		std::list<itemtype_t> listMaleItems;
		_TransferToItem(strMaleEquipment, listMaleItems);

		if (listMaleItems.size() > 0) {
			pInfo->m_listEquipments = listMaleItems;
			m_vecMaleRecommendEquipment.push_back(pInfo);
		}
	}
}

void CMallDataMgr::AddFemaleRecommedEquipment(int nIndex, std::string strName, std::string strFemaleEquipments)
{
	CRecommendEuipment *pInfo = new CRecommendEuipment;	
	if (pInfo != NULL) {
		pInfo->m_nIndex = nIndex;
		pInfo->m_strName = strName;

		std::list<itemtype_t> listFemaleItems;
		_TransferToItem(strFemaleEquipments, listFemaleItems);

		if (listFemaleItems.size() > 0) {
			pInfo->m_listEquipments = listFemaleItems;
			m_vecFemaleRecommendEquipment.push_back(pInfo);
		}
	}
}

void CMallDataMgr::_TransferToItem(std::string strItems, std::list<unsigned short> & listItems)
{
	if (strItems.size() > 0) {
		std::vector<std::string> vecStrItems;
		::SplitString(strItems, "|", vecStrItems);

		for (std::vector<std::string>::iterator itr = vecStrItems.begin(); itr != vecStrItems.end(); ++itr) {
			::TrimSpace(*itr);
			listItems.push_back((unsigned short)atoi(itr->c_str()));
		}
	}
}

void CMallDataMgr::_TransferToItem(std::string strItems, std::list<unsigned int> & listItems)
{
    if (strItems.size() > 0) {
        std::vector<std::string> vecStrItems;
        ::SplitString(strItems, "|", vecStrItems);

        for (std::vector<std::string>::iterator itr = vecStrItems.begin(); itr != vecStrItems.end(); ++itr) {
            ::TrimSpace(*itr);
            listItems.push_back((unsigned int)atoi(itr->c_str()));
        }
    }
}

bool CMallDataMgr::_CheckEquipmentData(std::vector<CRecommendEuipment *> & vecRecommendEquipment)
{
	for (std::vector<CRecommendEuipment *>::iterator itr = vecRecommendEquipment.begin(); itr != vecRecommendEquipment.end(); ++itr) {
		for (std::list<itemtype_t>::iterator itri = (*itr)->m_listEquipments.begin(); itri != (*itr)->m_listEquipments.end(); ++itri) {
			ItemConfig *pItemInfo1 = ConfigManager::Instance().GetItemConfigManager().GetByID(*itri);
			if (pItemInfo1 == NULL) {
				return false;
			}
			std::list<itemtype_t>::iterator itrii = itri;
			for (std::list<itemtype_t>::iterator itrj = ++itrii; itrj != (*itr)->m_listEquipments.end(); ++itrj) {
				ItemConfig *pItemInfo2 = ConfigManager::Instance().GetItemConfigManager().GetByID(*itrj);
				if (pItemInfo2 == NULL) {
					return false;
				}
				if (pItemInfo1->IsEquip() && pItemInfo2->IsEquip()) {
					if (pItemInfo1->m_anType[1] != pItemInfo2->m_anType[1]) {
						continue;
					} else {
						if (pItemInfo1->IsBadge()) {
							return false;
						} else if(pItemInfo1->IsCloth()) {
							if (pItemInfo1->m_anType[2] == pItemInfo2->m_anType[2]) {
								return false;
							}
						}
					}
				} else {
					return false;
				}
			}
		}
	}

	return true;
}

void CMallDataMgr::SetMallTip(const char *strTipTitle, const char *strTip)
{
	if (strTipTitle != NULL)
	{
		m_strMallTipTitle = strTipTitle;
	}
	if (strTip != NULL)
	{
		m_strMallTip = strTip;
	}
}

void CMallDataMgr::SetExperienceCardProp(unsigned short timesPerDay, unsigned short dayCanExperienced, 
	itemtype_t experienceCardItemType, std::string &experienceTypes)
{ 
	m_experienceCardTimesPerDay = timesPerDay;
	m_dayCanExperienced = dayCanExperienced;
	m_experienceCardItemType = experienceCardItemType;
	std::vector<unsigned int> vecExperienceType;
	TransferList(experienceTypes.c_str(), vecExperienceType);

	for (size_t i = 0; i < vecExperienceType.size(); ++i)
	{
		m_vecExperienceType.push_back((unsigned char)vecExperienceType[i]);
	}
}



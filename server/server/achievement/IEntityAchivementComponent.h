#ifndef		__IENTITYACHIEVEMENTCOMPONENT_H__
#define		__IENTITYACHIEVEMENTCOMPONENT_H__

#include "../../datastructure/DataStruct_Base.h"

class CEntityAttributeComponent;
class CEntityQuestComponent;
class CEntityChatComponent;
class CEntityNetComponent;
class CEntityItemComponent;
class CEntityCheckInComponent;

class IEntityAchievementComponent
{
public:
	IEntityAchievementComponent();
	virtual ~IEntityAchievementComponent();

public:
	virtual CEntityAttributeComponent *		GetRoleAttr() const = 0;
	virtual CEntityQuestComponent *			GetRoleQuest() const = 0;
	virtual CEntityChatComponent	*		GetRoleChat() const = 0;
	virtual CEntityNetComponent *			GetRoleNet() const = 0;
	virtual CEntityItemComponent*			GetRoleItem()const = 0;
	virtual CEntityCheckInComponent*       GetRoleCheck() const=0;
	unsigned int GetHistoryTitleCount() const {return (unsigned int)m_listTitle.size();}
	unsigned int GetHistoryAchievementCount() const {return (unsigned int)m_listAchievement.size();}
	unsigned int GetTotalScore() const;
	unsigned int GetSmallSpeakersUseCountInSevenDays() const;
	unsigned int GetBigSpeakersUseCountInSevenDays() const;
	unsigned int GetSendFlowersCountInSevenDays() const;
public:
	void SetFightResult(unsigned char nMode, unsigned char nDifficulty, unsigned );

public:
	// ��õĳƺ�
	list<CTitleInfo>			m_listTitle;							// �Ѿ���õĳƺ�
	list<CAchievementInfo>		m_listAchievement;						// �Ѿ���ɵĳɾ�
	unsigned int				m_nScore;								// �ɾͻ���
	// ��Ʒ���
	unsigned int				m_nHistoryClothNum;						// �ۼ�ӵ�е�����ʱЧ���·����� 
	unsigned int				m_nHistoryWishItemNum;					// �ۼƼ���Ը��������Ʒ����
	unsigned int				m_nHistoryBadageNum;					// �ۼ�ӵ�еĻ�����
	unsigned int				m_nGainWishItemNum;						// ��õ�Ը����Ʒ���� 
	unsigned int				m_nHistoryUseBouquetNum;				// ʹ�û���������
	CWeeklyEvent				m_BigSpeakersUseCountWeekly;			// һ����ʹ�ô����ȵ�����
	CWeeklyEvent				m_SmallSpeakersUseCountWeekly;			// һ����ʹ��С���ȵ�����
	CWeeklyEvent				m_SendFlowersCountWeekly;				// һ�����ͻ�����
	// �������
	unsigned int				m_nHistoryTotalScore;					// �ۼ�����÷֣�����ģʽ
	unsigned short				m_nNumWinBetween2To4Persons;			// 2�����ϵ�һ��
	unsigned short				m_nNumWinIn4Persons;					// 4���е�һ��
	unsigned short				m_nHistoryTaikoComboMusicNum;			// �ۼ�̫��ģʽ��missȫ��������
	unsigned short				m_nTaikoComboNum;						// ̫��ģʽcombo��
	unsigned int				m_nAutoMatchWithOppositeSexNum;			// ͨ������ƥ��ģʽ������һ����Ϸ������

	map<int, map<int, CFightResult > > m_mapFightResult;				// <ģʽ <�Ѷȣ�ʵ��>>.������ʷս������
};


#endif

//end file



#ifndef	__GLOBAL_ACTIVITY_DATA_H__
#define __GLOBAL_ACTIVITY_DATA_H__

//���ص�ȫ������
class CGlobalActivityData
{
protected:
	CGlobalActivityData();
	virtual ~CGlobalActivityData();

public:
	static CGlobalActivityData & Instance();
public:
	//���Ӷ��⾭�齱������
	void IncreaseExtraExpRate(unsigned short nDeltaExpRate);
	//���ٶ��⾭�齱������
	void DecreaseExtraExpRate(unsigned short nDeltaExpRate);
	//��ȡ���⾭�齱�����ʣ��ٷֱ�
	unsigned int GetExtraExpRate(){return m_nExtraExpRate;}
private:
	unsigned int			m_nExtraExpRate;	//����⾭�齱�����ʣ��ٷֱ�
};

#endif



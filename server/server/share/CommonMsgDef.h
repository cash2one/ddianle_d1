#ifndef __COMMONMSGDEF_H__
#define __COMMONMSGDEF_H__

///////////////////////////////////////////////////////////////////////////////////////
//����0x0000 - 0x00FF֮�����Ϣֵ�������ײ�ʹ��
//All messages between 0x0000 to 0x00FF are reserved, please do not use them in game!!!
///////////////////////////////////////////////////////////////////////////////////////
#define MSG_TYPE_ACCOUNT			0x0100	//ע�������Ϣ
#define MSG_TYPE_LOGIN				0x0200
#define MSG_TYPE_CHAT				0x0300	//
#define MSG_TYPE_SERVER				0x0400

#define MSG_TYPE_PLAYER				0x0500	//��ɫ���Լ���Ʒ����
#define MSG_TYPE_QUEST				0x0600	//������Ϣ

#define MSG_TYPE_EXCHANGE		    0x0800	//������Ϣ
#define MSG_TYPE_FRIEND				0x0900	//������Ϣ
#define MSG_TYPE_FAMILY				0x1000	//������Ϣ

#define MSG_TYPE_MAIL				0x1200	//�ʼ���Ϣ
#define MSG_TYPE_MALL				0x1300	//�̳���Ϣ
#define MSG_TYPE_ROOM				0x1400	//������Ϣ
#define MSG_TYPE_MARRIAGE		    0x1500	//������Ϣ
#define MSG_TYPE_RANKING			0X1600	//���а���Ϣ
#define MSG_TYPE_ACHIEVEMENT	    0X1700	//�ɾ�ϵͳ
#define MSG_TYPE_VIP				0X1800	//VIPϵͳ
#define MSG_TYPE_CHECKIN			0X1900	//ǩ��ϵͳ
#define MSG_TYPE_ACTIVITY			0X1A00	//�
#define MSG_TYPE_DANCEGROUP			0X1B00	//����
#define MSG_TYPE_PHOTOSTUDIO		0X1C00	//��Ӱ��
#define MSG_TYPE_AMUSEMENT			0X1D00	//���ų���
#define MSG_TYPE_LIVEROOM			0X1E00	//ֱ������
#define MSG_TYPE_PLATFORMPRIVILEGE  0X1F00  //ƽ̨��Ȩϵͳ
#define MSG_TYPE_DUNGEON            0X2000  //����
#define MSG_TYPE_NOTIFICATION		0X2100	//����֪ͨ��Ϣ
#define MSG_TYPE_MonthCard          0X2200  //�¿�
#define MSG_TYPE_Activeness         0X2300  //��Ծ��ϵͳ
#define MSG_TYPE_PET                0x2400  // ����ϵͳ
#define MSG_TYPE_MEDAL              0x2500  // ѫ��ϵͳ
#define MSG_TYPE_HANDBOOK           0x2600  // ͼ��ϵͳ
#define MSG_TYPE_HORSE              0x2700  // ����ϵͳ
#define MSG_TYPE_DANCEGROUP_SERVER  0x2800  // �����ڲ���Ϣ
#define MSG_TYPE_FAIRYLAND          0x2900  // �����ؾ�
#define MSG_TYPE_NEWROOM            0x2A00  // �·�����Ϣ
#define MSG_TYPE_NEWROOMBROADCAST   0x2B00  // ����㲥��Ϣ

#define MSG_TYPE_GM					0X3000	//GM��Ϣ
#define MSG_TYPE_EXTERNAL			0X3100	//External��Ϣ


/////////////////////////////////////////////////////////////////////////

#endif



//file end


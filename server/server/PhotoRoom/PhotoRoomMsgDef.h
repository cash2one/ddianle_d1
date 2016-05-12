#ifndef		__PHOTOSTUDIOMSGDEF_H__
#define		__PHOTOSTUDIOMSGDEF_H__


#include "../share/CommonMsgDef.h"

enum EMSG_PhotoStudio
{
	MSG_PhotoStudio_Begin = MSG_TYPE_PHOTOSTUDIO,

	MSG_C2S_CreatePhotoRoom,						//������Ӱ����
	MSG_S2C_CreatePhotoRoomFail,					//������Ӱ������

	MSG_C2S_ClosePhotoRoom,							//�ر���Ӱ��
	//MSG_S2C_ClosePhotoRoomSuc,					//�ر���Ӱ��ɹ�
	MSG_S2C_ClosePhotoRoomFail,						//�ر���Ӱ��ʧ��

	MSG_C2S_TryEnterMyPhotoRoom,					//���Խ����Լ���������Ӱ������(�ɹ�����MSG_S2C_EnterPhotoRoomSuc)
	MSG_S2C_TryEnterMyPhotoRoomFail,				//���Խ����Լ���������Ӱ��ʧ��

	MSG_C2S_EnterPhotoRoom,							//������뷿��
	MSG_S2C_EnterPhotoRoomSuc,						//������Ӱ����ɹ�
	MSG_S2C_EnterPhotoRoomFail,						//������Ӱ����ʧ��
	MSG_S2C_EnterPhotoRoomPlayer,					//����ҽ��뷿��

	MSG_C2S_QuitPhotoRoom,							//�˳���Ӱ��
	MSG_S2C_QuitPhotoRoomSuc,						//�˳���Ӱ��ɹ�
	MSG_S2C_QuitPhotoRoomFail,						//�˳���Ӱ��ʧ��
	MSG_S2C_PlayerQuitPhotoRoom,					//������˳�����Ӱ��

	MSG_C2S_InvitePhotoRoomPlayer,					//������Ӱ�����
	MSG_S2C_InvitePhotoRoomPlayerSuc,				//������Ӱ����ҳɹ�
	MSG_S2C_InvitePhotoRoomPlayerFail,				//������Ӱ�����ʧ��
	MSG_S2C_PhotoRoomInviteeNotification,			//֪ͨ��������
	MSG_C2S_PhotoRoomInviteeResponse,				//�������߻�Ӧ
	MSG_S2C_PhotoRoomInviteeResponseFail,			//�������߻�Ӧ����

	MSG_C2S_KickPhotoRoomPlayer,					//����
	MSG_S2C_KickPhotoRoomPlayerSuc,					//���˳ɹ�
	MSG_S2C_KickPhotoRoomPlayerFail,				//����ʧ��

	MSG_C2S_ChangeRoomPlayerState,					//���״̬�ı�
	MSG_S2C_ChangeRoomPlayerStateSuc,				//�ı�״̬�ɹ�		
	MSG_S2C_ChangeRoomPlayerStateFail,				//�ı�״̬ʧ��	
	MSG_S2C_PhotoRoomPlayerChangeStateNotification,	//֪ͨ״̬�ı�

	MSG_C2S_ChangePhotoRoomState,					//�ı���Ӱ��״̬
	MSG_S2C_ChangePhotoRoomStateSuc,				//�ı���Ӱ��״̬�ɹ�
	MSG_S2C_ChangePhotoRoomStateFail,				//�ı���Ӱ��״̬ʧ��
	MSG_S2C_PhotoRoomStateChangedNotification,		//֪ͨ��Ӱ��״̬�ı�

	MSG_C2S_ChangePhotoRoomAction,					//�ı���Ӱ�ﶯ��
	MSG_S2C_ChangePhotoRoomActionSuc,				//�ı���Ӱ�ﶯ���ɹ�
	MSG_S2C_ChangePhotoRoomActionFail,				//�ı���Ӱ�ﶯ��ʧ��
	MSG_S2C_PhotoRoomActionNotification,			//֪ͨ��Ӱ�ﶯ���ı�

	MSG_C2S_ChangePhotoRoomBG,						//�ı���Ӱ�ﱳ��
	MSG_S2C_ChangePhotoRoomBGSuc,					//�ı���Ӱ�ﱳ���ɹ�
	MSG_S2C_ChangePhotoRoomBGFail,					//�ı���Ӱ�ﱳ��ʧ��
	MSG_S2C_PhotoRoomBGNotification,				//֪ͨ��Ӱ�ﱳ���ı�

	MSG_S2C_PhotoRoomExpiredNotification,			//����ʣ��ʱ������

	MSG_C2S_GetMyPhotoRoomInfo,						//��ȡ�ҵ���Ӱ��ķ�����Ϣ
	MSG_S2C_GetMyPhotoRoomsSuc,						//���ػ�ȡ�ҵ���Ӱ��ķ�����Ϣ�ɹ�
	MSG_S2C_GetMyPhotoRoomsFail,					//���ػ�ȡ�ҵ���Ӱ��ķ�����Ϣʧ��

	MSG_S2C_PhotoRoomClosedNotification,			//��Ӱ��ر�����

	MSG_C2S_RenewPhotoRoom,							//��Ӱ������
	MSG_S2C_RenewPhotoRoomSuc,						//��Ӱ�����ѳɹ�
	MSG_S2C_RenewPhotoRoomFail,						//��Ӱ������ʧ��

    MSG_C2S_PressPhotoButtonAction,				    //ʹ�����ఴť
};



#endif






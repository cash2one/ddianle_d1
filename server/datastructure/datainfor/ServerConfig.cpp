#include "ServerConfig.h"
#include "../../socket/Ini.h"
#include "../Macro_Define.h"
#include "../DanceGroupInfoDef.h"

ServerConfig g_ServerConfig;

#define DEFAULT_MAIL_TITLE_LENGTH       48      // �ʼ����ⳤ��
#define DEFAULT_MAIL_CONTENT_LENGTH     400     // �ʼ����ݳ���
#define DEFAULT_BIRTHDAY_LENGTH         8       // ���ճ���
#define DEFAULT_EGG_WORDS_LENGTH        200     // ʱ�⵰���Գ���
#define DEFAULT_PET_NAME_LENGTH         24      // Ĭ�ϳ����ǳƳ���
#define DEFAULT_ROOM_NAME_LENGTH        50      // �������ֳ���
#define DEFAULT_AMUSE_ROOM_NAME_LENGTH  50      // ���ų�����������Ĭ�ϳ���
#define DEFAULT_GROUP_BASE_MEMBER_COUNT 30      // ���Ż�����Ա����
#define DEFAULT_GROUP_LEVEL_UP_ADDITION 5       // ����ÿ�����ӳ�Ա����

ServerConfig::ServerConfig()
{

}

ServerConfig::~ServerConfig()
{

}

bool ServerConfig::Load(std::string& fileName)
{
    CIni configIni;
    if( !configIni.Load(fileName.c_str() ) )
    {
        return false;
    }

    m_MaxNameLimit = configIni.GetInt("Limit", "MaxNameLength", MAX_NAME_LEN);
    m_MaxTitleLimit = configIni.GetInt("Limit", "MaxTitleLength", MAX_TITLE_LEN);
    m_FamilyLength = configIni.GetInt("Limit", "MaxFamilyLength", MAX_FAMILY_NAME);
    m_FamilyNameLength = configIni.GetInt("Limit", "MaxFamilyNameLength", FAMILY_NAME_LEN);
    m_MaxWeddingRoomLimit = configIni.GetInt("Limit", "MaxWeddingRoomLimit", DEFAULT_ROOM_NAME_LENGTH);
    m_SignatureLength = configIni.GetInt("Limit", "SignatureLength", MAX_SIGNATURE_LEN);
    m_DanceGroupNameLength = configIni.GetInt("Limit", "DanceGroupNameLength", (int)cMAX_DANCE_GROUP_NAME_LEN);
    m_DanceGroupAnnouncementLength = configIni.GetInt("Limit", "DanceGroupAnnouncementLength", (int)cMAX_DANCE_GROUP_ANNOUNCEMENT_LEN);
    m_DanceGroupSummary = configIni.GetInt("Limit", "DanceGroupSummary", (int)cMAX_DANCE_GROUP_PROFILE_LEN);
    m_DanceGroupTitleName = configIni.GetInt("Limit", "DanceGroupTitleName", (int)cMAX_DANCE_GROUP_TITILE_NAME_LEN);

    m_nMaxMailTitleLength = configIni.GetInt( "Limit", "MaxMailTitleLength", DEFAULT_MAIL_TITLE_LENGTH );
    m_nMaxMailContentLength = configIni.GetInt( "Limit", "MaxMailContentLength", DEFAULT_MAIL_CONTENT_LENGTH );

    m_nMaxRoomNameLength = configIni.GetInt( "Limit", "MaxRoomNameLength", DEFAULT_ROOM_NAME_LENGTH );
    m_nMaxAmuseRoomNameLength = configIni.GetInt( "Limit", "MaxAmuseRoomNameLength", DEFAULT_AMUSE_ROOM_NAME_LENGTH );

    m_nMaxDanceGroupBaseMemberCount = configIni.GetInt( "Limit", "MaxDanceGroupBaseMemberCount", DEFAULT_GROUP_BASE_MEMBER_COUNT );
    m_nMaxDanceGroupLevelUpAddition = configIni.GetInt( "Limit", "MaxDanceGroupLevelUpAddition", DEFAULT_GROUP_LEVEL_UP_ADDITION );

    m_nMaxEggWordsLength = configIni.GetInt( "Limit", "MaxEggWordsLength", DEFAULT_EGG_WORDS_LENGTH );

    m_nMaxPetNameLength = configIni.GetInt( "Limit", "MaxPetNameLength", DEFAULT_PET_NAME_LENGTH );

    // ���ղ���Ҫ���������ļ����̶�Ϊ: MAX_BIRTHDAY_LENGTH
    m_nMaxBirthdayLength = configIni.GetInt( "Limit", "MaxBirthdayLength", DEFAULT_BIRTHDAY_LENGTH );

    return true;
}

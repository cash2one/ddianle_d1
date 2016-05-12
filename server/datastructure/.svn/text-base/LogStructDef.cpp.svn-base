#include "LogStructDef.h"
#include "../socket/DBIMysql.h"
#include "../socket/Formatter.h"
#include <time.h>
#include <stdlib.h>

extern int g_nLine;

CPlayerActionLog::CPlayerActionLog()
:m_nRoleID(0)
, m_ePlayerAction(EPlayerAction_Unknown)
, m_nMoney(0)
, m_nCash(0)
{
}

CPlayerActionLog::~CPlayerActionLog()
{
}
void CPlayerActionLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    char smallBuff[16] = { 0 };
    sprintf( smallBuff, "%02d%d", g_nLine + 1, (int)m_ePlayerAction );
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_PlayerAction(role_id, action, money, cash, time) VALUES(%u, %d, %d, %u, %u)",
        m_nRoleID, (int)atoi(smallBuff), m_nMoney, m_nCash, (unsigned int)time(NULL));
}

CPlayerSwitchLineLog::CPlayerSwitchLineLog()
:m_nRoleID(0)
, m_eAction(EPlayerAction_Unknown)
, m_nLine(0)
{}

CPlayerSwitchLineLog::~CPlayerSwitchLineLog()
{}

void CPlayerSwitchLineLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleSwitchLine(roleid, action, line, time) VALUES(%u, %d, %d, %u)",
        m_nRoleID, (int)m_eAction, m_nLine, (unsigned int)time(NULL));
}

CRoleExpLog::CRoleExpLog()
:m_nRoleID(0)
, m_eAddExpCause(EAddExpCause_Unknown)
, m_nAddExp(0)
, m_nNewLevel(0)
, m_nNewExp(0)
{
}

CRoleExpLog::~CRoleExpLog()
{
}
void CRoleExpLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleExp(role_id, cause, add_exp, new_level, new_exp, time) VALUES(%u, %d, %u, %u, %u, %u)",
        m_nRoleID, (int)m_eAddExpCause, m_nAddExp, m_nNewLevel, m_nNewExp, (unsigned int)time(NULL));
}

CRoleQuestLog::CRoleQuestLog()
:m_nRoleID(0)
, m_nQuestID(0)
{
}

CRoleQuestLog::~CRoleQuestLog()
{
}
void CRoleQuestLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleQuest(role_id, quest_id, time) VALUES(%u, %u, %u)",
        m_nRoleID, m_nQuestID, (unsigned int)time(NULL));
}

CRoleAchievementLog::CRoleAchievementLog()
:m_nRoleID(0)
, m_eAchievementAction(EAchievementAction_Unknown)
, m_nAchievementID(0)
{
}

CRoleAchievementLog::~CRoleAchievementLog()
{
}
void CRoleAchievementLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleAchievement(role_id, action, achievement_id, time) VALUES(%u, %d, %u, %u)",
        m_nRoleID, (int)m_eAchievementAction, m_nAchievementID, (unsigned int)time(NULL));
}

CRoleDancingLog::CRoleDancingLog()
:m_nRoleID(0)
, m_eDancingAction(EDancingAction_Unknown)
, m_nScene(0)
, m_nMusicID(0)
, m_nMusicMode(0)
, m_nMusicLevel(0)
, m_nPlayerCount(0)
, m_nRank(0)
, m_nScore(0)
, m_nTeamMode(0)
, m_bIsDungeon(false)
{
}

CRoleDancingLog::~CRoleDancingLog()
{
}
void CRoleDancingLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleDancing "
        "(role_id, action, scene, team_mode, music_id, music_mode, music_level, "
        "player_count, rank, score, isdungeon, time) "
        "VALUES(%u, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %u)",
        m_nRoleID, (int)m_eDancingAction, m_nScene, m_nTeamMode, m_nMusicID, m_nMusicMode, m_nMusicLevel,
        m_nPlayerCount, m_nRank, m_nScore, (int)m_bIsDungeon, (unsigned int)time(NULL));
}

CRoleCheckInLog::CRoleCheckInLog()
:m_nRoleID(0)
, m_eCheckInAction(ECheckInAction_Unknown)
, m_bIsVip(false)
, m_nVipLevel(0)
, m_nDayIndex(0)
{
}

CRoleCheckInLog::~CRoleCheckInLog()
{
}
void CRoleCheckInLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleCheckIn1(role_id, action, dayindex, isvip,viplevel,time) VALUES(%u, %d, %u, %u, %u, %u)",
        m_nRoleID, (int)m_eCheckInAction, m_nDayIndex, (unsigned int)m_bIsVip, m_nVipLevel, (unsigned int)time(NULL));
}

CRoleSendLog::CRoleSendLog()
:m_nRoleID(0)
, m_eSendType(ESendType_Unknown)
, m_nItemType(0)
, m_nItemCount(0)
, m_nDuration(0)
, m_nTargetRoleID(0)
, m_nMailID(0)
{
}

CRoleSendLog::~CRoleSendLog()
{
}
void CRoleSendLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleSend(role_id, send_type, item_type, item_count, duration, target_role_id, mail_id, time) "
        "VALUES(%u, %d, %u, %d, %d, %u, %lld, %u)",
        m_nRoleID, (int)m_eSendType, m_nItemType, (int)m_nItemCount, m_nDuration, m_nTargetRoleID, m_nMailID, (unsigned int)time(NULL));
}

CRoleChangeBillLog::CRoleChangeBillLog()
:m_nRoleID(0)
, m_nChangeValue(0)
, m_eChangeBillCause(EChangeBillCause_Unknown)
, m_nNewPT(0)
{
}

CRoleChangeBillLog::~CRoleChangeBillLog()
{
}
void CRoleChangeBillLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytlogdb.log_RoleChangeBill(role_id, change_value, cause, new_pt, line, time) VALUES(%u, %d, %d, %u, %d, %u)",
        m_nRoleID, m_nChangeValue, (int)m_eChangeBillCause, m_nNewPT, g_nLine, (unsigned int)time(NULL));
}

CRoleChangeBindBillLog::CRoleChangeBindBillLog()
:m_nRoleID(0)
, m_nChangeValue(0)
, m_eChangeBindBillCause(EChangeBindBillCause_Unknown)
, m_nNewBindPT(0)
{
}

CRoleChangeBindBillLog::~CRoleChangeBindBillLog()
{
}

void CRoleChangeBindBillLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytlogdb.log_RoleChangeBindPoint(role_id, change_value, cause, new_bindpt, line, time) VALUES(%u, %d, %d, %u, %d, %u)",
        m_nRoleID, m_nChangeValue, (int)m_eChangeBindBillCause, m_nNewBindPT, g_nLine, (unsigned int)time(NULL));
}

CRoleChangeMoneyLog::CRoleChangeMoneyLog()
:m_nRoleID(0)
, m_nChangeValue(0)
, m_eChangeMoneyCause(EChangeMoneyCause_Unknown)
, m_nNewMoney(0)
{
}

CRoleChangeMoneyLog::~CRoleChangeMoneyLog()
{
}
void CRoleChangeMoneyLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytlogdb.log_RoleChangeMoney(role_id, change_value, cause, new_money, line, time) VALUES(%u, %d, %d, %u, %d, %u)",
        m_nRoleID, m_nChangeValue, (int)m_eChangeMoneyCause, m_nNewMoney, g_nLine, (unsigned int)time(NULL));
}


CRoleItemLog::CRoleItemLog()
:m_nRoleID(0)
, m_eItemAction(EItemAction_Unknown)
, m_nItemType(0)
, m_nOldCount(0)
, m_nOldDuration(0)
, m_nNewCount(0)
, m_nNewDuration(0)
, m_nAdditionalValue(0)
{
}

CRoleItemLog::~CRoleItemLog()
{
}
void CRoleItemLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleItem(role_id, action, item_column, item_id, item_type, "
        "old_count, old_duration, new_count, new_duration, additional_value, line, time) "
        "VALUES(%u, %d, %d, %lld, %u, %d, %d, %d, %d, %lld, %d, %u)",
        m_nRoleID, (int)m_eItemAction, (int)m_eItemColumn, m_nItemID, m_nItemType, (int)m_nOldCount, 
        m_nOldDuration, (int)m_nNewCount, m_nNewDuration, m_nAdditionalValue, g_nLine, (unsigned int)time(NULL));
}


CRoleChatLog::CRoleChatLog() :m_nRoleID(0), m_eChannel(eChatNone), m_bVoiceChat(false)
{

}

CRoleChatLog::~CRoleChatLog()
{

}

void CRoleChatLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    char *end = szSQL;
    sprintf(end, "INSERT INTO nsytsublogdb.log_RoleChat(role_id, channel, is_voice_chat, time, content) VALUES(%u, %d, %d, %u, '",
        m_nRoleID, (int)m_eChannel, (int)(m_bVoiceChat ? 1 : 0), (unsigned int)time(NULL));
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strText.c_str(), m_strText.size());
    sprintf(end, "')");
}

CRoleAmuseLog::CRoleAmuseLog() :m_nRoleID(0), m_nSceneID(0), m_nAction(EAction_None), m_nStayTime(0)
{

}

CRoleAmuseLog::~CRoleAmuseLog()
{

}

void CRoleAmuseLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleAmuse(role_id, action, scene, time, stay_time) VALUES(%u, %d, %d, %u, %d)",
        m_nRoleID, m_nAction, m_nSceneID, (unsigned int)time(NULL), m_nStayTime);
}

CRoleTimeEggLog::CRoleTimeEggLog() :
m_nRoleID(0), m_eEggAction(ETimeEggAction_None), m_nSceneID(0)
{

}

CRoleTimeEggLog::~CRoleTimeEggLog()
{

}

void CRoleTimeEggLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleTimeEgg(role_id, action, scene, time) VALUES(%u, %d, %d, %u)",
        m_nRoleID, m_eEggAction, m_nSceneID, (unsigned int)time(NULL));
}

//-----------------------------------------------CDanceGroupLog
CDanceGroupLog::CDanceGroupLog()
:m_eAction(EDanceGroupLog_None)
, m_nGroupId(0)
, m_strDGName("")
, m_nRoleId(0)
, m_strRoleName("")
, m_strParam("")
{

}

void CDanceGroupLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    char *end = szSQL;
    sprintf(end, "insert into nsytsublogdb.log_dancegroup(group_id, group_name, role_id, role_name, action, time, param) values( %u, '", m_nGroupId);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strDGName.c_str(), m_strDGName.size());
    sprintf(end, "', %u, '", m_nRoleId);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strRoleName.c_str(), m_strRoleName.size());
    sprintf(end, "', %d, %u, '", (int)m_eAction, (unsigned int)time(NULL));
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strParam.c_str(), m_strParam.size());
    sprintf(end, "')");
}

void CDanceGroupLog::ValuesToStr(EDanceGroupLogAction eAction, va_list ap)
{
    m_eAction = eAction;
    m_strParam = "";
    char szBuff[1024] = { 0 };
    switch (eAction)
    {
    case EDanceGroupLog_Create:
        vsnprintf(szBuff, sizeof(szBuff), "badge:%u", ap);
        break;
    case EDanceGroupLog_Destory:
        vsnprintf(szBuff, sizeof(szBuff), "badge:%u,effect:%u", ap);
        break;
    case EDanceGroupLog_MemberJoin:
        //vsnprintf( szBuff, sizeof( szBuff ), "", ap );
        break;
    case EDanceGroupLog_MemberLeave:
        //vsnprintf( szBuff, sizeof( szBuff ), "", ap );
        break;
    case EDanceGroupLog_MemberKicked:
        vsnprintf(szBuff, sizeof(szBuff), "roleId:%u,name:%s,title:%u", ap);
        break;
    case EDanceGroupLog_MemberContributionAdd:
        vsnprintf(szBuff, sizeof(szBuff), "addValue:%u,oldValue:%u,newValue:%u,type:%s", ap);
        break;
    case EDanceGroupLog_MemberContributionDec:
        vsnprintf(szBuff, sizeof(szBuff), "decValue:%u,oldValue:%u,newValue:%u,type:%s", ap);
        break;
    case EDanceGroupLog_MemberIntergralAdd:
        vsnprintf(szBuff, sizeof(szBuff), "addValue:%u,oldValue:%u,newValue:%u", ap);
        break;
    case EDanceGroupLog_HonourAdd:
        vsnprintf(szBuff, sizeof(szBuff), "addValue:%u,oldValue:%u,newValue:%u,type:%s", ap);
        break;
    case EDanceGroupLog_HonourDec:
        vsnprintf(szBuff, sizeof(szBuff), "decValue:%u,oldValue:%u,newValue:%u,type:%s", ap);
        break;
    case EDanceGroupLog_Levelup:
        vsnprintf(szBuff, sizeof(szBuff), "oldLevel:%d,newLevel:%d", ap);
        break;
    case EDanceGroupLog_ResConsume:
        vsnprintf(szBuff, sizeof(szBuff), "resName:%s,consumeValue:%u,oldValue:%u,newValue:%u,type:%s", ap);
        break;
    case EDanceGroupLog_ResDonate:
        vsnprintf(szBuff, sizeof(szBuff), "resId:%u,resName:%s,donateValue:%d,oldValue:%u,newValue:%u", ap);
        break;
    case EDanceGroupLog_LeaderChange:
        vsnprintf(szBuff, sizeof(szBuff), "newLeaderId:%u,newLeaderName:%s", ap);
        break;
    case EDanceGroupLog_MemberTitleChange:
        vsnprintf(szBuff, sizeof(szBuff), "oldTitle:%d,newTitle:%d", ap);
        break;
    case EDanceGroupLog_NameChange:
        vsnprintf(szBuff, sizeof(szBuff), "oldDGName:%s,newDGName:%s", ap);
        break;
    case EDanceGroupLog_Activity_PowerAdd:
        vsnprintf(szBuff, sizeof(szBuff), "addValue:%u,oldValue:%u,newValue:%u", ap);
        break;
    case EDanceGroupLog_ChallengeActiveAdd:
        vsnprintf(szBuff, sizeof(szBuff), "addValue:%u,oldValue:%u,newValue:%u,type:%s", ap);
        break;
    case EDanceGroupLog_ChallengeActiveDec:
        vsnprintf(szBuff, sizeof(szBuff), "decValue:%u,oldValue:%u,newValue:%u,type:%s", ap);
        break;
    case EDanceGroupLog_ProptyChange:
    case EDanceGroupLog_None:
    default:
        break;
    }
    m_strParam = szBuff;
}

//////////////////////////////////////////////////////////////////////////
CRoleConstellationMatchLog::CRoleConstellationMatchLog()
: m_nRoleID(0), m_eAction(CRoleConstellationMatchLog::EConstellationMatchAction_None)
, m_nCardConstellation(-1), m_nMatchRoleID(0), m_nSceneID(0)
{
}

CRoleConstellationMatchLog::~CRoleConstellationMatchLog()
{
}

void CRoleConstellationMatchLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    char *end = szSQL;
    sprintf(end, "INSERT INTO nsytsublogdb.log_RoleConstellationMatch "
        "(role_id, role_name, action, card_constellation, match_role_id, match_role_name, amuse_id, amuse_name, time) "
        "VALUES(%u, '", m_nRoleID);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strRoleName.c_str(), m_strRoleName.size());
    sprintf(end, "', %d, %d, %u, '", (int)m_eAction, m_nCardConstellation, m_nMatchRoleID);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strMatchRoleName.c_str(), m_strMatchRoleName.size());
    sprintf(end, "', %d, '", m_nSceneID);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strSceneName.c_str(), m_strSceneName.size());
    sprintf(end, "', %u)", (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////
CRoleVipLog::CRoleVipLog()
: m_nRoleID(0)
, m_eAction(CRoleVipLog::ERoleVipAction_None)
, m_eExpCause(ERoleVipExpCause_None)
, m_nAddValue(0)
{

}
CRoleVipLog::~CRoleVipLog()
{

}

void CRoleVipLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    char *end = szSQL;
    sprintf(end, "INSERT INTO nsytsublogdb.log_RoleVip (role_id, role_name, action, exp_cause, add_value, time) "
        "VALUES(%u, '", m_nRoleID);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strRoleName.c_str(), m_strRoleName.size());
    sprintf(end, "', %d, %d, %d, %u)", (int)m_eAction, (int)m_eExpCause, m_nAddValue, (unsigned int)time(NULL));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMeetActivityLog::CMeetActivityLog()
:m_nRoleID(0)
, m_eMeetAction(EMeetAction_None)
, m_nCoupleRoleID(0)
, m_nAmuseID(0)
, m_nTime(0)
{

}

CMeetActivityLog::~CMeetActivityLog()
{

}

void CMeetActivityLog::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.Log_RoleMeet(roleid, action, coupleroleid, amuseid, time ) VALUES(%u, %d, %u, %d, %u)",
        m_nRoleID, m_eMeetAction, m_nCoupleRoleID, m_nAmuseID, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////

CRoleAttrChangeLog::CRoleAttrChangeLog()
: m_nRoleID(0)
, m_eChangeCause(ERoleAttrChangeCause_None)
{
    m_strOldAttrValue.clear();
    m_strNewAttrValue.clear();
}

CRoleAttrChangeLog::~CRoleAttrChangeLog()
{
}

void CRoleAttrChangeLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    char *end = szSQL;
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleAttrChange(role_id, action, old_attr, new_attr, time) VALUES(%u, %d, '",
        m_nRoleID, (int)m_eChangeCause);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strOldAttrValue.c_str(), m_strOldAttrValue.size());
    sprintf(end, "', '");
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strNewAttrValue.c_str(), m_strNewAttrValue.size());
    sprintf(end, "', %u)", (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////
CRoleCoupleLog::CRoleCoupleLog()
: m_nMaleRoleID(0)
, m_nFemaleRoleID(0)
, m_eCoupleCause(ERoleCoupleCause_None)
, m_nBlessingValue(0)
, m_nIntimacy(0)
, m_nRedEvnelope(0)
{
}

CRoleCoupleLog::~CRoleCoupleLog()
{
}

void CRoleCoupleLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleCouple(maleid, femaleid, type, intimacy, blessingvalue, time, redevnelope) "
        "VALUES(%u, %u, %d, %d, %d, %u, %d )",
        m_nMaleRoleID, m_nFemaleRoleID, (int)m_eCoupleCause, m_nIntimacy, m_nBlessingValue, (unsigned int)time(NULL), m_nRedEvnelope);
}

//////////////////////////////////////////////////////////////////////////
CRoleDungeonDebrisLog::CRoleDungeonDebrisLog()
: m_nRoleID(0)
, m_eAction(EDebrisAction_None)
, m_nIsMainline(0)
, m_nDungeonID(0)
, m_nParcloseID(0)
, m_nDebrisID(0)
, m_nDebrisCount(0)
, m_nLeftDebrisCount(0)
{
}

CRoleDungeonDebrisLog::~CRoleDungeonDebrisLog()
{
}

void CRoleDungeonDebrisLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleDebris(roleid, action, ismainline, dungeonid, parcloseid, debrisid, debriscount, leftcount,time) "
        "VALUES(%u, %u, %u, %u, %u, %u, %u, %u, %u)",
        m_nRoleID, (unsigned int)m_eAction, m_nIsMainline, m_nDungeonID, m_nParcloseID, m_nDebrisID, m_nDebrisCount, m_nLeftDebrisCount, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////

CRoleVisitRankLog::CRoleVisitRankLog()
: m_nRoleID(0)
, m_eAction(EVisitRankAction_Hall)
, m_nDetailType(0)
, m_nMyRank(0)
{
}

void CRoleVisitRankLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleVisitRank(roleid, action, type, myrank, time) VALUES(%u, %d, %d, %u, %u)",
        m_nRoleID, (int)m_eAction, (int)m_nDetailType, (unsigned int)m_nMyRank, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////

CRoleFriendsLog::CRoleFriendsLog()
: m_nRoleID(0)
, m_nFriendID(0)
, m_eAction(EFriendsAction_Add)
, m_eMode(EFriendsMode_None)
{
}

void CRoleFriendsLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleFriends(roleid, friendid, action, mode, time) VALUES(%u, %u, %d, %d, %u)",
        m_nRoleID, m_nFriendID, (int)m_eAction, (int)m_eMode, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////
CRoleActivenessLog::CRoleActivenessLog()
: m_nRoleID(0)
, m_eAction(EActivenessAction_Unknown)
, m_nScore(0)
, m_nTarget(0)
{

}

void CRoleActivenessLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleActiveness(roleid, action, score, target, time) "
        " VALUES(%u, %d, %d, %d, %u)",
        m_nRoleID, (int)m_eAction, m_nScore, m_nTarget, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////
CRoleDungeonLog::CRoleDungeonLog()
: m_nRoleID(0)
, m_eAction(EDungeonAction_UnKnown)
, m_nValue(0)
{

}

void CRoleDungeonLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleDungeon(roleid, action, ismainline, dungeonid, parcloseid, value, time) "
        " VALUES(%u, %d, %d, %u, %u, %d, %u)",
        m_nRoleID, (int)m_eAction, (int)m_bIsMainline, m_nDungeonID, m_nParcloseID, m_nValue, (unsigned int)time(NULL));
}

/////////////////////////////////////////////////////////////////////////
CRoleMallLog::CRoleMallLog()
: m_nRoleID(0)
, m_eAction(EMallAction_UnKnown)
{

}

CRoleMallLog::~CRoleMallLog()
{

}

void CRoleMallLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleMall(roleid, action, time) "
        " VALUES(%u, %d, %u)",
        m_nRoleID, (int)m_eAction, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////
CRolePhotoRoomLog::CRolePhotoRoomLog()
: m_nRoleID(0)
, m_eAction(ERolePhotoRoomAction_UnKnown)
{

}

CRolePhotoRoomLog::~CRolePhotoRoomLog()
{

}

void CRolePhotoRoomLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RolePhotoRoom(roleid, action, time) "
        " VALUES(%u, %d, %u)",
        m_nRoleID, (int)m_eAction, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////
CRoleAuditionLog::CRoleAuditionLog()
: m_nRoleID(0)
, m_eAction(EAuditionAction_UnKnown)
, m_nMatchPlayer(0)
{

}

CRoleAuditionLog::~CRoleAuditionLog()
{

}

void CRoleAuditionLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleAudition(roleid, action, matchplayer, time) "
        " VALUES(%u, %d, %u, %u)",
        m_nRoleID, (int)m_eAction, m_nMatchPlayer, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////
RoleLongactingRechargeLog::RoleLongactingRechargeLog()
: m_nRoleID(0)
, m_eAction(ELongactingAction_UnKnown)
, m_nRewardIndex(0)
, m_bIsSucess(false)
, m_bIsOnline(false)
, m_nTotalMPoint(0)
{

}

RoleLongactingRechargeLog::~RoleLongactingRechargeLog()
{

}

void RoleLongactingRechargeLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleLongacting(roleid, action, rewardindex, issucess, isonline, totalpoint, time) "
        " VALUES(%u, %d, %u, %u, %u, %u, %u)",
        m_nRoleID, (int)m_eAction, m_nRewardIndex, (unsigned int)m_bIsSucess, (unsigned int)m_bIsOnline, m_nTotalMPoint, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////

CRoleCheckingMarkFailedLog::CRoleCheckingMarkFailedLog()
: m_nRoleID(0)
, m_nRoomType(0)
, m_nScene(0)
, m_nTeamMode(0)
, m_nMusicID(0)
, m_nMusicMode(0)
, m_nMusicLevel(0)
, m_nPlayerCount(0)
, m_nStartTime(0)
, m_nRound(0)
, m_nKeyRank(0)
, m_nLastSpecial(0)
, m_strRoundRank("")
, m_nMark(0)
, m_nCheckingMark(0)
{
}

void CRoleCheckingMarkFailedLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    char *end = szSQL;
    sprintf(end, "insert into nsytsublogdb.log_RoleCheckingMarkFailed(roleid, roomtype, "
        "scene, teammode, musicid, musicmode, musiclevel, playercount, starttime, round, "
        "keyrank, lastspecial, roundrank, mark, checkingmark, time) values(%u, %d, %d, %d, "
        "%d, %d, %d, %d, %d, %d, %d, %d, '", m_nRoleID, (int)m_nRoomType, m_nScene,
        (int)m_nTeamMode, m_nMusicID, m_nMusicMode, m_nMusicLevel, m_nPlayerCount,
        m_nStartTime, m_nRound, m_nKeyRank, m_nLastSpecial);
    end += strlen(end);
    end += CDBIMysql::EscapeString(end, m_strRoundRank.c_str(), m_strRoundRank.size());
    sprintf(end, "', %u, %u, %u)", m_nMark, m_nCheckingMark, (unsigned int)time(NULL));
}

ClothEffectLog::ClothEffectLog()
:m_nRoleID(0)
, m_eAction(EClothEffectAction_UnKnown)
, m_nItemID(0)
, m_nOldEffectID(0)
, m_nNewEffectID(0)
{

}

ClothEffectLog::~ClothEffectLog()
{

}

void ClothEffectLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleClothEffect(roleid, action, itemid, oldeffectid, neweffectid, time) "
        " VALUES(%u, %d, %lld, %u, %u, %u)",
        m_nRoleID, (int)m_eAction, m_nItemID, m_nOldEffectID, m_nNewEffectID, (unsigned int)time(NULL));
}

ClothEffectProgressLog::ClothEffectProgressLog()
:m_nRoleID(0)
, m_eAction(EClothEffectProgressAction_UnKnown)
, m_nSuitID(0)
, m_nParam1(0)
, m_strParam2("")
{

}

ClothEffectProgressLog::~ClothEffectProgressLog()
{

}

void ClothEffectProgressLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleClothEffectProgress(roleid, action, suitid, param1, param2, time) "
        " VALUES(%u, %d, %u, %u, '%s', %u)",
        m_nRoleID, (int)m_eAction, m_nSuitID, m_nParam1, m_strParam2.c_str(), (unsigned int)time(NULL));
}

CMedalLog::CMedalLog()
:m_nRoleID(0)
, m_nMedalID(0)
, m_nConditionID(0)
, m_nTimeStamp(0)
, m_eActivity(EMedalUnKonw)
, m_eAction(EMedalUnKonwAction)
{

}

CMedalLog::~CMedalLog()
{

}

void CMedalLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_Medal(roleid, action, medalid , conditionid , activity , time) "
        " VALUES(%u, %d, %d, %d, %d, %d)",
        m_nRoleID, m_eAction, m_nMedalID, m_nConditionID, m_eActivity, m_nTimeStamp);
}

CRedEnvelopeLog::CRedEnvelopeLog()
:m_nRoleID(0)
, m_eAction(ERedEnvelopeAction_UnKnown)
, m_nRedEnvelopeID(0)
, m_nOpenIndex(0)
, m_nRedEnvelopeMoney(0)
{

}

CRedEnvelopeLog::~CRedEnvelopeLog()
{
}

void CRedEnvelopeLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleRedEnvelope(roleid, action, redenvelopeid, openindex, money, time) "
        " VALUES(%u, %d, %u, %d, %d, %d)",
        m_nRoleID, m_eAction, m_nRedEnvelopeID, m_nOpenIndex, m_nRedEnvelopeMoney, (unsigned int)time(NULL));
}

CMagicArrayLog::CMagicArrayLog()
:m_nRoleID(0)
, m_eAction(EMagicArrayAction_UnKnown)
, m_nMagicArrayID(0)
{

}

CMagicArrayLog::~CMagicArrayLog()
{
}

void CMagicArrayLog::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleMagicArray(roleid, action, magicarrayid, time) "
        " VALUES(%u, %d, %u, %u)",
        m_nRoleID, m_eAction, m_nMagicArrayID, (unsigned int)time(NULL));
}

//////////////////////////////////////////////////////////////////////////
// role bigmama record
CRoleBigMamaDanceRecord::CRoleBigMamaDanceRecord()
    : m_nRoleID(0)
    , m_nDanceDanceGroupID(0)
    , m_nDanceRound(0)
{

}

CRoleBigMamaDanceRecord::CRoleBigMamaDanceRecord(unsigned int nRoleID, unsigned int nDanceGroupID, int nDanceRound)
    : m_nRoleID(nRoleID)
    , m_nDanceDanceGroupID(nDanceGroupID)
    , m_nDanceRound(nDanceRound)
{

}

CRoleBigMamaDanceRecord::~CRoleBigMamaDanceRecord()
{}

void CRoleBigMamaDanceRecord::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_RoleBigMamaDanceRecord(role_id, group_id, dance_round, stamp) "
        " VALUES(%u, %u, %d, %u)",
        m_nRoleID, m_nDanceDanceGroupID, m_nDanceRound, (unsigned int)time(NULL));
}

void CRoleBigMamaDanceRecord::InsertIntoLog()
{

}


//////////////////////////////////////////////////////////////////////////
// CGroupBigMamaDanceRecord
CGroupBigMamaDanceRecord::CGroupBigMamaDanceRecord()
    : m_nDanceGroupID(0)
    , m_nDanceRound(0)
    , m_nBeatType(0)
    , m_nBeatRole(0)
{

}

CGroupBigMamaDanceRecord::CGroupBigMamaDanceRecord(unsigned int nDanceGroupID, unsigned int nDanceRound, int nBeatType, unsigned int nBeatRole)
    : m_nDanceGroupID(nDanceGroupID)
    , m_nDanceRound(nDanceRound)
    , m_nBeatType(nBeatType)
    , m_nBeatRole(nBeatRole)
{

}


CGroupBigMamaDanceRecord::~CGroupBigMamaDanceRecord()
{

}

void CGroupBigMamaDanceRecord::ToSQL(char *szSQL, unsigned int nMaxSQLLen)
{
    sprintf(szSQL, "INSERT INTO nsytsublogdb.log_GroupBigMamaDanceRecord(group_id, dance_round, beat_type, beat_role, stamp) "
        " VALUES(%u, %d, %d, %u, %u)",
        m_nDanceGroupID, m_nDanceRound, m_nBeatType, m_nBeatRole, (unsigned int)time(NULL));
}

/////////////////////////////////////////////////////////////
//CPlayerType
CPlayerType::CPlayerType()
    : m_nRoleID(0)
    , m_nLastLoginTime(0)
    , m_nUserType(0)
{

}

CPlayerType::~CPlayerType()
{

}

void CPlayerType::ToSQL(char * szSQL, unsigned int nMaxSQLLen)
{
    Formatter::Format(szSQL, nMaxSQLLen, "INSERT INTO nsytsublogdb.log_PlayerType(role_id, time, user_type) VALUES(%u, %u, %d)", 
        m_nRoleID, m_nLastLoginTime, m_nUserType);
}

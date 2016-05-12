#ifndef _SERVER_CONFIG_H__
#define _SERVER_CONFIG_H__

#include <string>

class ServerConfig
{
public:
    ServerConfig();
    ~ServerConfig();

public:
    bool Load(std::string& filename);

public:
    int m_MaxNameLimit;                     //角色名字长度
    int m_MaxTitleLimit;                    //称号长度
    int m_MaxWeddingRoomLimit;              //婚姻房间名字长度
    int m_FamilyLength;                     //家族
    int m_FamilyNameLength;                 //家族名称长度
    int m_SignatureLength;                  //自我介绍长度
    int m_DanceGroupNameLength;             //舞团名字长度
    int m_DanceGroupAnnouncementLength;     //舞团公告长度
    int m_DanceGroupSummary;                //舞团简介长度
    int m_DanceGroupTitleName;              //舞团职位长度

    int m_nMaxMailTitleLength;              //邮件标题长度
    int m_nMaxMailContentLength;            //邮件内容长度

    int m_nMaxRoomNameLength;               //普通房间名字长度
    int m_nMaxAmuseRoomNameLength;          //时空之塔房间名字长度

    int m_nMaxDanceGroupBaseMemberCount;    //舞团基础成员上限
    int m_nMaxDanceGroupLevelUpAddition;    //舞团每级增加成员上限

    int m_nMaxBirthdayLength;               //生日长度

    int m_nMaxEggWordsLength;               //时光蛋留言长度

    int m_nMaxPetNameLength;                //宠物昵称长度
};

extern ServerConfig g_ServerConfig;


#endif

#ifndef __MANAGERMSGDEF_H__
#define __MANAGERMSGDEF_H__
#include "../share/CommonMsgDef.h"

enum eMSG_CHAT
{
    MSG_CHAT_BEGIN = MSG_TYPE_CHAT,
    MSG_CHAT_CHAT           ,//chat
    MSG_GM_Quest            ,//客户端向服务器发送的gm请求
    MSG_GM_GetQuest         ,//GM向服务器获取客户的请求
    MSG_S2C_Disconnect      ,//服务器跟客户断掉
    MSG_S2C_SysChat         ,//

    //聊天优化后的新增消息
    MSG_C2S_CHAT            ,//客户端发往服务器的除私聊以外的聊天消息
    MSG_C2S_CHAT_PRIV       ,//客户端发往服务器的私聊消息
    MSG_S2C_CHAT            ,//客户端发往服务器的除私聊以外的聊天消息
    MSG_S2C_CHAT_PRIV       ,//服务器发往客户端私聊消息,此消息是私聊时，给对方的
    MSG_S2C_CHAT_PRIVTOSELF ,//服务器发往客户端的私聊消息，此消息是私聊时，回给自己的

    MSG_C2S_AskAllFriendsInfo ,//向服务器查询所有好友信息
    MSG_S2C_AskAllFriendsInfo ,

    MSG_C2S_AskAllFriendsInfoByRoleId,
    MSG_S2C_AskAllFriendsInfoByRoleId,

    MSG_S2C_SyncAutoChatInfo,
    MSG_S2C_AutoChatResponse,

    MSG_S2G_Chat,
    MSG_S2G_Announce,
    MSG_G2S_Chat,
    MSG_G2S_ChatWindow      ,
    MSG_G2S_Admin           ,//系统管理公告，全世界系统公告
    MSG_G2S_World,

    MSG_S2G_GroupDanceChatHistoryRecord,          //记录 舞团聊天历史记录

    MSG_C2S_GroupDanceChatHistory,          //发送 聊天记录
    MSG_S2C_GroupDanceChatHistory,          //申请 聊天记录
    MSG_G2S_GroupDanceChatHistory,          //发送 聊天记录
    MSG_S2G_GroupDanceChatHistory,          //申请 聊天记录
};

#endif

//file end


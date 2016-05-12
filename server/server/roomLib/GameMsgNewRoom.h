#ifndef __LIBSERVERFRAME_GAMEMSGNEWROO_M__
#define __LIBSERVERFRAME_GAMEMSGNEWROO_M__

#include <string>
#include <map>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include "RoomDataStruct.h"

// 创建房间： c-s-g, g:
class GameMsg_C2S_NewCreateRoom : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_NewCreateRoom);

    GameMsg_C2S_NewCreateRoom(void);
    ~GameMsg_C2S_NewCreateRoom(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    std::string m_strRoomName;
    std::string m_strRoomPwd;
    char m_chRoomType;
    char m_chPhoneOS;
};


class GameMsg_C2S_NewTryEnterRoom : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_NewTryEnterRoom);

    GameMsg_C2S_NewTryEnterRoom(void);
    ~GameMsg_C2S_NewTryEnterRoom(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    char m_nRoleRoomType;// ERoleRoomType
    char m_chPhoneOS;
    unsigned int m_nRoomID;
    char m_nRoomType;
};


class GameMsg_C2S_NewQuitRoom : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_NewQuitRoom);

    GameMsg_C2S_NewQuitRoom(void);
    ~GameMsg_C2S_NewQuitRoom(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    char m_chQuitTo;
};


class GameMsg_C2S_NewPromoteRoomHost : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_NewPromoteRoomHost);
    GameMsg_C2S_NewPromoteRoomHost();
    ~GameMsg_C2S_NewPromoteRoomHost();

public:
    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    char m_chNewHostRoleRoomType;
    char m_chNewHostRoleRoomPos;
};


// 踢人
class GameMsg_C2S_NewKickPlayer : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_NewKickPlayer);

    GameMsg_C2S_NewKickPlayer(void);
    ~GameMsg_C2S_NewKickPlayer(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    char m_chRoleRoomType;
    char m_chRoleRoomPos;
    unsigned int m_nRoleID;
};


// 修改玩家在房间中的状态
class GameMsg_C2S_NewChangeRoleRoomState : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_NewChangeRoleRoomState);

    GameMsg_C2S_NewChangeRoleRoomState(void);
    ~GameMsg_C2S_NewChangeRoleRoomState(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    char m_chRoomType;
    char m_chRoleRoomState;
};


// 开启或者关闭房间中某个位置。
class GameMsg_C2S_NewChangeRoomPosState : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_NewChangeRoomPosState);

    GameMsg_C2S_NewChangeRoomPosState(void);
    ~GameMsg_C2S_NewChangeRoomPosState(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    char m_chRoleRoomType;
    char m_chRoleRoomPos;
    char m_chRoomPosState;
};


class GameMsg_S2C_NewCreateRoomSuc : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_NewCreateRoomSuc);
public:
    GameMsg_S2C_NewCreateRoomSuc(void);
    ~GameMsg_S2C_NewCreateRoomSuc(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
    unsigned char m_nRoomType;
    RoomWholeInfo m_RoomInfo;
};



class GameMsg_S2G_TransFromIDUpdate : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_TransFromIDUpdate);

    GameMsg_S2G_TransFromIDUpdate(void);
    ~GameMsg_S2G_TransFromIDUpdate(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

    int m_nTransformId;
};

class GameMsg_S2G_OnEquipNotify : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_OnEquipNotify);

    GameMsg_S2G_OnEquipNotify(void);
    ~GameMsg_S2G_OnEquipNotify(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);


public:
    DBItem m_dbEquip;
    unsigned int m_nEffectID;
};

class GameMsg_S2G_UnEquipNotify : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_UnEquipNotify);

    GameMsg_S2G_UnEquipNotify(void);
    ~GameMsg_S2G_UnEquipNotify(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

    itemid_t m_nItemID;
    unsigned short m_nColumn;
    unsigned short m_nIndex;
};

class GameMsg_S2G_EffectChangeNotify : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_EffectChangeNotify);

    GameMsg_S2G_EffectChangeNotify(void);
    ~GameMsg_S2G_EffectChangeNotify(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

    itemid_t m_nItemID;
    unsigned int m_nEffectID;
};

class GameMsg_S2G_RefreshSevenColor : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_RefreshSevenColor);

    GameMsg_S2G_RefreshSevenColor(void);
    ~GameMsg_S2G_RefreshSevenColor(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);


public:
    unsigned short        m_nSuitID;
    unsigned char         m_nColor;
};

class GameMsg_S2G_RefreshGeneNotify : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_RefreshGeneNotify);
    GameMsg_S2G_RefreshGeneNotify(void);
    ~GameMsg_S2G_RefreshGeneNotify(void);   

public:
    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
//    std::map<unsigned short, GeneData> m_mapGene;
    EGeneNotifyAction                m_eAction;
    GeneData           m_GeneData;
};

class GameMsg_S2G_AddMedals : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddMedals);

    GameMsg_S2G_AddMedals(void);
    ~GameMsg_S2G_AddMedals(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    unsigned char m_nSceneID;
    int m_nMedalID;
};


#endif // __LIBSERVERFRAME_GAMEMSGNEWROO_M__
#ifndef __ANNOUNCEMGR_H__
#define __ANNOUNCEMGR_H__

#include <map>
#include "../../socket/GameMsg_Base.h"
#include "../../socket/GameMsg_Processor.h"
#include "GMDataStruct.h"

class ITimerGm;

typedef std::vector<ITimerGm *>	ContainerTimer_t;
typedef std::map< time_t, ContainerTimer_t > MapTimer_t;
typedef std::map< unsigned int, CAnnounceInfo * > MapAnnouce_t;

class CAnnounceMgr : public GameMsg_Processor
{
private:
    CAnnounceMgr();
    ~CAnnounceMgr();
public:
    static CAnnounceMgr & Instance();
public:
    void Initialize();

    void Add(ITimerGm *pTimer);
    void ClearAllAnnounceTimer();

    void HandleNextAnnounce(CAnnounceInfo & rAnnounceInfo);

    void OnGMSendAnnounceOnce(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnGMSendAnnounceInterval(GameMsg_Base & msg, CSlotPeer & slotPeer);

    void OnGMGetAnnounce(GameMsg_Base& msg, CSlotPeer& slotPeer);
    void OnGMDeleteAnnounce(GameMsg_Base& msg, CSlotPeer& slotPeer);

    void LoadAnnounceDBCallBack(QueryBase & query);
    void AddAnnounceDBCallBack(QueryBase & query);
private:
    void _AddNewAnnounceDB(CAnnounceInfo & rAnnounceInfo);
    void _RegisterMessage();
    void _RegisterMessageProcessor();
    void _RemoveAnnounceRecordDB(CAnnounceInfo & announce);
    void _UpdateAnnounceRecordDB(CAnnounceInfo & announce);
    void _InitLoadedAnnouce();
public:
    MapAnnouce_t m_mapAnnounce;
};

#endif


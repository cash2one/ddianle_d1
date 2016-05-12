#ifndef __LIBSERVERFRAME_ROOMPOSMANAGER_H__
#define __LIBSERVERFRAME_ROOMPOSMANAGER_H__

#include <vector>
#include "../../datastructure/Macro_Define.h"

class PhoneRole
{
public:
    PhoneRole();
    PhoneRole(unsigned int nRoleID, Phone_OS eos);
    ~PhoneRole();

    unsigned int m_nRoleID;
    Phone_OS m_phone;
    static PhoneRole m_invalidPhoneRole;
};


class LWDanceRoomPosInfo
{
public:
    explicit LWDanceRoomPosInfo(int nMaxCount);
    virtual ~LWDanceRoomPosInfo();

    void Init();
    void Clear();


    int GetIdlePos() const;
    int GetFirstDancerPos() const;

    bool AddDancer(const PhoneRole &phoneRole);
    void RemoveDancer(int nPos);

    PhoneRole GetDancer(int nPos) const;

    bool MoveDancerPos(int nOldPos, int nNewPos);

    bool ClosePos(int nPos);
    void OpenPos(int nPos);

    bool IsPosClosed(int nPos) const;
    bool IsPosOpend(int nPos) const;

    bool HasFull() const;
    bool HasEmpty() const;

    int GetRolePos(unsigned int nRoleID) const;

    typedef std::vector<std::pair<unsigned int, char> > RoleVec;
    void GetAllPlayer(RoleVec &vecListner);
    int MaxDancerCount() { return m_vectDancer.size(); }

private:
    int FindPos(int nState) const;
    bool IsValidPos(int nPos) const;

private:
    std::vector<PhoneRole> m_vectDancer; // 这么多位置
    std::vector<int> m_vectPosState; // EDanceRoomPosState
};

// 肩负起玩家位置管理的责任（带位置的管理器）
class LWRoomPosManager
{
public:
    // 最大位置，0代表没有这个位置
    LWRoomPosManager(unsigned int dancerMaxCount, unsigned int audienceMaxCount);
    ~LWRoomPosManager();

    LWDanceRoomPosInfo* GetRoomPosInfo(ERoleRoomType eRoleType);
    bool GetRolePos(unsigned int nRoleID, ERoleRoomType &eRoleType, unsigned int &pos);
    bool AddDancer(ERoleRoomType eRoleType, const PhoneRole &phoneRole);
    bool RemoveRole(unsigned int nRoleID); // 
    unsigned int GetFirstDanceRole();
    void SetPostionState(ERoleRoomType eRoleType, unsigned int nPos, ERoomPosState ePosState);
    unsigned int GetPostionRole(ERoleRoomType eRoleType, unsigned int nPos);
    bool HasPlayerInPostion(ERoleRoomType eRoleType, unsigned int nPos);
    bool PostionOpened(ERoleRoomType eRoleType, unsigned int nPos);

    void InitRoomPosInfo();
    void ClearRoomPosInfo();

protected:

protected:
    LWDanceRoomPosInfo *m_pRoomDancer;
    LWDanceRoomPosInfo *m_pRoomAudience;
};



#endif // __LIBSERVERFRAME_ROOMPOSMANAGER_H__
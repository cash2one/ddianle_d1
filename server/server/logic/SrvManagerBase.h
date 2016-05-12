#ifndef __CSrvManagerBase_H__
#define __CSrvManagerBase_H__

#include "../share/ServerDataStruct.h"
#include "../../datastructure/RoleEntity.h"

class CSrvManagerBase
{

public:
    CSrvManagerBase();
    virtual ~CSrvManagerBase();

    virtual void OnUpdate(const unsigned long &lTimeElapsed);

    int GetMaxCount()const;
    int GetCurCount()const;
    CEntity* GetEntity(int iEntity);

    int GetState(int nIndex)const
    {
        if (nIndex > 0 && nIndex < m_nMaxIndex)
            return m_anEntityState[nIndex];
        else
            return -1;
    }

protected:
    bool Create(int nMaxCount);

protected:
    int m_nEntityCount;
    int	m_nMaxCount;
    int m_nMaxIndex;//最大的使用过的下标
    CEntity** m_ppEntity;
    int* m_anEntityState;

    void DeleteEntity(int iEntity);
};

#endif	//__CSrvManagerBase_H__


#include "../../socket/GameMsg_Base.h"
#include "SrvManagerBase.h"

int SendGameMsg(LPGameMsg_Base pMsg, const PLAYERTAG* pTag);

#include "../../socket/Windefine.h"
#include "../../socket/Utility.h"

CSrvManagerBase::CSrvManagerBase()
{
    m_nEntityCount = 1;
    m_nMaxCount = 0;
    m_ppEntity = NULL;

    m_anEntityState = NULL;
}

CSrvManagerBase::~CSrvManagerBase()
{
    if (m_ppEntity != NULL)
    {
        for (int i = 0; i < m_nMaxCount; i++)
            delete m_ppEntity[i];

        delete []m_ppEntity;
    }
    if (m_anEntityState != NULL)
    {
        delete []m_anEntityState;
    }
}


bool CSrvManagerBase::Create(int nMaxCount)
{
    m_nMaxCount = nMaxCount;
    if (m_nMaxCount < 1)
        m_nMaxCount = 10000;

    ASSERT(m_ppEntity == NULL);
    m_ppEntity = new CEntity*[m_nMaxCount];
    int i;
    for (i = 0; i < m_nMaxCount; i++)
        m_ppEntity[i] = NULL;
    m_nEntityCount = 1;


    ASSERT(m_anEntityState == NULL);
    m_anEntityState = new int[m_nMaxCount];
    for (i = 0; i < m_nMaxCount; i++)
        m_anEntityState[i] = 0;
    m_nMaxIndex = 0;

    return true;
}

void CSrvManagerBase::OnUpdate(const unsigned long &lTimeElapsed)
{
    for (int i = 1; i <= m_nMaxIndex; i++)
    {
        switch (m_anEntityState[i])
        {
        case 3://合法对象执行功能
            if (m_ppEntity[i]->IsValid())
                m_ppEntity[i]->EntityUpdate(lTimeElapsed);
            else
            {
                m_anEntityState[i] = 2;
            }
            break;
        case 2://呆删除对象
            m_anEntityState[i] = 1;
            break;
        case 1://立即删除对象
            delete m_ppEntity[i];
            m_ppEntity[i] = NULL;
            m_anEntityState[i] = 0;
            m_nEntityCount--;
            if (m_nMaxIndex == i)
            {
                int k;
                for (k = m_nMaxIndex - 1; k > 0; k--)
                {
                    if (m_anEntityState[k] != 0)
                        break;
                }
                m_nMaxIndex = k;
                ASSERT(m_nMaxIndex >= m_nEntityCount - 1);
            }
            break;
        }
    }
}

int CSrvManagerBase::GetMaxCount()const
{
    return m_nMaxCount;
}

int CSrvManagerBase::GetCurCount()const
{
    return m_nEntityCount;
}

CEntity* CSrvManagerBase::GetEntity(int iEntity)
{
    if (iEntity >= 0 && iEntity < m_nMaxCount)
    {
        return m_ppEntity[iEntity];
    }
    return NULL;
}


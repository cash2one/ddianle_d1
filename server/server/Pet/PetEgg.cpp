#include "PetEgg.h"


PetEgg::PetEgg(void)
    : m_nID(0)
    , m_nState(ePetEggState_Incubating)
    , m_nFinishedTime(0)
    , m_nTypeID(0)
{
}

PetEgg::~PetEgg(void)
{
}

void PetEgg::OnUpdate(unsigned int nNow)
{
    if (m_nID != 0 && m_nState == ePetEggState_Incubating)
    {
        if (nNow >= m_nFinishedTime)
        {
            m_nState = ePetEggState_Finished;

            CommonParam param;
            param.SetParam("PetEggFinished", 1);

            NotifyAll(param);
        }
    }
}

bool PetEgg::IsIncubating() const
{
    return m_nState == ePetEggState_Incubating;
}

bool PetEgg::IsFinished() const
{
    return m_nState == ePetEggState_Finished;
}

void PetEgg::SpeedUp()
{
    if (m_nState == ePetEggState_Incubating)
        m_nState = ePetEggState_Finished;
}

void PetEgg::Opened()
{
    m_nState = ePetEggState_Opened;
}

void PetEgg::SetState(unsigned char nState)
{
    if (nState < ePetEggState_Max)
        m_nState = nState;
}


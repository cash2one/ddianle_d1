#ifndef	__ALLACTIVITIESCONFIGMGR_H__
#define __ALLACTIVITIESCONFIGMGR_H__

class CParamPool;

class CAllActivitiesConfigMgr
{
private:
    CAllActivitiesConfigMgr();
    ~CAllActivitiesConfigMgr();
public:
    static CAllActivitiesConfigMgr & Instance();

    void AddXmasActivityTime(unsigned int nStartTime, unsigned int nEndTime);

    const char * SerializeName();
    void Serialize(CParamPool & IOBuff);
public:
    unsigned int m_nXmasActivityStartTime;
    unsigned int m_nXmasActivityEndTime;
};

#endif



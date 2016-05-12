#ifndef _IENTITY_H
#define _IENTITY_H

#include "../socket/Windefine.h"
#include <typeinfo>

using namespace std;

class CEntityComponent;

class IEntity
{
public:
    virtual ~IEntity() {}

public:
    virtual bool IsPlayer() const = 0;
    virtual bool IsNPC() const = 0;
    virtual bool IsItem() const = 0;
    virtual bool IsValid() const = 0;
    virtual CEntityComponent * GetComponent(int Index) const = 0;
    virtual CEntityComponent * GetComponent(const type_info & info_type) const = 0;

    virtual __int64 GetSessionID() const = 0;
    virtual unsigned short GetIndex() const = 0;
    virtual unsigned int GetEntityID() const = 0;

    virtual void SendEvent(int nID, void * pData) = 0;
};


#endif

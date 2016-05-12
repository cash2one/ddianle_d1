#ifndef		__ICOMMANDHANDLER_H__
#define		__ICOMMANDHANDLER_H__

#include <string>
using namespace std;

class CEntityComponent;

class ICmdHandler
{
public:
	ICmdHandler(){};
	virtual ~ICmdHandler(){};
public:
	virtual string Cmd() const = 0;
	virtual ICmdHandler *CreateCmdHandler() = 0;
	virtual void process(CEntityComponent &cmp, int nPara1, int nPara2, int nPara3, void * pData, const string & strCmd)= 0;
};

#endif


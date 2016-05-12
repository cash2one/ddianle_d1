#ifndef __ADDR_MGR_H__
#define __ADDR_MGR_H__

#include <list>
#include <map>
using namespace std;

#include "SocketDef.h"

namespace ZeroSocket
{

class CAddrMgr
{
public:
	CAddrMgr();
	//禁止某个地址一段时间,参数为in_addr的整数类型值
	void Deny(unsigned int addr, int nSecond);
	//禁止某个网段;参数字符串为网段的前面的ip地址后面补0，例如 172.18.132.0 或者 61.0.0.0
	void DenySection(const char* addr);
	//判断某个地址是否被禁止
	bool BeDenied(unsigned int addr);
protected:
	//响应时间流逝，timeElapse为自上次调用后流逝的时间，单位是毫秒
	void Update(int& timeElapse);
private:
	int m_nTimeElapse;
	map< unsigned int, int > mapAddr;
	list< unsigned int >listAddrSection;
};
}
#endif//__ADDR_MGR_H__


//file end


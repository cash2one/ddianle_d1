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
	//��ֹĳ����ַһ��ʱ��,����Ϊin_addr����������ֵ
	void Deny(unsigned int addr, int nSecond);
	//��ֹĳ������;�����ַ���Ϊ���ε�ǰ���ip��ַ���油0������ 172.18.132.0 ���� 61.0.0.0
	void DenySection(const char* addr);
	//�ж�ĳ����ַ�Ƿ񱻽�ֹ
	bool BeDenied(unsigned int addr);
protected:
	//��Ӧʱ�����ţ�timeElapseΪ���ϴε��ú����ŵ�ʱ�䣬��λ�Ǻ���
	void Update(int& timeElapse);
private:
	int m_nTimeElapse;
	map< unsigned int, int > mapAddr;
	list< unsigned int >listAddrSection;
};
}
#endif//__ADDR_MGR_H__


//file end


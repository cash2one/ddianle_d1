#ifndef __COMMAND_PROCESSOR_H__
#define __COMMAND_PROCESSOR_H__

#include "../libServerFrame/Console.h"

class CommandProcessor : public CommandPrivilegeManager
{
public:
    CommandProcessor(void);
    ~CommandProcessor(void);

    virtual void ParseCommand(char* strCommandLine);
    virtual int ProcessCommand(void *param);

protected:
    //�������н���Ϊ����Ͳ���
    void PrepareParse();

protected:
    std::string m_strCommandLine;           //�����������

    int m_nParam1, m_nParam2, m_nParam3;    //������������������,Ŀǰ���ֻ�ܽ���������������,����Ĳ�����Ҫ�Լ�����������
    std::string m_strCommand;               //����������������ַ���
    std::string m_strParam;                 //���������Ĳ�������ַ���

    std::vector<std::string> m_asParam;     //�ÿո�ָ��Ĳ����б�
    std::vector<int> m_anParam;             //�ÿո�ָ��Ĳ����б����в�����������atoi()
};

#endif//__COMMAND_PROCESSOR_H__


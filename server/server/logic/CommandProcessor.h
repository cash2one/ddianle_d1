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
    //将命令行解析为命令和参数
    void PrepareParse();

protected:
    std::string m_strCommandLine;           //输入的命令行

    int m_nParam1, m_nParam2, m_nParam3;    //解析出来的整数参数,目前最多只能解析出来两个参数,更多的参数需要自己解析命令行
    std::string m_strCommand;               //解析出来的命令的字符串
    std::string m_strParam;                 //解析出来的参数表的字符串

    std::vector<std::string> m_asParam;     //用空格分隔的参数列表
    std::vector<int> m_anParam;             //用空格分隔的参数列表，所有参数均来自于atoi()
};

#endif//__COMMAND_PROCESSOR_H__


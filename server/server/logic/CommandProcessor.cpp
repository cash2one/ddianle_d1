#include "CommandProcessor.h"
#include "../Command/ICommandHandler.h"
#include "../Command/CommandHandlerManager.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../chat/EntityChatComponent.h"

CommandProcessor::CommandProcessor(void)
{
}

CommandProcessor::~CommandProcessor(void)
{
}

int CommandProcessor::ProcessCommand(void *param)
{
    CEntityComponent *roleChat = (CEntityComponent *)param;
    if (roleChat == NULL)
        return -1;

    CEntityAttributeComponent *roleAttr = ((CEntityChatComponent *)roleChat)->GetRoleAttr();
    if (roleAttr == NULL)
        return -1;

    if (HasCommandPrivilege(m_strCommand, roleAttr->GetPriv()))
    {
        ICmdHandler *pCmdHandler = CCmdHanderManager::Instance().GetCmdHandler(m_strCommand);
        if (pCmdHandler != NULL)
        {
            pCmdHandler->process(*roleChat, m_nParam1, m_nParam2, m_nParam3, NULL, m_strCommandLine);
        }
        return 0;
    }

    WriteLog(LOGLEVEL_ERROR, "[GM] roleid=%u try to execute cmd=%s without privilege",
        roleAttr->GetRoleID(), m_strCommandLine.c_str());

    return -1;
}

void CommandProcessor::ParseCommand(char* szInput)
{
    if (szInput == NULL || szInput[0] == 0)
        return ;

    PrepareParse();

    char *pstr = NULL, *pstr2 = NULL;
    m_strCommandLine = szInput;

    //trim left
    pstr = szInput;
    while (*pstr == ' ')
        pstr++;

    //get command
    pstr2 = strchr(pstr, ' ');
    if (pstr2 != NULL) 
    {
        *pstr2++ = 0;
    }
    m_strCommand = pstr;

    // parse param
    if (pstr2 != NULL)
    {
        while (*pstr2 == ' ')
            pstr2++;
        m_strParam = pstr2;
        pstr = strchr(pstr2, ' ');
        std::string strParam;
        while (pstr != NULL)
        {
            *pstr++ = 0;
            strParam = pstr2;
            m_asParam.push_back(strParam);
            while (*pstr == ' ')
                pstr++;
            pstr2 = pstr;
            pstr = strchr(pstr2, ' ');
        }
        strParam = pstr2;
        if (strParam.size() > 0)
            m_asParam.push_back(strParam);
        std::vector<std::string>::iterator it;
        for (it = m_asParam.begin(); it != m_asParam.end(); ++it)
        {
            m_anParam.push_back(atoi(it->c_str()));
        }

        if (m_anParam.size() > 0)
            m_nParam1 = m_anParam[0];
        if (m_anParam.size() > 1)
            m_nParam2 = m_anParam[1];
        if (m_anParam.size() > 2)
            m_nParam3 = m_anParam[2];
    }
}

void CommandProcessor::PrepareParse()
{
    m_strCommandLine.assign("");

    m_nParam1 = 0;
    m_nParam2 = 0;
    m_nParam3 = 0;

    m_strCommand.assign("");
    m_strParam.assign("");

    m_asParam.clear();
    m_anParam.clear();
}



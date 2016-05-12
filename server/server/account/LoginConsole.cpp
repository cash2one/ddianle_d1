#include "LoginConsole.h"
#include "LoginCirculator.h"
#include <time.h>
#include "../share/GameServerLinkInfo.h"
#include "../util/CommonFunc.h"
#include "../../socket/Formatter.h"

int CLoginConsole::ProcessCommand(int nParam /* = 0 */)
{
    CConsole::ProcessCommand(nParam);
    switch(m_nCommand) 
    {
    case 1://count
        Formatter(m_strExport) << "Current player count = " << g_pCirculator->GetCurLoginPlayerSize() << "\n";
        break;
    case 2://listAccount
        {
            m_strExport.assign("  Account | state |    login time    | group | line |\n");
            struct tm tTime;
            CLoginCirculator::tmapOnlineInfo::iterator it = g_pCirculator->m_mapAccountOnline.begin();
            for (; it != g_pCirculator->m_mapAccountOnline.end(); ++it)
            {
                LocalTime((time_t*)&it->second->nTimeLogin, &tTime);

                const GameServerLinkInfo *pLinkInfo = g_pCirculator->GetGameServerLinkInfo( it->second->nSlot );
                if ( pLinkInfo == NULL )
                    continue;

                std::string strLine;
                Formatter::Format(strLine, "%9d | %5d | %04d-%02d-%02d %02d:%02d | %5d | %4d |\n",
                    it->first, it->second->nLoginState, tTime.tm_year+1900, tTime.tm_mon+1, tTime.tm_mday, 
                    tTime.tm_hour, tTime.tm_min, pLinkInfo->GetGroup(), pLinkInfo->GetLine() );

                if (m_strExport.length() + strLine.length() >= MAX_OUTPUT_STRING)
                    break;
                else
                    m_strExport.append(strLine);
            }
        }
        break;
    case 3://dropAccount
        {
            CLoginCirculator::tmapOnlineInfo::iterator it =
                g_pCirculator->m_mapAccountOnline.find((unsigned int ) m_nParam1);
            if (it != g_pCirculator->m_mapAccountOnline.end())
            {
                g_pCirculator->m_mapAccountOnline.erase(it);
                Formatter(m_strExport) << "Account " << m_nParam1 << " dropped!\n";
            }
            else
            {
                Formatter(m_strExport) << "Account " << m_nParam1 << " not found!\n";
            }
        }
        break;
    case 4://listServer
        {
            m_strExport.assign("  Index |    ip address   | port | clients | group | line |\n");
            for ( unsigned short slot = 0; slot < (unsigned short)g_pCirculator->m_pGameSrvSocketMgr->GetMaxClientCount(); ++slot )
            {
                const GameServerLinkInfo *pLinkInfo = g_pCirculator->GetGameServerLinkInfo( slot );
                if ( pLinkInfo == NULL )
                    continue;

                in_addr in;
                std::string strLine;
                in.s_addr = pLinkInfo->GetAddr();

                Formatter::Format(strLine, "%7d | %15s | %4d | %7d | %5d | %4d |\n", slot, inet_ntoa(in), 
                    pLinkInfo->GetPort(), pLinkInfo->GetClient(), pLinkInfo->GetGroup(), pLinkInfo->GetLine() );

                if (m_strExport.length() + strLine.length() >= MAX_OUTPUT_STRING)
                    break;
                else
                    m_strExport.append(strLine);
            }
        }
        break;
    case 5://softLimit
        {
            if (m_nParam1 != 0)
            {
                g_pCirculator->m_nSoftLimitClientCount = m_nParam1;
                Formatter(m_strExport) << "Old limit: " << g_pCirculator->m_nSoftLimitClientCount
                    << ", New limit: " << m_nParam1 << "\n";
            }
            else
            {
                Formatter(m_strExport) << "Soft limit: " << g_pCirculator->m_nSoftLimitClientCount << "\n";
            }
        }
        break;
    case 6://specialUser
        {
            extern list< char* >g_listSpecialUser;
            if (m_strParam.size() > 0 && m_strParam.size() < MAX_NAME_LEN)
            {
                char *pUsername = new char[MAX_NAME_LEN];
                strcpy( pUsername, m_strParam.c_str());
                g_listSpecialUser.push_back(pUsername);
            }

            m_strExport.assign("SpecialUser: \n");
            for (list< char* >::iterator it = g_listSpecialUser.begin();
                it != g_listSpecialUser.end(); ++it)
            {
                std::string strName(*it);
                m_strExport.append(strName);
                m_strExport.append("\n");

                if (m_strExport.length() > MAX_OUTPUT_STRING - 100)
                    break;
            }
        }
        break;
    }

    return m_nCommand;
}


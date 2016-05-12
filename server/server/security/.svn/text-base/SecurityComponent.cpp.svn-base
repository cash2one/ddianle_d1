#include "../../datastructure/DataStruct_DB.h"
#include "../util/CommonFunc.h"
#include "../../socket/GameMsg_Map.h"
#include "../libServerFrame/QueryManager.h"
#include "../item/ItemProcess.h"
#include "../SystemSetting/SystemSettingMgr.h"

#include "SecurityComponent.h"
#include "SecurityQuizDataManager.h"
#include "GameMsg_Security.h"

CSecurityComponent::CSecurityComponent()
: CEntityComponent(true, true)
{
    m_pAttr = NULL;
    m_pNet = NULL;
}
CSecurityComponent::~CSecurityComponent()
{
}

void CSecurityComponent::Start()
{
    m_pAttr = GetComponent<CRoleAttribute>();
    m_pNet = GetComponent<CRoleNet>();
}

void CSecurityComponent::SerializeComponent(CParamPool &IOBuff)
{
#ifdef SERVER_IOS
    bool bHasSecurityQuizFlag = false;
    if (m_securityQuiz.size() > 0 && m_securityQuiz.begin()->m_nQuizID > 0)
    {
        bHasSecurityQuizFlag = true;
    }
    if (CSystemSettingMgr::Instance().IsSecurityQuizOpen() != true)
    {
        bHasSecurityQuizFlag = true;
    }

    IOBuff.AddBool(bHasSecurityQuizFlag);

    // 安全保护问题
    SerializeSecurityQuiz(IOBuff);
#endif //SERVER_IOS
}

void CSecurityComponent::RegComponentNetMsgMap()
{
#ifdef SERVER_IOS
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SetSecurityQuiz);
    RegMsgProcessor(MSG_C2S_SetSecurityQuiz, &CSecurityComponent::OnSetSecurityQuiz);
#endif //SERVER_IOS
}

void CSecurityComponent::OnSetSecurityQuiz(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_SetSecurityQuiz *pMsg = (GameMsg_C2S_SetSecurityQuiz *)&msg;
    std::string strError = "";

    // 基本的安全保护问题个数校验
    if (pMsg->m_quizList.size() != MAX_SQUIZ_COUNT)
    {
        strError = "Security_Invalid_Quiz_Count";
    }
    else
    {
        for (SecurityQuizList::const_iterator it = pMsg->m_quizList.begin(); it != pMsg->m_quizList.end(); ++it)
        {
            // 安全保护问题校验，是否存在
            const SecurityQuiz *pQuiz = SecurityQuizDataManager::instance().GetSecurityQuiz(it->m_nQuizID);
            if (pQuiz == NULL)
            {
                strError = "Security_Invalid_Quiz";
                break;
            }
            else if (it->m_strAnswer.length() <= 0 || it->m_strAnswer.length() >= MAX_SQUIZ_LEN)//答案是否过长
            {
                strError = "Security_Invalid_Quiz_Answer";
                break;
            }
        }

        if (strError.empty())
        {
            int nQuizIndex = 0;
            SecurityQuiz_DB *pQuizDB = new SecurityQuiz_DB;
            for (SecurityQuizList::const_iterator it = pMsg->m_quizList.begin(); it != pMsg->m_quizList.end(); ++it)
            {
                // 安全保护问题校验，是否存在
                const SecurityQuiz *pQuiz = SecurityQuizDataManager::instance().GetSecurityQuiz(it->m_nQuizID);
                if (pQuiz != NULL)
                {
                    pQuizDB->nQuizID[nQuizIndex] = it->m_nQuizID;
                    strcpy(pQuizDB->szAnswer[nQuizIndex], it->m_strAnswer.c_str());

                    // 更新内存数据
                    SecurityQuiz quiz(pQuiz->m_nQuizID, pQuiz->m_strQuiz.c_str(), pQuizDB->szAnswer[nQuizIndex]);
                    m_securityQuiz.push_back(quiz);
                }

                ++nQuizIndex;
            }

            // 记录数据
            AddQuery(Query_Security_SetSecurityQuiz, m_pAttr->m_nRoleID, pQuizDB);

            // 发送奖励
            std::string &strMailTitle = SecurityQuizDataManager::instance().MailTitle();
            std::string &strMailContent = SecurityQuizDataManager::instance().MailContent();
            std::list<CItem> &rewardItem = SecurityQuizDataManager::instance().RewardItem(m_pAttr->m_nSex);
            int nMoney = SecurityQuizDataManager::instance().RewardMoney();
            CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), rewardItem, EItemAction_Add_SetSecurityQuiz, 0, true, EMailType_SetSecurityQuiz, strMailTitle, strMailContent);
            m_pAttr->ChangeMoney(nMoney, EChangeMoneyCause_Add_SetSecurityQuiz, 0);

            // 返回成功
            unsigned char nSex = 0;
            if (m_pAttr != NULL)
                nSex = m_pAttr->m_nSex;

            GameMsg_S2C_SetSecurityQuizSuc suc;
            suc.m_rewardItem = SecurityQuizDataManager::instance().RewardItem(nSex);
            suc.m_nMoney = SecurityQuizDataManager::instance().RewardMoney();
            m_pNet->SendPlayerMsg(&suc);
        }
    }

    if (!strError.empty())
    {
        GameMsg_S2C_SetSecurityQuizFail fail;
        fail.m_strError = strError;
        m_pNet->SendPlayerMsg(&fail);
    }
}

void CSecurityComponent::SerializeSecurityQuiz(CParamPool &IOBuff)
{
    SecurityQuizList &quizList = SecurityQuizDataManager::instance().QuizList();
    IOBuff.AddUShort((unsigned short)quizList.size());
    for (SecurityQuizList::iterator it = quizList.begin(); it != quizList.end(); ++it)
    {
        IOBuff.AddUChar(it->m_nQuizID);
        IOBuff.AddStringW(it->m_strQuiz.c_str());
    }
}

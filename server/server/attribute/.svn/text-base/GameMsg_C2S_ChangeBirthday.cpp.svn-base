#include "GameMsg_C2S_ChangeBirthday.h"
#include "../../server/share/ServerMsgDef.h"
#include "../../socket/Utility.h"

GameMsg_C2S_ChangeBirthday::GameMsg_C2S_ChangeBirthday()
    : GameMsg_Base(MSG_C2S_ChangeBirthday)
{
    memset(m_szYear, 0, MAX_YEAR_LEN);
    memset(m_szMonth, 0, MAX_MONTH_LEN);
    memset(m_szDay, 0, MAX_DAY_LEN);
}

GameMsg_C2S_ChangeBirthday::~GameMsg_C2S_ChangeBirthday()
{
}

bool GameMsg_C2S_ChangeBirthday::doDecode(CParamPool &IOBuff)
{
    string strTemp;

    IOBuff.GetStringW(strTemp);
    CopyString2Buffer(strTemp, m_szYear, MAX_YEAR_LEN - 1);

    IOBuff.GetStringW(strTemp);
    CopyString2Buffer(strTemp, m_szMonth, MAX_MONTH_LEN - 1);

    IOBuff.GetStringW(strTemp);
    CopyString2Buffer(strTemp, m_szDay, MAX_DAY_LEN - 1);

    return true;
}

//end


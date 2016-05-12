#include "LuckDrawPrizeFactory.h"
#include "LuckDrawItemPrize.h"
#include "LuckDrawMoneyPrize.h"

#include <stdio.h>


CLuckDrawPrizeBase* CLuckDrawPrizeFactory::CreateLuckDrawPrizeByType(unsigned char nType)
{
    CLuckDrawPrizeBase *pRet = NULL;

    switch (nType)
    {
    case ELuckDrawPrizeType_Item:
        {
            pRet = new CLuckDrawItemPrize;
        }
        break;

    case ELuckDrawPrizeType_Money:
        {
            pRet = new CLuckDrawMoneyPrize;
        }
        break;
    }

    return pRet;
}

CLuckDrawPrizeBase* CLuckDrawPrizeFactory::CreateLuckDrawPrizeByInstance(const CLuckDrawPrizeBase *pInstance)
{
    CLuckDrawPrizeBase *pRet = NULL;

    if (pInstance == NULL)
        return pRet;

    switch (pInstance->GetType() )
    {
    case ELuckDrawPrizeType_Item:
        {
            const CLuckDrawItemPrize *pItem = dynamic_cast<const CLuckDrawItemPrize *>(pInstance);

            if (pItem != NULL)
                pRet = new CLuckDrawItemPrize(*pItem);
        }
        break;

    case ELuckDrawPrizeType_Money:
        {
            const CLuckDrawMoneyPrize *pMoney = dynamic_cast<const CLuckDrawMoneyPrize *>(pInstance);

            if (pMoney != NULL)
                pRet = new CLuckDrawMoneyPrize(*pMoney);
        }
        break;
    }

    return pRet;
}


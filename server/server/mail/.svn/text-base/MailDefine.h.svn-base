#ifndef _MAIL_DEFINE_H_
#define _MAIL_DEFINE_H_

#include <string>
#include <map>
#include <list>
#include "../../datastructure/DataStruct_DB.h"

using namespace std;

enum MailReturnType				// 部分错误代码依赖物品的错误代码
{
	MAIL_OK = 0,
	MAIL_PARAM_ERROR = 1,
	MAIL_NORECEIVER = 2,
	MAIL_MONEY_NOTENOUGH = 3,
	MAIL_NOITEM_GET = 4,
	MAIL_NOMONEY_GET  = 5,

	MAIL_ITEMGRID_ITEMFULL = 10,
	MAIL_ITEM_UNIQUEEXIST = 11,
	MAIL_ITEM_HASGET = 12,
	MAIL_ITEM_HASNOTGET = 13,

	MAIL_UNKNOWN = 34,
	MAIL_FAIL = 35,

	MAIL_ITEM_NotQualifiedToGetDefaultRing = 36,
    MAIL_NOT_RECEIVE_STRANGE = 37,

	MAIL_MAX
};

enum MailLoadType
{
	MAIL_NO_NOTICE_CLIENT = 0,
	MAIL_NOTICE_CLIENT
};

#endif //_MAIL_DEFINE_H_


//end file



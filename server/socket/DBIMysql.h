// Description:  database interface for mysql

#ifndef __DATABASE_INTERFACE_MYSQL__
#define __DATABASE_INTERFACE_MYSQL__

#ifdef WIN32

#ifndef _WIN32
#define _WIN32
#endif

#ifndef __LCC__
#define __LCC__
#endif

#pragma warning(disable:4267)
#include <vector>
#ifndef NTDDI_WIN7SP1
#define NTDDI_WIN7SP1 0x06010100
#endif
#include "../server/mysql/mysql.h"
#include "../server/mysql/errmsg.h"
#pragma comment(lib, "libmysql.lib")
//#pragma comment(lib, "mysqlclient.lib")

#else //WIN32
#include <vector>
#ifdef _MYSQL_INDEPEND
#include "mysql.h"
#include "errmsg.h"
#else
#include "../server/mysql/mysql.h"
#include "../server/mysql/errmsg.h"
#endif

#include "Windefine.h"

#endif//WIN32


#define MAX_SQL_LEN         2024
#define MAX_NAMEPASS		30

#ifndef _TEST_DBI
#define _TEST_DBI           0
#endif

#include <string>

class CDBIMysql
{
public:
    CDBIMysql();
    virtual ~CDBIMysql();

public:
    bool ReOpenDatabase();
    bool OpenDatabase(const char *user, const char *password, const char* dbname, const char* server, unsigned int nPort);
    void CloseDatabase();
protected:
    bool ExecSQL(const char *strSQL = NULL, unsigned int length = 0);
    MYSQL_ROW FetchRow();
    void SafeFreeResult();
    int GetAffectedRowCount();//只返回修改记录的语句的影响行数

public:
    static unsigned long EscapeString(char *to, const char *from, unsigned long from_length);
    static std::string EscapeString( const std::string &str );

protected:
    MYSQL_RES *result;
    MYSQL_ROW row;
    char sql[MAX_SQL_LEN];

    MYSQL m_mysqlConnection;

    char m_szUser[MAX_NAMEPASS];
    char m_szPass[MAX_NAMEPASS];
    char m_szDB[MAX_NAMEPASS];
    char m_szServer[MAX_NAMEPASS];
    unsigned int m_nPort;

    std::vector<MYSQL_ROW> m_rowManager;
    int m_nRowFieldCount;
};

#endif//__DATABASE_INTERFACE_MYSQL__

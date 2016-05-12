// Description: interface for accessing mysql database

#include "DBIMysql.h"
#include <stdio.h>
#include <stdlib.h>
#include "Utility.h"
#include "Log.h"
#include "conv.h"

#ifdef WIN32
#pragma warning(disable:4996)
#endif

#ifndef _TEST_DBI
#define _TEST_DBI   0
#endif

extern CLog g_DBLog;

CDBIMysql::CDBIMysql()
{
    result = NULL;
    row = NULL;
    memset(sql, 0, sizeof(sql));

    mysql_init(&m_mysqlConnection);

    m_nPort = 0;
    m_nRowFieldCount = 0;
}

CDBIMysql::~CDBIMysql()
{
    SafeFreeResult();
    CloseDatabase();
}

bool CDBIMysql::OpenDatabase(const char *user, const char *password, const char* dbname, const char* server, unsigned int nPort)
{
    strcpy(m_szUser, user);
    strcpy(m_szPass, password);
    strcpy(m_szDB, dbname);
    strcpy(m_szServer, server);
    m_nPort = nPort;

    return ReOpenDatabase();
}

bool CDBIMysql::ReOpenDatabase()
{
    if (!mysql_real_connect(&m_mysqlConnection, m_szServer, m_szUser, m_szPass, m_szDB, m_nPort, NULL, 0))
    {
        return false;
    }
    else
    {
        return true;
    }
}

unsigned long CDBIMysql::EscapeString(char *to, const char *from, unsigned long from_length)
{
    const char* szFrom = from;
#ifndef UTFDB
    std::string strGBKSQL = UTF8ToGBK(szFrom, from_length);
    szFrom = strGBKSQL.c_str();
    from_length = strGBKSQL.size();
#endif
    return mysql_escape_string(to, szFrom, from_length);
}

std::string CDBIMysql::EscapeString( const std::string &str )
{
    const char *from = str.c_str();
    unsigned long length = str.length();

#ifndef UTFDB
    const std::string &strGBK = UTF8ToGBK( str );
    from = strGBK.c_str();
    length = strGBK.length();
#endif

    size_t nMaxSize = length * 3 + 1;
    char *buf = new char[nMaxSize];
    memset( buf, 0, nMaxSize );
    mysql_escape_string( buf, from, length );

    std::string tmp( buf );
    delete[] buf;

    return tmp;
}

bool CDBIMysql::ExecSQL(const char *strSQL, unsigned int length)
{
    if (strSQL == NULL)
        strSQL = sql;
    if (length == 0)
        length = strlen(strSQL);

    SafeFreeResult();
#if _TEST_DBI > 0
    TRACE("ExecSQL(): %s\n", strSQL);
#endif

    if (mysql_real_query(&m_mysqlConnection, strSQL, length))
    {
        unsigned int nErrNo = mysql_errno(&m_mysqlConnection);
        WriteLogDB(LOGLEVEL_ERROR, "Failed query %s, Error %d:%s\n", strSQL, nErrNo, mysql_error(&m_mysqlConnection));
        TRACE("Failed query %s, Error %d:%s\n", strSQL, nErrNo, mysql_error(&m_mysqlConnection));
        if (nErrNo == CR_CONNECTION_ERROR
            || nErrNo == CR_CONN_HOST_ERROR
            || nErrNo == CR_SERVER_GONE_ERROR
            || nErrNo == CR_SERVER_LOST
            || nErrNo == CR_TCP_CONNECTION
            )
        {
            TRACE("ReOpenDatabase\n");
            if (ReOpenDatabase())
            {
                return ExecSQL(strSQL, length);
            }
        }
        return false;
    }
    else
    {
#ifdef WIN32
#ifdef _DEBUG
        WriteLogDB(LOGLEVEL_DEBUG, "Query OK. %s.\n", strSQL);
#endif
#endif
        result = mysql_store_result(&m_mysqlConnection);
        return true;
    }
}

#ifndef UTFDB

MYSQL_ROW CDBIMysql::FetchRow()
{
    if (result == NULL)
        return NULL;

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row == NULL)
        return row;

    int nFields = mysql_num_fields(result);
    MYSQL_ROW newRow = new char*[nFields];
    m_rowManager.push_back(newRow);
    m_nRowFieldCount = nFields;

    for (int i = 0; i < nFields; i++)
    {
        if (row[i] == NULL || strlen(row[i]) == 0)
        {
            newRow[i] = (char *)"";
            continue;
        }

        std::string strUTF8 = GBKToUTF8(row[i]);
        int nRowILen = strUTF8.size() + 1;
        newRow[i] = new char[nRowILen + 1];
        newRow[i][nRowILen] = 0;
        strncpy(newRow[i], strUTF8.c_str(), nRowILen);
    }

    return newRow;
}

void CDBIMysql::SafeFreeResult()
{
    std::vector<MYSQL_ROW>::iterator it = m_rowManager.begin();
    for (; it != m_rowManager.end(); ++it)
    {
        MYSQL_ROW row = *it;
        for (int i = 0; i < m_nRowFieldCount; i++)
        {
            if (row[i] != NULL && strlen(row[i]) != 0)
            {
                delete[] row[i];
                row[i] = NULL;
            }
        }
        delete[] row;
    }
    m_rowManager.clear();
    if (result != NULL)
    {
        mysql_free_result(result);
        result = NULL;
    }
}

#else

MYSQL_ROW CDBIMysql::FetchRow()
{
    if (result == NULL)
        return NULL;

    MYSQL_ROW row = mysql_fetch_row(result);
    return row;
}

void CDBIMysql::SafeFreeResult()
{
    if (result != NULL)
    {
        mysql_free_result(result);
        result = NULL;
    }
}

#endif

int CDBIMysql::GetAffectedRowCount()
{
    return (int)mysql_affected_rows(&m_mysqlConnection);
}

void CDBIMysql::CloseDatabase()
{
    SafeFreeResult();
    mysql_close(&m_mysqlConnection);
}

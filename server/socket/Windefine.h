#ifndef __WINDOWS_DEFINE_H__
#define __WINDOWS_DEFINE_H__

/**********************************************************************************
///// windows��linux����ϵ�һЩ����

1.linux��g++Ҫ��ÿ���ļ���β��Ҫ�и����У�������о���

2.ͷ�ļ��ķ�ֹ��ΰ�������#ifndef...#define...#endif�ķ�ʽ����ͨ�ã�
vc.net��ʹ��#pragma once�� g++��Ϊ��ʱ

3.�µ�ISO��׼c++��for(int i = 0; i < 10; i++){...}����i��������������forѭ���ڵģ�
��������forѭ����������i��g++ʹ�����µı�׼,vc�������ѭ���������ѭ������

4.vc������Ĭ�ϵ����ݶ��뷽ʽ��g++��ͬ��vc��8�ֽڶ���ġ�Ϊ�˱�֤���ݸ�ʽ������ƽ̨��
��������ͬ����Ҫָ������ʱ������ݶ��뷽ʽ����#pragma pack(x)����ָ����ȷ�����뷽ʽ

**********************************************************************************/

#ifdef WIN32
#pragma warning( disable: 4049 )  /* more than 64k source lines */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4786 ) //identifier was truncated to '255' characters in the debug information
#ifndef NTDDI_WIN7SP1
#define NTDDI_WIN7SP1 0x06010100
#endif
#include <winsock2.h>
#include <windows.h>
#else//WIN32

typedef long long         __int64;

typedef long				LONG;
typedef short				SHORT;
typedef unsigned long		DWORD;
typedef int					BOOL;
typedef int					HMODULE;
typedef unsigned char		BYTE;
typedef unsigned char		byte;
typedef unsigned short		WORD;
#define DWORD_PTR			DWORD
typedef unsigned short		WCHAR;
typedef unsigned int		UINT;
//typedef unsigned int		size_t;
typedef const char*			LPCTSTR;
typedef char*				LPTSTR;
typedef const char*			LPCSTR;
typedef char*				LPSTR;
typedef char				TCHAR;
typedef void*				LPVOID;

// Standard constants
#undef FALSE
#undef TRUE
#undef NULL

#define FALSE   0
#define TRUE    1
#define NULL    0

#define _ULONGLONG_
#ifndef _ULONGLONG_


typedef __int64           LONGLONG;

typedef unsigned __int64  ULONGLONG;

typedef LONGLONG          *PLONGLONG;

typedef ULONGLONG         *PULONGLONG;

#endif // _ULONGLONG_


typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT, *LPRECT;

typedef struct tagSIZE
{
    LONG        cx;
    LONG        cy;
} SIZE, *PSIZE, *LPSIZE;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, *LPPOINT;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))


#ifndef MAX_PATH
#define MAX_PATH  256
#endif

#ifndef MAX_STRING
#define MAX_STRING    256
#endif

#endif //WIN32

#ifndef ASSERT
    #ifdef _DEBUG
        #ifdef WIN32
            #define ASSERT(f) if(!(f))_asm { int 3 };
        #else
            #include <cassert>
            #define ASSERT(x) if(!(x))TRACE("ASSERT %s:%d!!!\n", __FILE__, __LINE__);
        #endif//WIN32
    #else
        #define ASSERT(x) ((void)0);
    #endif //_DEBUG
#endif//ASSERT

#ifdef WIN32
#define atoi64(x)	_atoi64(x)
#else
#define atoi64(x)	atoll(x)
#endif

#endif //__WINDOWS_DEFINE_H__


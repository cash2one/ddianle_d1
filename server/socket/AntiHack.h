//AntiHack.h

#include "windefine.h"

//�г������������õ���ģ��
void PrintModules( unsigned long processID );

//�õ���ǰִ���ļ���md5
bool GetModuleMD5(HMODULE hMode, unsigned char digest[16]);


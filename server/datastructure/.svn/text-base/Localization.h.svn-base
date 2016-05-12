#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__

#include "../datastructure/Macro_Define.h"
#include <map>
#include <string>
#include "../datastructure/DataStruct_Base.h"

class CLocalization
{
	CLocalization();
public:
	~CLocalization();
	//获取实例
	static CLocalization & Instance();
	//初始化实例
	bool Init();
	//获取字符串内容
	std::string GetString(std::string strKey);
	std::string GetString(std::string strKey, unsigned int nMaxStringSize);
private:
	//注册相关的本地化字符串
	void _Register();
	//加载
	bool _Load();
	//检查
	bool _Check();
	//字符串对应表
	std::map< std::string, std::string > m_stringMap;
};

#endif//__LOCALIZATION_H__


//file end


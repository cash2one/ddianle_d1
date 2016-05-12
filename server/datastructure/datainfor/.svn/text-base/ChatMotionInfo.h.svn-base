#ifndef __ChATMOTIONINFO_H__
#define __ChATMOTIONINFO_H__

#define ChatMotionName  "/data/chatmotion.bytes"

#include <string>

using namespace std;

class CFileStream;

class CChatMotionInfo
{
public:
	CChatMotionInfo();
	~CChatMotionInfo();
public:
	short m_motionID;		//UI表情ID
	string m_spriteName;	//精灵名
	string m_atlasName;		//图集名

	bool Load(CFileStream&file);
	bool Save(CFileStream&file);
};

#endif


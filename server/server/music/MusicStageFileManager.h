#ifndef _MUSIC_STAGE_FILE_MANAGER_H_
#define _MUSIC_STAGE_FILE_MANAGER_H_

#include "../../datastructure/Macro_Define.h"

#include <string>
#include <vector>

class CMusicStageFileManager
{
private:
    CMusicStageFileManager();
    virtual ~CMusicStageFileManager();
protected:
    static CMusicStageFileManager &Instance();

public:
    static void GetStageFileName(
        const std::string& strMusicName, int nMode, int nLevel, 
        std::string &strStageFile);

    static bool LoadStage(const std::string& strStageFile, Phone_OS osType,
        std::vector<char>& vecAndroidData);

    static bool LoadStage(const std::string& strStageFile, 
        std::vector<char>& vecAndroidData, 
        std::vector<char>& vecIosData, 
        std::vector<char>& vecWinPhoneData);

    static int CalcuStageTime(int nMode,
        std::vector<char>& vecAndroidData,
        std::vector<char>& vecIosData,
        std::vector<char>& vecWinPhoneData);

protected:
    static bool _LoadFile(const std::string &strPath, std::vector<char>& vecData);
    static int _AnalyseStageTime(const std::string& strTimeFlag, std::vector<char>& vecStageData);
};

#endif //_MUSIC_STAGE_FILE_MANAGER_H_


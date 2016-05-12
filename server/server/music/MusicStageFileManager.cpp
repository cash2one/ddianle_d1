#include "MusicStageFileManager.h"
#include "../util/CommonFunc.h"
#include "../../socket/Log.h"

#include <algorithm>
#include <fstream>
#include <map>

extern string g_sPath;


CMusicStageFileManager::CMusicStageFileManager()
{

}

CMusicStageFileManager::~CMusicStageFileManager()
{

}

CMusicStageFileManager &CMusicStageFileManager::Instance()
{
    static CMusicStageFileManager s_instance;
    return s_instance;
}

void CMusicStageFileManager::GetStageFileName(
    const std::string& strMusicName, int nMode, int nLevel, 
    std::string &strStageFile)
{
    if ( nMode == MusicMode_Taiko )
    {
        strStageFile.assign( "taigu/" );
        strStageFile.append( strMusicName );
        strStageFile.append( ".tg" );
    }
    else if ( nMode == MusicMode_Tradition )
    {
        strStageFile.assign( "tradition/" );
        strStageFile.append( strMusicName );
        strStageFile.append( ".td" );
    }
    else if( nMode == MusicMode_Osu )
    {
        strStageFile.assign( "osu/" );
        strStageFile.append( strMusicName );
        strStageFile.append( ".os" );

    }
    else if ( nMode == MusicMode_Rhythm )
    {
        strStageFile.assign( "rhythm/" );
        strStageFile.append( strMusicName );
        strStageFile.append( ".rh" );
    }
    else if ( nMode == MusicMode_Sweethearts )
    {
        strStageFile.assign( "heartbeats/" );
        strStageFile.append( strMusicName );
        strStageFile.append( ".hb" );
    }
    else if ( nMode == MusicMode_ADB )
    {
        strStageFile.assign( "au/" );
        strStageFile.append( strMusicName );
        strStageFile.append( ".au" );
    }
    else if ( nMode == MusicMode_RainBow )
    {
        strStageFile.assign( "rainbow/" );
        strStageFile.append( strMusicName );
        strStageFile.append( ".rb" );
    }
	else if ( nMode == MusicMode_SuperOsu )
	{
		strStageFile.assign( "superosu/" );
		strStageFile.append( strMusicName );
		strStageFile.append( ".so" );
	}

    if ( nLevel == MusicLevel_Easy )
    {
        strStageFile.append( "e" );
    }
    else if ( nLevel == MusicLevel_Normal )
    {
        strStageFile.append( "n" );
    }
    else
    {
        strStageFile.append( "h" );
    }
}

bool CMusicStageFileManager::LoadStage(
    const std::string& strStageFile, 
    std::vector<char>& vecAndroidData, 
    std::vector<char>& vecIosData, 
    std::vector<char>& vecWinPhoneData)
{
    bool bRes = false;
    std::string strStagePath;

    // Android
    bRes = LoadStage(strStageFile, OS_Android, vecAndroidData);
    if ( !bRes )
    {
        WriteLog(LOGLEVEL_ERROR, "[ERROR] LoadStage: load Android stage file failed. (%s)", 
            strStagePath.c_str());
        return bRes;
    }

    // iOS
    bRes = LoadStage(strStageFile, OS_Ios, vecIosData);
    if ( !bRes )
    {
        WriteLog(LOGLEVEL_ERROR, "[ERROR] LoadStage: load iOS stage file failed. (%s)", 
            strStagePath.c_str());
        return bRes;
    }

    // WinPhone
    //bRes = LoadStage(strStageFile, OS_WP8, vecWinPhoneData);
    //if ( !bRes )
    //{
    //    WriteLog(LOGLEVEL_ERROR, "[ERROR] LoadStage: load WinPhone stage file failed. (%s)", 
    //        strStagePath.c_str());
    //    return bRes;
    //}
    vecWinPhoneData = vecAndroidData; // 关闭WindowsPhone平台，不再读取WindowsPhone关卡，直接使用Android关卡替代

    return bRes;
}

bool CMusicStageFileManager::LoadStage(const std::string& strStageFile, Phone_OS osType,
    std::vector<char>& vecAndroidData)
{
    bool bRes = false;
    std::string strStagePath;
    std::string strOSPath;

    switch (osType)
    {
    case OS_Android: strOSPath.assign("android/"); break;
    case OS_Ios: strOSPath.assign("ios/"); break;
    case OS_WP8: strOSPath.assign("win/"); break;
    case OS_Max:
    case OS_None:
    default: return false;
    }

    strStagePath.assign( g_sPath );
    strStagePath.append( "data/stage/" );
    strStagePath.append( strOSPath );
    strStagePath.append( strStageFile );

    bRes = _LoadFile(strStagePath, vecAndroidData);

    if ( !bRes )
    {
        WriteLog(LOGLEVEL_ERROR, "[ERROR] LoadStage: load (%s) stage file failed. (%s)", 
            strOSPath.c_str(), strStagePath.c_str());
    }

    return bRes;
}

bool CMusicStageFileManager::_LoadFile(const std::string &strPath, std::vector<char>& vecData)
{
    static std::map<std::string, std::vector<char> > cachedStageData;

    if (cachedStageData.find(strPath) != cachedStageData.end())
    {
        vecData = cachedStageData.find(strPath)->second;
        return true;
    }

    std::ifstream fileStream;
    fileStream.open( strPath.c_str(), ios_base::binary );
    if ( fileStream.good() )
    {
        fileStream.seekg( 0, ios_base::end );

        int nFileSize = (int)fileStream.tellg();
        if ( nFileSize > 0 )
        {
            vecData.clear();
            vecData.resize( nFileSize );

            fileStream.seekg( 0, ios_base::beg );
            fileStream.read( &vecData[0], nFileSize );

            cachedStageData[strPath] = vecData;

            return true;
        }
    }
    fileStream.close();

    return false;
}

int CMusicStageFileManager::CalcuStageTime(int nMode, std::vector<char>& vecAndroidData, std::vector<char>& vecIosData, std::vector<char>& vecWinPhoneData)
{
    std::string strTimeFlag = "";

    if ( nMode == MusicMode_Taiko )
    {
        strTimeFlag.assign( "MATCHTIME:" );
    }
    else if ( nMode == MusicMode_Tradition )
    {
        strTimeFlag.assign( "MATCHTIME:" );
    }
    else if ( nMode == MusicMode_Osu )
    {
        strTimeFlag.assign( "Tags:" );
    }
    else if ( nMode == MusicMode_ADB )
    {
        strTimeFlag.assign( "MATCHTIME:" );
    }
    else if ( nMode == MusicMode_RainBow )
    {
        strTimeFlag.assign( "MATCHTIME:" );
    }
	else if ( nMode == MusicMode_SuperOsu )
	{
		 strTimeFlag.assign( "Tags:" );
	}

    int nAndroidTime = _AnalyseStageTime( strTimeFlag, vecAndroidData );
    int nIosTime = _AnalyseStageTime( strTimeFlag, vecIosData );
    int nWinPhoneTime = _AnalyseStageTime( strTimeFlag, vecWinPhoneData );

    int nStageTime = ( nAndroidTime < nIosTime ? nAndroidTime : nIosTime );
    nStageTime = ( nStageTime < nWinPhoneTime ? nStageTime : nWinPhoneTime );
    return nStageTime;
}

int CMusicStageFileManager::_AnalyseStageTime(
    const std::string& strTimeFlag, std::vector<char>& vecStageData)
{
    int nStageTime = 0;

    int nFlagLen = (int)strTimeFlag.length();
    if ( nFlagLen > 0 )
    {
        int nDataLen = (int)vecStageData.size();
        for ( int i = 0; i < nDataLen; ++i )
        {
            int nDataIndex = i;
            int nFlagIndex = 0;
            while ( nDataIndex < nDataLen && nFlagIndex < nFlagLen )
            {
                if ( vecStageData[nDataIndex] != strTimeFlag[nFlagIndex] )
                {
                    break;
                }

                ++nDataIndex;
                ++nFlagIndex;
            }

            if ( nFlagIndex == nFlagLen )
            {
                int nTimeIndex = nDataIndex;
                while ( nTimeIndex < nDataLen )
                {
                    if ( !isdigit( vecStageData[nTimeIndex] ) )
                    {
                        break;
                    }

                    ++nTimeIndex;
                }

                std::string strStageTime( &vecStageData[nDataIndex], nTimeIndex - nDataIndex );
                nStageTime = atoi( strStageTime.c_str() );

                break;
            }
        }
    }

    return nStageTime;
}


/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\MagicLampConfigMgr.h
    author:     

    purpose:    对魔法神灯配置数据进行管理
*********************************************************************/

#ifndef __MAGICLAMPCONFIGMGR_H__
#define __MAGICLAMPCONFIGMGR_H__
#ifdef WIN32
#pragma warning(disable:4018)
#pragma warning(disable:4100)
#endif

#include <string>
#include <map>

#include "MagicLampConfig.h"
#include "./FileStream.h"

//////////////////////////////////////////////////////////////////////////
///魔法神灯数据管理
//////////////////////////////////////////////////////////////////////////

class CFileStream;
class CMagicLampConfigMgr
{
public:
    CMagicLampConfigMgr(){};
    virtual ~CMagicLampConfigMgr();

public:
    // 加载魔法神灯配置
    bool LoadMagicLampConfig( const char * pFileName );

    // 销毁
    void Destroy();

    // 获取BOSS 装备配置
    CBossConfig * GetAIBossInfo( unsigned int nBossID );
    unsigned int GetMaxBossID();

    // 获取副本配置
    CDungeonConfig * GetDungeonConfig( unsigned int nDungeonID );
    unsigned int GetMaxMainlineID();
    CThemeConfig * GetThemeConfig( unsigned int nThemeID );

    std::map<unsigned int,CThemeParcloseConfig*> * GetThemeParcloseList( unsigned int nThemeID );
    unsigned int GetMaxThemeID();

    // 获取副本ID
    unsigned int GetDungeonIDByParcloseID( unsigned int nParcloseID );
    std::string GetDungeonNameByParcloseID( unsigned int nParcloseID );

    // 获取主线副本列表
    const CMainlineConfigTable & GetDungeonConfigTable(){ return m_DungeonConfig; }

    // 获取主题列表
    const std::map<unsigned int,CThemeConfig*> & GetThemeConfigList(){ return m_ThemeConfig; }

    // 获取关卡配置
    CParcloseConfig * GetMainlineParcloseConfig( unsigned int nParcloseID );
    CParcloseConfig * GetParcloseConfig( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID );
    const std::map<unsigned int,CParcloseConfig*> & GetParcloseConfigList(){ return m_ParcloseConfig; }
    CThemeParcloseConfig * GetThemeParcloseConfig( unsigned int nThemeID, unsigned int nParcloseID );
    CMainlineDropConfig * GetParcloseDropInfo( unsigned int nParclose );

    // 获取下一个关卡
    unsigned int GetMainlineNextParcloseID( unsigned int nParcloseID );
    unsigned int GetThemeNextParcloseID( unsigned int nThemeID, unsigned int nParcloseID );
    unsigned int GetNextParcloseID( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID );

    // 获取第一个关卡ID
    unsigned int GetFirstParcloseID();
    unsigned int GetThemeFirstParcloseID( unsigned int nThemeID );

    // 获取剧情
    CStoryConfig * GetStoryConfig( unsigned int nStoryID );

    // 获取目标
    CTargetConfig * GetTargetConfig( unsigned int nTargetID );

    // 获取主线掉落
    CMainlineDropConfig * GetMainlineDrop( unsigned int nParcloseID );
    std::map<unsigned int,CRubbishDropConfig*> & GetMainlineRubbishDrop(){ return m_MainlineRubbishDropConfig; }
    CThemeDropConfig * GetThemeDrop( unsigned int nThemeID, unsigned int nParcloseID );
    std::map<unsigned int,CRubbishDropConfig*> & GetThemeRubbishDrop(){ return m_ThemeRubbishDropConfig; }

    // 获取碎片
    CDebrisConfig * GetDebrisConfig( unsigned int nDebrisID );
    std::map<unsigned int,CDebrisConfig*> & GetDebrisConfigList(){ return m_DebrisConfig; }

private:
    std::map<int,CBossConfig*>                                           m_BossConfig;
    std::map<unsigned int,CDungeonConfig*>                               m_DungeonConfig;
    std::map<unsigned int,CParcloseConfig*>                              m_ParcloseConfig;
    std::map<unsigned int,CMainlineDropConfig*>                          m_MainlineDropConfig;
    std::map<unsigned int,CRubbishDropConfig*>                           m_MainlineRubbishDropConfig;
    std::map<unsigned int,CThemeConfig*>                                 m_ThemeConfig;
    std::map<unsigned int,std::map<unsigned int,CThemeParcloseConfig*> > m_ThemeParcloseConfig;
    std::map<unsigned int,std::map<unsigned int,CThemeDropConfig*> >     m_ThemeDropConfig;
    std::map<unsigned int,CRubbishDropConfig*>                           m_ThemeRubbishDropConfig;
    std::map<unsigned int,CStoryConfig*>                                 m_StoryConfig;
    std::map<unsigned int,CTargetConfig*>                                m_TargetConfig;
    std::map<unsigned int,CDebrisConfig*>                                m_DebrisConfig;

};


#endif



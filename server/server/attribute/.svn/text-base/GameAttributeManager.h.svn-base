#ifndef __GAMSERVER_GAMEATTRIBUTEMANAGER_H__
#define __GAMSERVER_GAMEATTRIBUTEMANAGER_H__

class GameAttributeManager
{
    GameAttributeManager();
public:
    ~GameAttributeManager();

    static GameAttributeManager& Instance(); // 


    void Initialize(); //
    void OnUpdate(const unsigned long &lTimeElapsed);


    // 刷新时间：

private:
    bool m_bIsOpen;
    int m_nActIndex; // 开启状态下还要检测活动所属于的周期
};


#endif // __GAMSERVER_GAMEATTRIBUTEMANAGER_H__

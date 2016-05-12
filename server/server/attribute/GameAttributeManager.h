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


    // ˢ��ʱ�䣺

private:
    bool m_bIsOpen;
    int m_nActIndex; // ����״̬�»�Ҫ��������ڵ�����
};


#endif // __GAMSERVER_GAMEATTRIBUTEMANAGER_H__

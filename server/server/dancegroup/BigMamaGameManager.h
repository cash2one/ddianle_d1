#ifndef __GAMESERVER_BIGMAMAGAMEMANAGER_H__
#define __GAMESERVER_BIGMAMAGAMEMANAGER_H__


class BigMamaGameManager
{
private:
    BigMamaGameManager();

public:
    ~BigMamaGameManager();

    BigMamaGameManager& Instance() {
        static BigMamaGameManager s;
        return s;
    }

    bool IsOpen(); // ÊÇ·ñ¿ªÆô
    


private:
    
};




#endif // __GAMESERVER_BIGMAMAGAMEMANAGER_H__
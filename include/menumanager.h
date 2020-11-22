#include "olcPixelGameEngine.h"
#include "menuobject.h"

class menumanager
{
public:
    menumanager() { }

    void Open(menuobject* mo);
    void Close();

    void OnUp();
    void OnDown();
    void OnLeft();
    void OnRight();
    void OnBack();

    menuobject* OnConfirm();
    
    void Draw(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset);

private:
    std::list<menuobject*> panels;
};
#include "retromenu.h"

RetroMenu::RetroMenu()
{
    // Load Sprites
    sprGFX = new olc::Sprite("./files/sprites/RetroMenuSprite.png");
    sprLOGO = new olc::Sprite("./files/sprites/MenuLogoSprite.png");
        
    // Create Menu Tree
    mo["main"].SetID(10000).SetTable(1, 5);

    mo["main"]["2D Map"].SetTable(1, 4);
    auto& menuPick2DMap = mo["main"]["2D Map"];
    menuPick2DMap["Fermi-Ulam"].SetID(SET_2DMAP_FERMIULAM);
    menuPick2DMap["Halley"].SetID(SET_2DMAP_HALLEY);
    menuPick2DMap["Henon"].SetID(SET_2DMAP_HENON);
    menuPick2DMap["Standard"].SetID(SET_2DMAP_STANDARD);
    menuPick2DMap["Standard Non-Twist"].SetID(SET_2DMAP_STDNONTWST);

    mo["main"]["1D Map"].SetTable(1, 4);
    auto& menuPick1DMap = mo["main"]["1D Map"];
    menuPick1DMap["Linear Sine"].SetID(SET_1DMAP_LINEARSIN);
    menuPick1DMap["Logistic"].SetID(SET_1DMAP_LOGISTIC);
    menuPick1DMap["Logistic (2nd Order)"].SetID(SET_1DMAP_LOGISTIC2ND);
    menuPick1DMap["Moran"].SetID(SET_1DMAP_MORAN);
    menuPick1DMap["Triangular"].SetID(SET_1DMAP_TRIANGLE);
    
    mo["main"]["Buy DLC"].Enable(false);
    mo["main"]["Donate"].Enable(false);
    mo["main"]["Exit"].SetID(EXIT_ALL);

    mo.Build();

    mm.Open(&mo["main"]);
}

int RetroMenu::run_frame(olc::PixelGameEngine& pge)
{
    pge.Clear(olc::Pixel(204, 229, 255));
    menuobject* command = nullptr;

    // Handle Key events   
    if (pge.GetKey(olc::Key::M).bPressed)     { mm.Open(&mo["main"]);    }
    if (pge.GetKey(olc::Key::UP).bPressed)    { mm.OnUp();    }
    if (pge.GetKey(olc::Key::DOWN).bPressed)  { mm.OnDown();  }
    if (pge.GetKey(olc::Key::LEFT).bPressed)  { mm.OnLeft();  }
    if (pge.GetKey(olc::Key::RIGHT).bPressed) { mm.OnRight(); }
    if (pge.GetKey(olc::Key::Z).bPressed)     { mm.OnBack();  }
    if (pge.GetKey(olc::Key::SPACE).bPressed ||
        pge.GetKey(olc::Key::ENTER).bPressed) { command = mm.OnConfirm(); }

    // Handle command
    if (command != nullptr)
    {
        sLastAction = "Selected: " + command->GetName() + "ID: " + std::to_string(command->GetID());
        mm.Close();

        return command->GetID();
    }

    // Draw Logo
    olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
    pge.SetPixelMode(olc::Pixel::ALPHA);
    int32_t LogoScale = 3 * nPixelScale;
    olc::vi2d vLogoPosition = {(pge.ScreenWidth() - LogoScale * sprLOGO->width) / 2, 10};
    pge.DrawSprite(vLogoPosition, sprLOGO, LogoScale);
    pge.SetPixelMode(currentPixelMode);
        
    // Print Current Version
    pge.DrawString(0.78 * pge.ScreenWidth(), 0.97 * pge.ScreenHeight(), "Version 0.0.1", olc::DARK_GREY, 2);

    // Draw Menu
    olc::vi2d PanelSize = nPatch * mo["main"].GetSizeInPatches();
    mm.Draw(pge, sprGFX, {(pge.ScreenWidth() - PanelSize.x) / 2, 
                          (pge.ScreenHeight() - PanelSize.y) / 2});

    // Print last command
    pge.DrawString(10, 0.97 * pge.ScreenHeight(), sLastAction, olc::BLACK, 2);

    return RUN_MENU;
}
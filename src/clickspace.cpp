#include "clickspace.h"

bool olcClickSpace::OnUserCreate()
{
    // Load Sprites
    menu.sprGFX  = new olc::Sprite("./files/sprites/RetroMenuSprite.png");
    menu.sprLOGO = new olc::Sprite("./files/sprites/MenuLogoSprite.png");
    sprLogoIFUSP = new olc::Sprite("./files/sprites/LogoIFUSP.png");
    sprLogoUSP   = new olc::Sprite("./files/sprites/LogoUSP.png");
        
    // Create Menu Tree
    menu.mo["main"].SetID(RUN_MENU).SetTable(1, 8);

    menu.mo["main"]["2D Map"].SetTable(1, 4);
    auto& menuPick2DMap = menu.mo["main"]["2D Map"];
    menuPick2DMap["Fermi-Ulam"].SetID(SET_2DMAP_FERMIULAM);
    menuPick2DMap["Halley"].SetID(SET_2DMAP_HALLEY);
    menuPick2DMap["Henon"].SetID(SET_2DMAP_HENON);
    menuPick2DMap["Standard"].SetID(SET_2DMAP_STANDARD);
    menuPick2DMap["Standard Non-Twist"].SetID(SET_2DMAP_STDNONTWST);

    menu.mo["main"]["1D Map"].SetTable(1, 4);
    auto& menuPick1DMap = menu.mo["main"]["1D Map"];
    menuPick1DMap["Linear Sine"].SetID(SET_1DMAP_LINEARSIN);
    menuPick1DMap["Logistic"].SetID(SET_1DMAP_LOGISTIC);
    menuPick1DMap["Logistic (2nd Order)"].SetID(SET_1DMAP_LOGISTIC2ND);
    menuPick1DMap["Moran"].SetID(SET_1DMAP_MORAN);
    menuPick1DMap["Triangular"].SetID(SET_1DMAP_TRIANGLE);

    menu.mo["main"]["Automaton"].SetID(RUN_CELLAUTO);
    
    menu.mo["main"]["Controls"].Enable(false);
    menu.mo["main"]["Buy DLC"].Enable(false);
    menu.mo["main"]["Donate"].Enable(false);
    menu.mo["main"]["Credits"].SetID(RUN_CREDITS);
    menu.mo["main"]["Exit"].SetID(EXIT_ALL);

    menu.mo.Build();

    menu.mm.Open(&menu.mo["main"]);

    return true;
}

bool olcClickSpace::OnUserUpdate(float fElapsedTime)
{   
    switch (RunningID)
    {
        case RUN_MENU:
            RunningID = menu.run_frame(*this);
            if (RunningID > SET_2DMAP &&
                RunningID < SET_1DMAP)
            {
                map2D.SetApplication((*this), RunningID);
                RunningID = RUN_2DMAP;
            }
            if (RunningID > SET_1DMAP)
            {
                map1D.SetApplication((*this), RunningID);
                RunningID = RUN_1DMAP;
            }
            if (RunningID == RUN_CELLAUTO)
            {
                cellauto.SetApplication((*this));
            }
            break;
        case RUN_2DMAP:
            RunningID = map2D.run_frame(*this);
            if (RunningID == RUN_MENU) { menu.mm.Open(&menu.mo["main"]); }
            else if (RunningID > SET_2DMAP && RunningID < LOCAL_2DMAP_MENU)
            {
                map2D.SetApplication((*this), RunningID);
                RunningID = RUN_2DMAP;
            }
            break;
        case RUN_1DMAP:
            RunningID = map1D.run_frame(*this);
            if (RunningID == RUN_MENU) { menu.mm.Open(&menu.mo["main"]); }
            break;
        case RUN_CELLAUTO:
            RunningID = cellauto.run_frame(*this);
            if (RunningID == RUN_MENU) { menu.mm.Open(&menu.mo["main"]); }
            break;
        case RUN_CREDITS:
            RunningID = run_credits();
            break;
        case EXIT_ALL:
            return false;
        default:
            break;
    }

    return true;
}

int olcClickSpace::run_credits()
{
    Clear(olc::VERY_DARK_GREY);

    if (GetKey(olc::ESCAPE).bPressed) 
    {
        menu.mm.Open(&menu.mo["main"]);
        return RUN_MENU; 
    }

    // Institution logos
    olc::Pixel::Mode currentPixelMode = GetPixelMode();
    SetPixelMode(olc::Pixel::ALPHA);
    olc::vi2d vPosLogoIFUSP = {2, ScreenHeight() - sprLogoIFUSP->height - 2};
    olc::vi2d vPosLogoUSP = {ScreenWidth() - sprLogoUSP->width - 2, 
                             ScreenHeight() - sprLogoUSP->height - 2};
    uint8_t nLogoScale = 1;
    DrawSprite(vPosLogoIFUSP, sprLogoIFUSP, nLogoScale);
    DrawSprite(vPosLogoUSP,   sprLogoUSP,   nLogoScale);
    SetPixelMode(currentPixelMode);

    // Draw Texts
    std::vector<std::string> Lines(0);
    Lines.push_back("Made by");
    Lines.push_back("Scholars from the University of Sao Paulo");
    Lines.push_back(" ");
    Lines.push_back("Matheus Lazarotto");
    Lines.push_back(" ");
    Lines.push_back("Vitor Oliveira");
    Lines.push_back(" ");
    Lines.push_back("Matheus Palmero");
    Lines.push_back("\n");

    Lines.push_back("Made with");
    Lines.push_back(" ");
    Lines.push_back("olc::PixelGameEngine");
    Lines.push_back("\n");

    Lines.push_back("Acknowledgments to");
    Lines.push_back(" ");
    Lines.push_back("the Great, the One Lone Coder");
    Lines.push_back(" ");
    Lines.push_back("Javidx9");

    int nPaddingY = 40;
    int nTextScale = 3;
    for (auto& l : Lines)
    {
        olc::vi2d TextProp = GetTextSizeProp(l);
        DrawString((ScreenWidth() - TextProp.x * nTextScale) / 2, nPaddingY, l, olc::WHITE, nTextScale);
        nPaddingY += TextProp.y * nTextScale;
    }
    
    //// TO DO: Put text inside a .txt file to read and     ////
    ////        automatically print the credit line by line ////

    return RUN_CREDITS;
}
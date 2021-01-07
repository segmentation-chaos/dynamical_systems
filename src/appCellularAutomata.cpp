#include "appCellularAutomata.h"

appCellularAutomata::appCellularAutomata()
{

}

appCellularAutomata::~appCellularAutomata()
{
    // for (unsigned int j = 0; j < CellsState.size(); j += 1)
    // {
        // CellsState[j].clear();
        // StoreState[j].clear();
    // }
    // CellsState.clear();
    // StoreState.clear();
}

void appCellularAutomata::SetApplication(olc::PixelGameEngine& pge)
{
    cScreenSize.x = pge.ScreenWidth(); 
    cScreenSize.y = pge.ScreenHeight(); 

    cGridOffset.x = (cScreenSize.x - cGridSize.x * PixelSize) / 2;
    cGridOffset.y = (cScreenSize.y - cGridSize.y * PixelSize) / 2;

    CellsState.resize(cGridSize.y + 2, std::vector<int8_t>(0));
    StoreState.resize(cGridSize.y + 2, std::vector<int8_t>(0));

    SetRandomState();
}

void appCellularAutomata::EvolveState()
{
    // Store output state
    for (int i = 1; i <= cGridSize.x; i += 1)
    {
        for (int j = 1; j <= cGridSize.y; j += 1)
        {
            StoreState[j][i] = CellsState[j][i];
        }
    }

    // Evolve cells
    for (int i = 1; i <= cGridSize.x; i++)
    {
        for (int j = 1; j <= cGridSize.y; j++)
        {
            int8_t nNeighbours = StoreState[j-1][i-1] + StoreState[j-1][i+0] + StoreState[j-1][i+1] +
                                 StoreState[j+0][i-1] +          0           + StoreState[j+0][i+1] +
                                 StoreState[j+1][i-1] + StoreState[j+1][i+0] + StoreState[j+1][i+1];

            if (StoreState[j][i] == 1)
            { 
                if (nNeighbours != 2 && nNeighbours != 3) { CellsState[j][i] = 0; }
            }
            else if (StoreState[j][i] == 0)
            {
                if (nNeighbours == 3) { CellsState[j][i] = 1; }
            }
        }
    }
}

void appCellularAutomata::SetRandomState()
{
    // Set grid memory and random state
    srand((unsigned) time(0));
    for (int j = 0; j < (cGridSize.y + 2); j += 1)
    {
        CellsState[j].resize(cGridSize.x + 2);
        StoreState[j].resize(cGridSize.x + 2);
        for (int i = 0; i < (cGridSize.x + 2); i += 1)
        {
            // Borders
            if (i == 0 || i == (cGridSize.x + 1) ||
                j == 0 || j == (cGridSize.y + 1))
            {
                CellsState[j][i] = -1;
                StoreState[j][i] = -1;
            }
            else
            {
                int r = rand() % 2 == 0;
                CellsState[j][i] = r;
                StoreState[j][i] = r;
            }            
        }
    }

    // [h+2] and [w+2] are used in order to get free borders
    // at [0] and [h+1].
    // CellsState[0] and CellsState[h+1] are never dealt 
    // with. However, periodic conditions can be implemented 
    // in the EvolveState() function (you may also create 
    // your own, customized, EvolveState()).
}

void appCellularAutomata::DrawGrid(olc::PixelGameEngine& pge)
{
    // Vertical Lines
    for (int i = 1; i <= cGridSize.x; i += 1)
    {
        olc::vi2d LineA = {PixelSize * i, PixelSize};
        olc::vi2d LineB = {PixelSize * i, PixelSize * (cGridSize.y + 1)};
        pge.DrawLine(LineA + cGridOffset, LineB + cGridOffset, gridRGBA);
    }
    // Horizontal Lines
    for (int j = 1; j <= cGridSize.y; j += 1)
    {
        olc::vi2d LineA = {PixelSize, PixelSize * j};
        olc::vi2d LineB = {PixelSize * (cGridSize.x + 1), PixelSize * j};
        pge.DrawLine(LineA + cGridOffset, LineB + cGridOffset, gridRGBA);
    }
}

void appCellularAutomata::DrawState(olc::PixelGameEngine& pge)
{
    // Draw living cells from current State
    for (int i = 1; i <= cGridSize.x; i += 1)
    {
        for (int j = 1; j <= cGridSize.y; j += 1)
        {
            if (CellsState[j][i] == 1)
            {
                olc::vi2d CellPos = {PixelSize * i, PixelSize * j};
                olc::vi2d CellSize = {PixelSize, PixelSize};
                pge.FillRect(CellPos + cGridOffset, CellSize, cellRGBA);
            }
        }
    }

    // Draw border frame
    olc::vi2d OuterLayer = {cGridOffset.x + PixelSize, cGridOffset.y + PixelSize};
    olc::vi2d InnerLayer = {cGridOffset.x + PixelSize + 1, cGridOffset.y + PixelSize + 1};
    olc::vi2d BorderSize = {PixelSize * cGridSize.x, PixelSize * cGridSize.y};
    pge.DrawRect(OuterLayer, BorderSize, gridRGBA);
    pge.DrawRect(InnerLayer, BorderSize, gridRGBA);
}

int appCellularAutomata::run_frame(olc::PixelGameEngine& pge)
{
    // Return to main menu
    if (pge.GetKey(olc::Key::ESCAPE).bPressed) 
    {   
        return RUN_MENU; 
    }

    // Toggle color modes
    if (pge.GetKey(olc::Key::N).bPressed)
    {
        ColorMode = (ColorMode + 1) % 3;
        switch (ColorMode)
        {
            case 0:
                backgroundRGBA = NightModeBackgroundRGBA;
                gridRGBA = NightModeGridRGBA;
                cellRGBA = NightModeCellRGBA;
                break;
            case 1:
                backgroundRGBA = GrayModeBackgroundRGBA;
                gridRGBA = GrayModeGridRGBA;
                cellRGBA = GrayModeCellRGBA;
                break;
            case 2:
                backgroundRGBA = LightModeBackgroundRGBA;
                gridRGBA = LightModeGridRGBA;
                cellRGBA = LightModeCellRGBA;
                break;
        }
    }

    // Toggle grid
    if (pge.GetKey(olc::Key::G).bPressed)
    {
        draw_grid = !draw_grid;
    }

    // Toggle pause
    if (pge.GetKey(olc::Key::SPACE).bPressed)
    {
        pause = !pause;
    }

    // Reset Random state
    if (pge.GetKey(olc::Key::R).bPressed)
    {
        SetRandomState();
    }

    // Clear and Draw
    pge.Clear(backgroundRGBA);
    DrawState(pge);

    // Iterate OR edit state
    if (!pause)
    {
        EvolveState();
    }
    else
    {
        // Highlight cell
        olc::vi2d GridMousePos = {(pge.GetMouseX() - cGridOffset.x - 1) / PixelSize, 
                                  (pge.GetMouseY() - cGridOffset.y - 1) / PixelSize};
        
        if (0 <= GridMousePos.x && GridMousePos.x <= cGridSize.x &&
            0 <= GridMousePos.y && GridMousePos.y <= cGridSize.y)
        {
            pge.DrawRect(PixelSize * GridMousePos + cGridOffset, {PixelSize+1, PixelSize+1}, highlightcellRGBA);

            // Give birth to cell
            if (pge.GetMouse(0).bHeld)
            {
                CellsState[GridMousePos.y][GridMousePos.x] = 1;
            }
            // Kill cell
            if (pge.GetMouse(1).bHeld)
            {
                CellsState[GridMousePos.y][GridMousePos.x] = 0;
            }
        }

        if (pge.GetKey(olc::Key::RIGHT).bPressed)
        {
            EvolveState();
        }
    }

    // Draw grid
    if (draw_grid) { DrawGrid(pge); }

    //// TO DO: INCREASE|DECREASE EVOLVE RATE ////

    int nTextScale = 3;
    pge.FillRect({0,  0}, {pge.ScreenWidth(), 8 * 3 * nTextScale}, olc::VERY_DARK_GREY);
    pge.DrawLine({0, 8 * 3 * nTextScale}, {pge.ScreenWidth(), 8 * 3 * nTextScale}, olc::WHITE);
    std::string HeaderText = "Cellular Automata - Conway's Game of Life";
    int nTextSize = HeaderText.size();
    pge.DrawString({(pge.ScreenWidth() - nTextSize * 8 * nTextScale) / 2, 8 * nTextScale}, HeaderText, olc::WHITE, nTextScale);

    return RUN_CELLAUTO;
}
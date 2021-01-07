#include <ctime>
#include <unordered_map>
#include "command_ids.h"
#include "olcPixelGameEngine.h"

class appCellularAutomata
{
public:
    appCellularAutomata();
    ~appCellularAutomata();
    int run_frame(olc::PixelGameEngine& pge);
    void SetApplication(olc::PixelGameEngine& pge);

private:
    // Flags
    bool pause = false;
    bool draw_grid = false;

    // Cells Grid settings
    olc::vi2d cScreenSize;
    olc::vi2d cGridSize = {100, 100};
    std::vector<std::vector<int8_t>> CellsState;
    std::vector<std::vector<int8_t>> StoreState;

    // Draw functions and Evolution
    int8_t PixelSize = 9;
    olc::vi2d cGridOffset;
    void EvolveState();
    void SetRandomState();
    void DrawGrid(olc::PixelGameEngine& pge);
    void DrawState(olc::PixelGameEngine& pge);

    // Color palette
    int8_t ColorMode = 0;
    olc::Pixel backgroundRGBA = { 50,  50,  55, 255};
    olc::Pixel gridRGBA = { 255, 255, 255, 255};
    olc::Pixel cellRGBA = {   0, 255,   0, 255};
    olc::Pixel highlightcellRGBA = { 255,   0,   0, 150};
    olc::Pixel NightModeBackgroundRGBA = {  50,  50,  55, 255};
    olc::Pixel GrayModeBackgroundRGBA  = { 150, 150, 150, 255};
    olc::Pixel LightModeBackgroundRGBA = { 255, 255, 255, 255};
    olc::Pixel NightModeCellRGBA = {   0, 255,   0, 255};
    olc::Pixel GrayModeCellRGBA  = {   0,   0,   0, 255};
    olc::Pixel LightModeCellRGBA = {   0,   0,   0, 255};
    olc::Pixel NightModeGridRGBA = { 255, 255, 255, 255};
    olc::Pixel GrayModeGridRGBA  = { 255, 255, 255, 255};
    olc::Pixel LightModeGridRGBA = { 100, 100, 100, 255};
};
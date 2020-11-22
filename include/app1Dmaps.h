#include <unordered_map>
#include "command_ids.h"
#include "features.hpp"
#include "olcPixelGameEngine.h"

class app1DMap
{
public:
    app1DMap();
    ~app1DMap();
    int run_frame(olc::PixelGameEngine& pge);
    void SetApplication(olc::PixelGameEngine& pge,
                        int32_t command_id);
private:
    Analysis *anal;
    Maps_1d *map;
    std::unordered_map<std::string, Maps_1d*> map_items;
    std::unordered_map<std::string, Analysis> analysis_items;
    // Relate a map string name to a int ID
    std::unordered_map<int32_t, std::string> IDtable;

    // System scale routines
    void CanvasToSystem();
    void SystemToCanvas();

    // Draw Functions
    void DrawFunction(olc::PixelGameEngine& pge);
    void DrawIdentity(olc::PixelGameEngine& pge);
    void DrawCobWeb(olc::PixelGameEngine& pge);
    void DrawAxes(olc::PixelGameEngine& pge);

    // Calculation limits
    int32_t FunctionPts = 15000;
    int32_t CobWebIter  = 500;

    // Canvas coordinates
    olc::vi2d cPos;
    olc::vi2d cSize;

    // System coordinates
    olc::vd2d sPos;
    olc::vd2d sPosMin;
    olc::vd2d sPosMax;

    // System axis
    olc::vd2d sAxOrigin;
    olc::vd2d sAxSize;

    // Colours Palette
    olc::Pixel identityRGBA = {50, 50, 50, 255};
    olc::Pixel functionRGBA = {0, 50, 200, 255};
    olc::Pixel cobwebRGBA = {255, 0, 0, 255};
    olc::Pixel axesRGBA = {0, 130, 0, 255};
};
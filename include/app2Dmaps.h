#include <unordered_map>
#include "command_ids.h"
#include "features.hpp"
#include "olcPixelGameEngine.h"

class app2DMap
{
public:
    app2DMap();
    ~app2DMap();
    int run_frame(olc::PixelGameEngine& pge);
    void SetApplication(olc::PixelGameEngine& pge, 
                        int32_t command_id);

private:
    Analysis *anal;
    Maps_2d *map;
    std::unordered_map<std::string, Maps_2d*> map_items;
    std::unordered_map<std::string, Analysis> analysis_items;
    // Relate a 2D map string name to an int ID
    std::unordered_map<int32_t, std::string> IDtable;

    // System scale routines
    void CanvasToSystem();
    void SystemToCanvas();
    void SetNewScale();
    
    // Draw Functions
    void DrawPhasePoint(olc::PixelGameEngine& pge);
    void DrawPointLine(olc::PixelGameEngine& pge);
    void DrawZoomRect(olc::PixelGameEngine& pge);
    void DrawOrbits(olc::PixelGameEngine& pge);

    // Dynamics functions
    void IterateMapOnClick(olc::PixelGameEngine& pge);
    void IterateLinePts(olc::PixelGameEngine& pge);
    void SetLinePtsToRun(olc::PixelGameEngine& pge);

    // Orbit points
    long unsigned int orb_size;
    std::vector<int> orb_chunks;
    std::vector<std::vector<double>> orb_ics;
    std::vector<std::vector<double>> orb_pts;
    std::vector<std::vector<double>> line_orb;

    // Canvas coordinates
    olc::vi2d cPos;
    olc::vi2d cSize;
    
    // System coordinates
    olc::vd2d sPos;
    olc::vd2d sPosMin;
    olc::vd2d sPosMax;
    olc::vd2d sPosScaleMin;
    olc::vd2d sPosScaleMax;

    // Zoom rectangle coordinates
    olc::vi2d cZoomFrameA;
    olc::vi2d cZoomFrameB;

    // Initial conditions line
    olc::vi2d cLinePosA;
    olc::vi2d cLinePosB;

private:
    // Flags
    bool run_dynamics = false;
    bool draw_orbits = false;

    bool line_hold = false;
    bool line_quit = false;
    bool line_run = false;
    bool zoom_hold = false;
    bool zoom_set = false;

    // Limits settings        
    int line_pts = 0;
    int max_line_pts = 50;
    int line_iter = 0;
    int line_iter_max = 1500;

    // Drawing colors
    olc::Pixel lineRGBA = {255, 0, 0, 255};
    olc::Pixel zoomRGBA = {0, 128, 255, 255};
    
    // Color mode
    int bNightMode = false;
    olc::Pixel backgroundRGBA = olc::WHITE;
    olc::Pixel pointRGBA = olc::BLACK;
    olc::Pixel NightModeBackgroundRGBA = { 50,  50,  55, 255};
    olc::Pixel LightModeBackgroundRGBA = {255, 255, 255, 255};
    olc::Pixel NightModePointRGBA = {255, 255, 255, 255};
    olc::Pixel LightModePointRGBA = { 10,  10,  10, 255};
};
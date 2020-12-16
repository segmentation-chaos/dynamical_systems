#include <cmath>
#include "olcPixelGameEngine.h"

class ColorWheel
{
public:
    ColorWheel() {}
    ~ColorWheel();
    void SetColorWheel(int wheel_radius, olc::vi2d offset);
    void HandleColorWheel(olc::PixelGameEngine& pge, olc::Pixel& pixel, bool& run);

private:
    // Color chart settings
    uint8_t pixelWidth = 4;
    uint16_t radius;
    olc::vi2d vFrameOffset;
    olc::vi2d vFrameSize;
    olc::vi2d vCircleOffset;
    olc::vi2d vVBarOffset;
    olc::vi2d vVBarSize;
    olc::vi2d vWheelCursor;
    olc::vi2d vBarCursor;
    olc::vi2d vTextRGBOffset;
    olc::vi2d vTextHSVOffset;
    olc::vi2d vSelectedOffset;

    // Flag
    bool enter = true;

    // Color variables
    float H = 0.0;      // Hue
    float S = 0.0;      // Saturation
    float V = 1.0;      // Value (Brightness)
    uint8_t R = 255;    // Red
    uint8_t G = 255;    // Green
    uint8_t B = 255;    // Blue
    uint8_t* vPixelMap; // Store RGBA from wheel

    // Drawing functions
    void CalculateColorWheel();
    void HSVtoRGB(float h, float s, float v);
    void RGBtoHSV(uint8_t r, uint8_t g, uint8_t b);
    void DrawFrame(olc::PixelGameEngine& pge);
    void DrawColorChart(olc::PixelGameEngine& pge);
    void DrawColorWheel(olc::PixelGameEngine& pge);
    void DrawBrightnessBar(olc::PixelGameEngine& pge);
};

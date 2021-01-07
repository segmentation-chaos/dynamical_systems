#include "colorwheel.h"

ColorWheel::~ColorWheel()
{
    delete[] vPixelMap;
}

void ColorWheel::SetColorWheel(int wheel_radius, olc::vi2d offset)
{
    // Calculate pixel map once for a given radius
    radius = wheel_radius;
    vPixelMap = new uint8_t[4 * 2 * radius * 2 * radius];
    CalculateColorWheel();

    // Set features positions
    olc::vi2d vPadding = {5, 5};
    vFrameOffset    = offset;
    vCircleOffset   = vPadding + olc::vi2d{radius, radius} + vFrameOffset;
    vVBarSize       = olc::vi2d{radius / 6, 2 * radius}; 
    vVBarOffset     = vCircleOffset + olc::vi2d{radius + 15, 0};
    vWheelCursor    = vCircleOffset;
    vBarCursor      = vVBarOffset + olc::vi2d{vVBarSize.x / 2, -radius};
    vTextRGBOffset  = vVBarOffset + olc::vi2d{vVBarSize.x + 30, -radius};
    vTextHSVOffset  = vTextRGBOffset + olc::vi2d{0, 100};
    vSelectedOffset = olc::vi2d{vTextHSVOffset.x, vFrameOffset.y + 2 * radius + vPadding.y - 50};
    vFrameSize      = olc::vi2d{vTextRGBOffset.x + 185, 2 * radius} + 2 * vPadding;
    
}

void ColorWheel::HSVtoRGB(float h, float s, float v)
{
    // Hue: [0.0, 360.0] |  Saturation: [0.0, 1.0] | Value (Brightness): [0.0, 1.0]
    auto f = [&](float n)
    {
        float k = fmod(n + h / 60.0, 6.0);
        return (v - v * s * fmax(0, fmin(k, fmin(4.0 - k, 1.0))));
    };

    R = (int)(255 * f(5));
    G = (int)(255 * f(3));
    B = (int)(255 * f(1));
}

void ColorWheel::RGBtoHSV(uint8_t r, uint8_t g, uint8_t b)
{
    // Red: [0, 255] | Blue: [0, 255] | Green: [0, 255]
    V = (float) std::max(r, std::max(g, b)) / (float) 255;
    float C = V - (float) std::min(r, std::min(g, b)) / (float) 255;
    
    if (C == 0.0) { H = 0.0; }
    else if (V == (float) r / (float) 255) { H = fmod(60.0 * (6.0 + (g - b) / (255 * C)), 360.0); }
    else if (V == (float) g / (float) 255) { H = fmod(60.0 * (2.0 + (b - r) / (255 * C)), 360.0); }
    else if (V == (float) b / (float) 255) { H = fmod(60.0 * (4.0 + (r - g) / (255 * C)), 360.0); }

    if (V == 0) { S = 0.0; }
    else { S = C / V; }
}

void ColorWheel::CalculateColorWheel()
{
    for (int x = -radius; x < radius; x += 1)
    {
        for (int y = -radius; y < radius; y += 1)
        {
            float r = sqrt(x * x + y * y) / (float) radius;
            float angle = atan2(y, x) * 180.0 / M_PI;
            HSVtoRGB(angle, r, V);
            uint8_t alpha = 255;
            if (r > 1.0) 
            { 
                R = 0;
                G = 0;
                B = 0;
                alpha = 0;
            }

            // Fix index in map
            int nMapWidth = 2 * radius;
            int transX = x + radius;
            int transY = y + radius;
            int pixelWidth = 4;
            int index = (transY * nMapWidth + transX) * pixelWidth;
            vPixelMap[index + 0] = R;
            vPixelMap[index + 1] = G;
            vPixelMap[index + 2] = B;
            vPixelMap[index + 3] = alpha;
        }
    }
}

void ColorWheel::DrawColorWheel(olc::PixelGameEngine& pge)
{
    olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
    pge.SetPixelMode(olc::Pixel::ALPHA);
    for (int x = -radius; x < radius; x += 1)
    {
        for (int y = -radius; y < radius; y += 1)
        {
            int nMapWidth = 2 * radius;
            int transX = x + radius;
            int transY = y + radius;
            int index = (transY * nMapWidth + transX) * pixelWidth;
            olc::Pixel pixel = {vPixelMap[index + 0], 
                                vPixelMap[index + 1],
                                vPixelMap[index + 2],
                                vPixelMap[index + 3]};
            pge.Draw({vCircleOffset.x + x, vCircleOffset.y + y}, pixel);
        }
    }
    pge.SetPixelMode(currentPixelMode);
}

void ColorWheel::DrawBrightnessBar(olc::PixelGameEngine& pge)
{
    for (int x = 0; x < vVBarSize.x; x += 1)
    {
        for (int y = -radius; y < radius; y += 1)
        {
            uint8_t brightness = (uint8_t)(255 * ((1 - (float) y / (float) radius) / 2.0));
            pge.Draw({vVBarOffset.x + x, vVBarOffset.y + y}, 
                     {brightness, brightness, brightness});
        }
    }
}

void ColorWheel::DrawColorChart(olc::PixelGameEngine& pge)
{
    // Draw circle
    DrawColorWheel(pge);
    pge.DrawCircle(vCircleOffset, radius+1, olc::WHITE);
    
    // Draw value bar
    DrawBrightnessBar(pge);
    olc::vi2d BarPos = vVBarOffset + olc::vi2d{0, -radius};
    pge.DrawRect(BarPos, vVBarSize, olc::WHITE);
}

void ColorWheel::DrawFrame(olc::PixelGameEngine& pge)
{
    olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
    pge.SetPixelMode(olc::Pixel::ALPHA);
    pge.FillRect(vFrameOffset, vFrameSize, olc::Pixel{ 30,  30,  30, 150});
    pge.DrawRect(vFrameOffset, vFrameSize, olc::Pixel{255, 255, 255, 255});
    pge.SetPixelMode(currentPixelMode);
}

void ColorWheel::HandleColorWheel(olc::PixelGameEngine& pge, olc::Pixel& pixel, bool& run)
{
    // Set initial value t
    if (enter) 
    { 
        R = pixel.r;
        G = pixel.g;
        B = pixel.b;
        enter = false;
    }
    
    olc::vi2d vMouse = {pge.GetMouseX(), pge.GetMouseY()};

    // Handle color selection
    olc::vi2d PosInWheel = vMouse - vCircleOffset;
    int ClickDist = sqrt(PosInWheel.x * PosInWheel.x + 
                         PosInWheel.y * PosInWheel.y);
    if (pge.GetMouse(0).bHeld && (ClickDist < (float) radius))
    {
        int transX = PosInWheel.x + radius;
        int transY = PosInWheel.y + radius;
        int index = (transY * (2 * radius) + transX) * pixelWidth;
        R = vPixelMap[index + 0];
        G = vPixelMap[index + 1];
        B = vPixelMap[index + 2];
        RGBtoHSV(R, G, B);
        vWheelCursor = vMouse;
    }

    // Handle brightness selection
    olc::vi2d PosInBar = vMouse - vVBarOffset - olc::vi2d{vVBarSize.x, 0};
    if (pge.GetMouse(0).bHeld && abs(PosInBar.x) <= vVBarSize.x && abs(PosInBar.y) <= radius)
    {
        V = (1 - (float) PosInBar.y / (float) radius) / 2.0;
        CalculateColorWheel();
        HSVtoRGB(H, S, V);
        vBarCursor.y = vMouse.y;
    }

    // Draw Frame
    DrawFrame(pge);

    // Draw color wheel and brightness bar
    DrawColorChart(pge);

    // Draw selected color cursor 
    pge.DrawCircle(vWheelCursor, 4, olc::WHITE);
    pge.DrawCircle(vWheelCursor, 5, olc::BLACK);

    // Draw selected brightness cursor
    pge.DrawCircle(vBarCursor, 4, olc::WHITE);
    pge.DrawCircle(vBarCursor, 5, olc::BLACK);

    // Draw current HSV
    pge.DrawString(vTextHSVOffset + olc::vi2d{0,  0}, "H:" + std::to_string(H), olc::WHITE, 2);
    pge.DrawString(vTextHSVOffset + olc::vi2d{0, 30}, "S:" + std::to_string(S), olc::WHITE, 2);
    pge.DrawString(vTextHSVOffset + olc::vi2d{0, 60}, "V:" + std::to_string(V), olc::WHITE, 2);

    // Draw current RGBA
    pge.DrawString(vTextRGBOffset + olc::vi2d{0,  0}, "R:" + std::to_string(R), olc::RED,   2);
    pge.DrawString(vTextRGBOffset + olc::vi2d{0, 30}, "G:" + std::to_string(G), olc::GREEN, 2);
    pge.DrawString(vTextRGBOffset + olc::vi2d{0, 60}, "B:" + std::to_string(B), olc::BLUE,  2);

    // Draw current selected color
    pge.FillRect(vSelectedOffset, {50, 50}, {R, G, B});
    pge.DrawRect(vSelectedOffset, {50, 50}, olc::WHITE);

    pixel = olc::Pixel{R, G, B};
    if (pge.GetKey(olc::Key::Z).bPressed)
    {
        run = false;
        enter = true;
    }
}
#ifndef CANVAS_H
#define CANVAS_H

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <SDL2/SDL.h>

using namespace std;

class Canvas 
{
    public:
        SDL_Event cEvent;

        Canvas();
        Canvas(const Canvas &);
        Canvas& operator=(const Canvas &);
        ~Canvas();

        void init(unsigned int scrWidth, unsigned int scrHeight,
                  string WindowName = "Dynamical Systems - ClickSpace",
                  unsigned short int r = 255,
                  unsigned short int g = 255,
                  unsigned short int b = 255);
        
        void show();

        void setColor(unsigned short int r,
                      unsigned short int g,
                      unsigned short int b,
                      unsigned short int alpha);
        
        void clear(unsigned short int r = 255,
                   unsigned short int g = 255,
                   unsigned short int b = 255);

        void CanvasToSystem();

        void SystemToCanvas();

        void SetNewScale();

        void drawPhasePoint();

        void drawZoomRect();

        void drawLine();

        void drawOrbit();

        int getWidth();
        
        int getHeight();

        // Orbit points
        vector<vector<double>> orb_pts;

        // Canvas coordinates
        int cX;
        int cY;

        // System coordinates 
        double sX;
        double sY;
        double sX_min;
        double sX_max;
        double sY_min;
        double sY_max;
        double sX_scl_min;
        double sX_scl_max;
        double sY_scl_min;
        double sY_scl_max;

        // Zoom rectangle coordinates
        int zoom_cX_a;
        int zoom_cY_a;
        int zoom_cX_b;
        int zoom_cY_b;

        // Initial conditions line
        int line_cX_a;
        int line_cY_a;
        int line_cX_b;
        int line_cY_b;
        
        // Flags
        bool quit = false;
        bool mouse_hold = false;
        bool zoom = false;
        bool zoom_quit = false;
        bool zoom_clear = false;
        bool line_hold = false;
        bool line_quit = false;
        bool line_run = false;
        
        int line_pts = 0;
        int max_line_pts = 50;

        // Drawing colors
        Uint8 lineR = 255;
        Uint8 lineG = 0;
        Uint8 lineB = 0;
        Uint8 lineA = 20;

        Uint8 zoomR = 0;
        Uint8 zoomG = 128;
        Uint8 zoomB = 255;
        Uint8 zoomA = 20;

        Uint8 mappR = 0;
        Uint8 mappG = 0;
        Uint8 mappB = 0;
        Uint8 mappA = 255;

    private:
        SDL_Window *cWindow;
        SDL_Renderer *cRenderer;
        SDL_Texture *cMapTexture;

        unsigned int cWidth;
        unsigned int cHeight;
};

#endif
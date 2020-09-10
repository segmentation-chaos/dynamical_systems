#include "canvas.hpp"

Canvas::Canvas() {}

Canvas::Canvas(const Canvas &other)
{
    *this = other;
}

Canvas& Canvas::operator=(const Canvas &other)
{
    if (this != &other)
    {
        cWidth = other.cWidth;
        cHeight = other.cHeight;
        cWindow = other.cWindow;
        cRenderer = other.cRenderer;
        cEvent = other.cEvent;
    }

    return *this;
}

Canvas::~Canvas()
{
    SDL_DestroyTexture(cMapTexture);
    SDL_DestroyRenderer(cRenderer);
    SDL_DestroyWindow(cWindow);
    cMapTexture = NULL;
    cRenderer = NULL;
    cWindow = NULL;

    SDL_Quit();
}

void Canvas::init(unsigned int scrWidth, unsigned int scrHeight, string WindowName, 
             unsigned short int r, unsigned short int g, unsigned short int b)
{
    if (0 > SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        // Window creation
        cWindow = SDL_CreateWindow(WindowName.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                                       SDL_WINDOWPOS_UNDEFINED,
                                                       scrWidth, scrHeight,
                                                       SDL_WINDOW_SHOWN |
                                                       SDL_WINDOW_RESIZABLE);
        if (NULL == cWindow)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        }

        // Renderer creation
        cRenderer = SDL_CreateRenderer(cWindow, -1, SDL_RENDERER_SOFTWARE);
        if (NULL == cRenderer)
        {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            SDL_SetRenderDrawColor(cRenderer, r, g, b, 255);
            cWidth = scrWidth;
            cHeight = scrHeight;
        }
        
        // Map Texture
        cMapTexture = SDL_CreateTexture(cRenderer, SDL_PIXELFORMAT_RGBA8888,
                                                   SDL_TEXTUREACCESS_TARGET,
                                                   cWidth, cHeight);
        SDL_SetRenderTarget(cRenderer, cMapTexture);
        SDL_SetRenderDrawColor(cRenderer, r, g, b, 255);
        SDL_RenderClear(cRenderer);
        SDL_SetRenderTarget(cRenderer, NULL);
    }
}

void Canvas::show()
{
    SDL_RenderPresent(cRenderer);
}

void Canvas::setColor(unsigned short int r, unsigned short int g,
                      unsigned short int b, unsigned short int alpha)
{
    SDL_SetRenderDrawColor(cRenderer, r, g, b, alpha);
}

void Canvas::clear(unsigned short int r, unsigned short int g, 
                   unsigned short int b) 
{
    SDL_SetRenderDrawColor(cRenderer, r, g, b, 255);
    SDL_RenderClear(cRenderer);
}

void Canvas::setViewport(int x, int y, int w, int h)
{
    SDL_Rect viewport = {x, y, w, h};
    SDL_RenderSetViewport(cRenderer, &viewport);
}

void Canvas::CanvasToSystem()
{
    sX = fabs(sX_scl_max - sX_scl_min) * ((double) cX / (double) cWidth) + sX_scl_min;
    sY = fabs(sY_scl_max - sY_scl_min) * ((double) (cHeight - cY) / (double) cHeight) + sY_scl_min;

}

void Canvas::SystemToCanvas() 
{
    cX = (int) cWidth * (sX - sX_scl_min) / fabs(sX_scl_max - sX_scl_min);
    cY = (int) cHeight * (1.0 - ((sY - sY_scl_min) / fabs(sY_scl_max - sY_scl_min)));
}

void Canvas::SetNewScale()
{
    double new_x_min = sX_scl_min;
    double new_x_max = sX_scl_max;
    double new_y_min = sY_scl_min;
    double new_y_max = sY_scl_max;

    // Update scale to zoomed window
    if (zoom_cX_b > zoom_cX_a)
    {
        new_x_min = fabs(sX_scl_max - sX_scl_min) * ((double) zoom_cX_a / (double) cWidth) + sX_scl_min;
        new_x_max = fabs(sX_scl_max - sX_scl_min) * ((double) zoom_cX_b / (double) cWidth) + sX_scl_min;
    }
    else
    {
        new_x_max = fabs(sX_scl_max - sX_scl_min) * ((double) zoom_cX_a / (double) cWidth) + sX_scl_min;
        new_x_min = fabs(sX_scl_max - sX_scl_min) * ((double) zoom_cX_b / (double) cWidth) + sX_scl_min;
    }

    if (zoom_cY_a < zoom_cY_b)
    {
        new_y_min = fabs(sY_scl_max - sY_scl_min) * ((double) (cHeight - zoom_cY_b) / (double) cHeight) + sY_scl_min; 
        new_y_max = fabs(sY_scl_max - sY_scl_min) * ((double) (cHeight - zoom_cY_a) / (double) cHeight) + sY_scl_min; 
    }
    else
    {
        new_y_max = fabs(sY_scl_max - sY_scl_min) * ((double) (cHeight - zoom_cY_b) / (double) cHeight) + sY_scl_min; 
        new_y_min = fabs(sY_scl_max - sY_scl_min) * ((double) (cHeight - zoom_cY_a) / (double) cHeight) + sY_scl_min; 
    }

    sX_scl_min = new_x_min;
    sX_scl_max = new_x_max;
    sY_scl_min = new_y_min;
    sY_scl_max = new_y_max;
}

void Canvas::drawPhasePoint()
{
    setColor(mappR, mappG, mappB, mappA);
    SDL_RenderDrawPoint(cRenderer, cX, cY);
}

void Canvas::drawZoomRect()
{
    SDL_Rect zoomRect = {zoom_cX_a, zoom_cY_a, (zoom_cX_b - zoom_cX_a), (zoom_cY_b - zoom_cY_a)};
    setColor(zoomR, zoomG, zoomB, zoomA);
    SDL_RenderDrawRect(cRenderer, &zoomRect);
}

void Canvas::drawZoomLine()
{
    setColor(lineR, lineG, lineB, lineA);
    SDL_RenderDrawLine(cRenderer, line_cX_a, line_cY_a, line_cX_b, line_cY_b);

    SDL_Rect ctrPt = {line_cX_a - 3, line_cY_a - 3, 6, 6};
    SDL_Rect tipPt = {line_cX_b - 3, line_cY_b - 3, 6, 6};
    SDL_RenderDrawRect(cRenderer, &ctrPt);
    SDL_RenderDrawRect(cRenderer, &tipPt);

    double versor[2];
    if (line_pts != 0)
    {
        versor[0] = (line_cX_b - line_cX_a) / (double) line_pts;
        versor[1] = (line_cY_b - line_cY_a) / (double) line_pts;

        for (int i = 1; i < line_pts; i++)
        {
            SDL_Rect ctrPt = {line_cX_a - 3 + (int) ((double) i * versor[0]),
                              line_cY_a - 3 + (int) ((double) i * versor[1]), 6, 6};
            SDL_RenderDrawRect(cRenderer, &ctrPt);
        }
    }
}

void Canvas::drawLine(int xo, int yo, int xf, int yf, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    setColor(r, g, b, a);
    SDL_RenderDrawLine(cRenderer, xo, yo, xf, yf);
}

void Canvas::drawPoint(int xo, int yo, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    setColor(r, g, b, a);
    SDL_RenderDrawPoint(cRenderer, xo, yo);
}

void Canvas::drawOrbit()
{
    for (unsigned int i = 0; i < orb_pts[0].size(); i++)
    {
        sX = orb_pts[0][i];
        sY = orb_pts[1][i];

        SystemToCanvas();
        drawPhasePoint();
    }
}

int Canvas::getWidth()
{
    return cWidth;
}

int Canvas::getHeight()
{
    return cHeight;
}

int Canvas::canvas_quit()
{
    SDL_DestroyRenderer(cRenderer);
    SDL_DestroyWindow(cWindow);
    cRenderer = NULL;
    cWindow = NULL;

    SDL_Quit();

    return 0;
}
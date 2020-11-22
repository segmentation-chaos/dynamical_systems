/*                                    */
/*            CLICK SPACE             */
/*                                    */
/* An educational application for dy- */
/* namical systems and C++ programmi- */
/* ng. Two applications available:    */
/*                                    */
/* 2D maps:                           */
/* Allows the interaction with a can- */
/* vas from which orbits of 2D maps   */
/* can be iterated.                   */
/* Controls:                          */
/* Left click (hold): iterate orbit   */
/* Right click: open zoom frame       */
/* (When zooming) Left click: zoom in */
/* C: total zoom out                  */
/* X: delete previous orbit           */
/* Ctrl + X: delete all orbits        */
/* Ctrl + S: Save current orbits      */
/*        (file in results/map_name/) */
/* R: Start line of init. cond. pts   */
/* (In line) Scrool up: increase pts  */
/* (In line) Scrool down: decrease pts*/
/* (In line) Space bar: run all       */
/* (In line) S: exit line             */
/* N: Toggle night | light mode color */
/*                                    */
/*                                    */
/* 1D maps:                           */
/* Allows the interaction with a can- */
/* vas from which cob web orbits are  */
/* draw, allowing also the control of */
/* one of the system's parameter      */
/* Controls:                          */
/* Left mouse click: run cob web      */
/* W: increase parameter              */
/* S: decrease parameter              */
/* ESC: back to main menu             */
/*                                    */
/*                                    */
/* This program is implemented with   */
/* the olcPixelGameEngine, as develo- */
/* ped by Javidx9 (David Bahr). Its   */
/* original license and readme file   */
/* are shown in LICENSE/, and the en- */
/* gine file in olcPixelGameEngine.h. */
/* Moreover, the code for the main    */
/* menu was directly copied from it,  */
/* along with small tips and tricks   */
/* that Javidx9 brilliantly passes on */
/* to all commom folks willing to le- */
/* arn C++ from his channel (Thank    */
/* you Javidx9), although any errors  */
/* or bugs are in full responsability */
/* of the developers.                 */
/*                                    */
/* Designed and manufactured in       */
/* Brazil by:                         */
/* M. Lazarotto                       */
/* V. Oliveira                        */
/* M. Palmero                         */
/* (21/11/2020)                       */
/*                                    */
/*                                    */
/* To run the program, within a term- */
/* inal compile it by typing:         */
/*                                    */
/* make clean && make                 */
/*                                    */
/* And run it by typing:              */
/*                                    */
/* vblank_mode=0 ./click_space        */
/*                                    */

#include "clickspace.h"

int main()
{
    // App window settings
    olcClickSpace application;
    int32_t ScreenWidth = 1200;
    int32_t ScreenHeight = 1000;
 
    // Main game loop  
    if (application.Construct(ScreenWidth, ScreenHeight, 1, 1))
    {
        application.Start();
    }

    return 0;
}
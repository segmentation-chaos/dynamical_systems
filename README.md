# dynamical_systems
Development of a comprehensive tool for analyzing dynamical systems by graduate students of the Oscillations Control Group (Institute of Physics of the University of São Paulo).

## Current maps
* Halley
* Hénon
* Simplified Fermi-Ulam model
* Standard
* Standard nontwist 

## Features
* Orbit 
* Phase space
* Interactive phase space (clickSpace)
* Winding number

## ClickSpace commands
* Left click (hold): run orbit for click initial point
* Right click (hold): open zoom window (zoom in when released)
* c: total zoom out
* r: creates initial condition line
* s: destroy initial condition line
* Scrool up|down: increases|decreases the number of points in initial condition line

## Install
To instal dyn_sys, just type `make` in `dynamical_systems/` folder and `./dyn_sys` to run it.

In order to install dyn_sys, one must have the c++11 compiler available; also, the SDL2 C library is used for graphics, hence, the same is required. In case you have ubuntu, you can get the package directly from repository:

`apt-get install libsdl2-dev `

The first library `libsdl2-dev` is the Simple DirectMedia Layer with development version, which allows us to load `...hpp` files from it.
The second library `libsdl2-image-dev` is the sub library for image loading, required for textures and external images.
The third library `libsdl2-ttf-dev` is the sub library for text loading.

Development of a comprehensive and interactive tool for analyzing dynamical systems (2D maps) made by students of the Oscillations Control Group (http://portal.if.usp.br/controle/), Institute of Physics of the University of São Paulo, Brazil. 

The program is made entirely in C/C++, open source code, using SDL2 graphical library (https://www.libsdl.org/). The current version runs in linux OS. It also requires basic knowledge of C/C++ to set the analysis and maps from the main code. Future versions will contain a user friendly graphical interface. 

Authors: M. Lazarotto; V. de Oliveira; M. Palmero (Last update 27/08/2020)

## Run
To run dyn_sys, just type `make` in `dynamical_systems/` folder to compile and `./dyn_sys` to run it. `make clean` will delete the executable.

## Install
In order to install dyn_sys, one must have the c++11 compiler available along with SDL2 C library, which is used for graphics. In case you have ubuntu, you can get the package directly from repository:

`apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev`

The first library `libsdl2-dev` is the Simple DirectMedia Layer with development version, which allows us to load `...hpp` files from it.
The second library `libsdl2-image-dev` is the sub library for image loading, required for textures and external images.
The third library `libsdl2-ttf-dev` is the sub library for text loading.

# Current maps
## 2D maps
* Halley
* Hénon
* Simplified Fermi-Ulam model
* Standard
* Standard nontwist 

## 1D maps
* Linear sine 
* Logistic
* Logistic (2nd order)
* Moran
* Triangle

## Features
* Orbit 
* Phase space
* Interactive phase space (clickSpace)
* Winding number

## ClickSpace commands
* Left click (hold): run orbit for click initial point
* Right click: open zoom window (zoom in with Left click)
* x: delete last orbit
* p: save orbit into file -> `results/map_name`
* c: total zoom out
* r: creates initial condition line
* s: destroy initial condition line
* e: run dynamics for initial condition line points
* Scrool up|down: increases|decreases the number of points in initial condition line

## CobWeb commands
* Left click (hold): run cob web iteration
* w: increase map parameter (par[0])
* s: decrease map parameter (par[0])

## Future developments
* Fully contained graphical interface
* Interactive cobweb plot for 1D maps
* Bifurcation diagram
* Data saving
* Billiards
* +Maps
* +Analysis tools

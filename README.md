Development of a comprehensive and interactive tool for analyzing dynamical systems (2D and 1D maps) made by students of the Oscillations Control Group (http://portal.if.usp.br/controle/), Institute of Physics of the University of São Paulo, Brazil. 

The code for `dynamical_systems` is made entirely in C/C++ and based on the olcPixelGameEngine graphical library, as made available by Javidx9 (for whom the authors are deeply grateful). The engine licences are kept in 'LICENSES' folder, along with its original README file 'https://github.com/OneLoneCoder/olcPixelGameEngine'. 

The current version runs in linux OS but it may be easily ported to Windows.

Authors: M. Lazarotto; V. de Oliveira; M. Palmero (Last update 20/11/2020)

## Run
To run dyn_sys, just type `make` in `dynamical_systems/` folder to compile it and `vblank_mode=0 ./dyn_sys` to run it. `make clean` will delete the executable. The `vblank_mode=0' is used in ubuntu to unlock the frame rate. Otherwise it may work but with fixed 60 fps.

## Install
In order to install dyn_sys, one must have the c++17 compiler. 

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
* 2D interactive phase-space
* 1D interactive plot (Cob Web)

## ClickSpace commands
* Left click (hold): run orbit for clicked initial point
* Right click: open zoom window 
** (When zooming: Left click to set zoom)
* C: total zoom out
* X: delete last orbit
* Ctrl + X: delete all orbits
* Ctrl + S: Save all orbits (file in -> `results/map_name`)
* R: creates initial condition line
* S: destroy initial condition line
* SPACE: run dynamics for initial condition line points
* Scrool UP|DOWN: increases|decreases the number of points in initial condition line
* N: toggle night | light mode on and off.

## CobWeb commands
* Left click (hold): run cob web iteration
* w: increase map parameter (par[0])
* s: decrease map parameter (par[0])

## Future developments
* Fully contained graphical interface
* Bifurcation diagram
* Billiards
* Fractals
* Celular automata
* +Maps
* +Analysis tools

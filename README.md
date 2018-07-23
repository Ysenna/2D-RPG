# 2D-RPG

# Building and running

## Linux
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

install tinyxml2 >= 6.0.0 
> sudo apt install libtinyxml2-dev

compile sainteos tmxparser
see https://github.com/sainteos/tmxparser
> git clone git@github.com:sainteos/tmxparser.git
> mkdir build
> cd build
> cmake ..
> make
> sudo make install

if 'tinyxml2Config.cmake' didn't get installed with libtinycml2 package,
open tmxparser/CMakeLists.txt and remove following line:
> find_package(tinyxml2 REQUIRED)
then repeat tmxparser installation procedure.

# voleng
## What is VolEng?
This is a work-in-progress "toy engine" focused on task parallelism and speed.
VolEng's name indicates **Vol**umetric **Eng**ine, intended to focus on volumetric data
such as three-dimensional "cube bodies" or two-dimensional orthographic tilemaps.

## Dependencies
- X11
- OpenGL
- PhysFS
- Threads
- GLFW3
- SoLoud
- LibPoco (Foundations & Util)

VolEng uses CMake for its building. At the moment, this project is set up to build under the GNU C++ compiler under Linux, however, only basic changes would be needed to make this cross platform. This would include dependency switching for X11 under Windows/MacOS.

GLFW3, SoLoud, and LibPoco would each need to be compiled and installed from source for CMake to recognize them as CMake modules.

## Resource Packages

VolEng supports archived resource packages through the usage of PhysFS. A "data" directory can be specified in the Application to load packages from a specific directory. These resource packages use an XML manifest file in the following format:

```
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
    <title>VolEng Core</title>
    <name>core</name>
    
    <resources>
        <res type="texture" alias="icon" path="icon.png"/>
        <res type="sfx" alias="boop" path="negative.wav"/>
        <res type="music" alias="night" path="song.mp3"/>
    </resources>
</manifest>
```

Resources can be asynchronously or synchronously loaded, with identifiers that can be hashed once and used forever. Identifiers take the form of ```type://pkgname/alias```, a specially parsed URI scheme that distinguishes type, source, and alias. An example identifier from the above XML manifest could be ```texture://core/icon```.

## Building
Assuming you have the required dependencies, building should be as simple as the following commands:

```
git clone https://github.com/walkerje/voleng/
cd voleng
mkdir build && cd build
cmake ..
make
```

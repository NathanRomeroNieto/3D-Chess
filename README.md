# 3D-Chess
3D chess game made in OpenGL

###Dependencies
- Freeglut3 (Opensource version of GLUT).
- GLEW (GL extensions wrangler library).
- Assimp model loader.
- SOIL image loader.
- A compiler with at least C++11 support.

###Compiling the program
- Using make file:
```
make
```
- Using compiler flags:
```
-lGL -lGLU -lglut -lGLEW -lassimp -lSOIL -w -std=c++11
```
- Example:
```
g++ main.cpp pieces.cpp textfile.cpp -lGL -lGLU -lglut -lGLEW -lassimp -lSOIL -w -std=c++11 -o Chess3D
```

###Linux install commands for each dependency
```
//Universe repo needed for SOIL and Assimp installations
sudo apt-add repository universe
sudo apt-get update

sudo apt-get install freeglut3-dev
sudo apt-get install libglew1.6-dev
sudo apt-get install libassimp-dev
sudo apt-get install libsoil-dev
```

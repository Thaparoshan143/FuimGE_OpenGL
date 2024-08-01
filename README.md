# Simple Graphics Renderer

#### Project made in Mac, will only work with Apple sillicon out of box, for other system change the build config in `build.sh` and attach appropriate libraries 

Entry point :

> main.cpp

change the application logic and properties as required.

### How to use

##
    git clone https://github.com/Thaparoshan143/FuimGR_OpenGL

> open project directory

## 
    git checkout release 
    bash build.sh
    bash run.sh

Build application exist inside `exe` folder

<hr>

## How to use 
![GR interface](misc/GRinterface.png?raw=true "Title")


https://github.com/user-attachments/assets/d8c7db2c-b227-4ee1-b9f7-57d133938d35


### Left
> <b>World</b> \
    - Color \
    - Grid \
    - Wireframe \
> <b>Camera</b> \
    - Position \
    - Mouse Sensitivity \
    - Movement Speed \
    - Zoom \
> <b>Light (Directional/Sun)</b> \
    - Direction \
    - Color 

### Right 
> <b>Object List</b>
> <b>Inspector</b> \
    - Transform \
    - Material Properties 

## Navigation
> <b>Movement</b> \
> W - Forward \
> S - Backward \
> A - Left \
> D - Right \
> Q - Up \
> E - Down 

> Mouse click and drag for view change

<hr>

### Used Library
- GLAD - Loader
- GLFW - Context 
- Assimp : 3D Model loading
- ImGui - GUI 

Find it under `includes` and `./src/external` folders

# Note : Copy at your own risk, written by dumb me

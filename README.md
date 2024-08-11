# TP SI & Anim ESIR2

This git repository contains all results for TP 1-3 of Anim and TP 1-3 of SI

These projects use OpenGL for rendering, and are built over the teacher's base code.

## SI

### Project 1

This first project was an introduction to OpenGL and its uses. We are required to draw a triangle on the screen using raw gl commands, and our own shader.

Files :
- src/Application/GL3_TP1.h
- data/Shaders/base.vert
- data/Shaders/red.frag

Controls : 
- ZQSD : Up/Left/Down/Right

![image](https://github.com/user-attachments/assets/38d6ce53-d0ce-40bf-bb98-fe5216040b1c)

### Project 2

More advanced than project 1, we must use the same functions to now display models (here a box) and do a basic lightning model, in our case Phong. All shaders are coded by hand. We do use a helper class to manager shaders and uniforms. We also make use of Vertex Array Objects.

Files :
- src/Application/GL3_TP2.h
- data/Shaders/phong.vert
- data/Shaders/phong.frag

Controls :
- ZQSD : Up/Left/Down/Right
- R/F : Forward/Backward
- Y/H : Pitch axis
- G/J : Yaw axis

![image](https://github.com/user-attachments/assets/0d12a9ff-30b0-416f-8f0b-b6613d990f9e)

### Project 3

Our final SI project was to create post processing effects such as depth of field and blur, cell shading and reflections. We also coded skyboxes, all using shaders.

Files :
- src/Application/GL3_TP3.h
- data/Shaders/PhongDirectional.vert
- data/Shaders/PhongDirectional.frag
- data/Shaders/Skybox.vert
- data/Shaders/Skybox.frag
- data/Shaders/PostProcessing/CellShading.frag
- data/Shaders/PostProcessing/DepthOfField.frag
- data/Shaders/PostProcessing/Fullscreen.vert

Controls :
- ZQSD : Forward/Left/Backward/Right
- R/F : Up/Down
- Y/H : Pitch axis
- G/J : Yaw axis

![image](https://github.com/user-attachments/assets/7188d6ce-ade3-4a8a-b21d-1710d7b93875)

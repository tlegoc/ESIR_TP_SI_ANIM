# TP SI & Anim ESIR2

This git repository contains all results for TP 1-3 of Anim and TP 1-3 of SI

These projects use OpenGL for rendering, and are built over the teacher's base code.

## Anim

### Project 1

The first anim project zas an introduction to the scene graph and quaternion/vector interpolation. We had to create a bee (sort of) and animate it only using parent/child hierachy.

Files :
 - src/Application/HermitInterp.h
 - src/Application/TrajectoryInterp.h
 - src/Application/Insect.h
 - src/Application/TP1_siaa.h

Controls :
  - ZQSD : Forward/Left/Back/Right
  - AE : Up/Down

![image](https://github.com/user-attachments/assets/e4dfd985-a51b-4f2c-8afa-010f4e84877a)

### Project 2

Inverse kinematic implementation on a variable size chain.

Files :
 - src/Animation/CCD.h
 - src/Application/TP2_siaa.h

Controls :
- ZQSD : Up/Left/Down/Right
- R/F : Forward/Backward
- Y/H : Pitch axis
- G/J : Yaw axis
- N : Randomize target position

![image](https://github.com/user-attachments/assets/21a9bb8a-8522-4f46-915a-18fbcd869f6e)

### Project 3

Implementation of a cloth using a spring mass system, and basic collision responses. Support 4 and 8 neighbours systems.

Files :
 - Animation/Physics.h (partially, forgot what I did)
 - Animation/SpringMassSystem.h (partially, forgot what I did)
 - src/Application/TP3_siaa.h

Controls :
- ZQSD : Up/Left/Down/Right
- R/F : Forward/Backward
- Y/H : Pitch axis
- G/J : Yaw axis
- C : Unlock one of the corners

![image](https://github.com/user-attachments/assets/d550d51c-7284-441d-b864-d70b78dce6f4)

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

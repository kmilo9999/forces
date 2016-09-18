Report Project 4
CS560  Animation
Camilo Diaz
c.diaz@digipen.edu

This project was compiled in Visual Studio 2013 using C++ in debug mode X86 - Win32.
Camera Controls:
Use the AWSD Keys to move the camera around.
Keep press the mouse's middle button and drag it in order to rotate the camera.

IJKL Keys control the position of the right anchor
Arrow Keys control the position in the Left anchor

By default the gravity is turned off. Use the button "Toogle Gravity" located in the GUI to toggle the gravity

All the libraries required for the project are in the folder "Libraries":
- Assimp : Free Library that loads 3D models and their animations.
- Anttweakbar : GUI Libray to debug the project.
- glew: Opengl Wrapper
- glfw: Create the window and handle the input
- stb-master: Loads image files.

IN ORDER TO RUN IT SUCCESSFULLY :
The following dll's must be in the Debug Folder:
- AntTweakBar.dll
- assimpd.dll
- glfw3.dll

I have implemented my own Math library (you can find it in the folder "Math"). This one uses Vectors of 2,3 and 4 dimensions,
Matrices and Quaternions. Additionally, I have implemented my own View and Projections matrices, quaternions operations
such as addition, subtraction, multiplication, conjugate and Rotational Matrix using Quaternions.
The Camera and the transformations applied to the models are VQS. The interpolation is made by SLERP. the algorithm is
located in the file Math.cpp line 542

The model displayed in the program is 27 mass points in a cube objectand two anchor points linked by springs vertically, 
horizontally and diagonally between adjacent points.You can see the initialization of the particle system in the classs 
Render.cpp line 357.

By default the gravity is turned off. Use the button "Toogle Gravity" located in the GUI to toggle the gravity. 
I did this because the spring stiffness is not strong enough to keep the cube shape.
I was able to modify the stiffness in a 2, 3 or 4 connected masses system. However, using 27 masses ,the forces apply in 
every direction and the stiffness must be calculated carefully.

Additionally, I'm drawing the springs (links) between masses for debug purpose and demonstrate that the masses are linked.

All the masses are connected by spring which produce a force equals to -KX. the value of K represent the stiffness of 
the spring (how much the spring must be move from its relax length to produce any force) and X implies the displacement
of the spring from its relax length.

I implemented a semi implicit euler with time steps of of 0.01. Lower that value is too slow and higher values makes the 
system too unstable. A damping value of 0.98 is multiplied by the velocity at time t in order to produce a solution more stable.
The implementation of this integrator is in the class EulerSemiImplicit.cpp







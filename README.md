# CSE167 Project 1 and 2

## Description

This is the first and the second project of [CSE167](http://ivl.calit2.net/wiki/index.php/CSE167F2020), an introductory computer graphic course taught in UCSD by Professor Schulze. It implements Mesh class to read obj files and store models using triangles. It also implements Phong shading and point light with iterative controls. 

## Build

The project is managed using Visual Studio on Win10. It depends on OpenGL, GLEW and GLM. With these dependencies configured correctly, this project should also be able to run on OS X and Linux. Instructions can be found [here](http://ivl.calit2.net/wiki/index.php/BasecodeCSE167F20).

4 obj files are loaded by this program. They are: [bunny](http://ivl.calit2.net/wiki/images/3/3f/BunnyF14.zip), [sandal](http://ivl.calit2.net/wiki/images/e/ec/SandalF20.zip), [bear](http://www.calit2.net/~jschulze/tmp/BearF14.zip) and [sphere](http://ivl.calit2.net/wiki/images/0/0a/CSE167SphereOBJ.zip). Put them in root directory and the program should load them correctly. You may also replace them by models you like, but codes need to be changed accordingly.

## Usage

- Press `n` to switch between normal shading and Phong shading.
- Press `F1`, `F2` and `F3` to switch between 3 models.
- Press `1` to enter mode 1, where you can use mouse to rotate the model, and use wheel to zoom in and out.
- Press `2` to enter mode 2, where you can  use mouse to rotate the point light, and use wheel to move point light closer or further.
- Press `3` to enter mode 3, where you can use mouse to rotate the model and the point light together, and use wheel to zoom in and out.

## Artworks!

Here are some artworks of this little fun project. They show how different surfaces can be simulated in Phong shading, and how moving light source affect lights and reflections.

Model 1 in Mode 2: Metallic surface.

![Model 1 in Mode 2](https://cdn.jsdelivr.net/gh/TonyZYT2000/ImageHost@master/Phong1.gif)

Model 2 in Mode 1: Matte surface.

![Model 2 in Mode 1](https://cdn.jsdelivr.net/gh/TonyZYT2000/ImageHost@master/Phong2.gif)

Model 3 in Mode 3: Jade surface.

![Model 3 in Mode 3](https://cdn.jsdelivr.net/gh/TonyZYT2000/ImageHost@master/Phong3.gif)
# Description

This is a final project required for approval in the Algorithms and Programing discipline at the Federal University of Rio Grande do Sul (UFRGS).

I started the project with a fork of Quickstart raylib to focus only in the ideia implemetation. You can find the same in https://github.com/raylib-extras/raylib-quickstart .

This project is a game inspired in "The Legend of Zelda" (1986), an adventure RPG game created by Nintendo.


# Build and Running the Project

## Linux Users
* CD into the build folder
* run `./premake5 gmake2`
* CD back to the root
* run `./build_run.sh`

# Output files
The built code will be in the bin dir

# Building for other OpenGL targets
If you need to build for a different OpenGL version than the default (OpenGL 3.3) you can specify an OpenGL version in your premake command line. Just modify the bat file or add the following to your command line

## For OpenGL 1.1
`--graphics=opengl11`

## For OpenGL 2.1
`--graphics=opengl21`

## For OpenGL 4.3
`--graphics=opengl43`

## For OpenGLES 2.0
`--graphics=opengles2`

## For OpenGLES 3.0
`--graphics=opengles3`

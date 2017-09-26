#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
 
//structure to hold opengl uints
typedef struct{
    unsigned int shader_program;
    unsigned int VAO;
    unsigned int VBO;
    GLFWwindow* window;
} Opengl_s;
 
typedef Opengl_s* Opengl;

//opengl render
void* init_opengl( void* arg );
void render_primatives( double primatives[], Opengl gl, int size );


#endif

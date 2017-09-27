#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow( 1000, 1000, "Deepnet", NULL, NULL );
    glfwMakeContextCurrent( window );
                                 
    glewExperimental = GL_TRUE;
    glewInit();

    return 0;
                                    
}

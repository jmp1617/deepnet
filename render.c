#define GLEW_STATIC

#include "deepnet.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

//reset viewport based on window size
void framebuffer_size_callback( GLFWwindow* window, int width, int height ) {
    glViewport( 0, 0, width, height );
}

int init_opengl( Opengl gl ){
    
    //init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    //create the window
    GLFWwindow* window = glfwCreateWindow( 800, 600, "OpenGL", NULL, NULL );
    if ( !window ) {
        printf("Failed to create glfw window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent( window );

    glViewport( 0, 0, 800, 800 );

    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    glewInit();

    //Init Shaders
    const char *vertexShaderSource = "#version 330 core\n"
         "layout (location = 0) in vec3 aPos;\n"
         "void main()\n"
         "{\n"
         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
         "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
         "out vec4 FragColor;\n"
         "void main()\n"
         "{\n"
         "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
         "}\n\0";

    //compile and link shaders into shader program
    unsigned int vertexShader;
    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );
#ifdef DEBUG
    int v_success;
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &v_success );
    printf("Vertex Shader Success: %d\n", v_success);
#endif

    unsigned int fragmentShader;
    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );
#ifdef DEBUG
    int f_success;
    glGetShaderiv(fragmentShader, GL_COMPILE, &f_success);
    printf("Fragment Shader Success: %d\n",f_success);
#endif

    //create the shader program
    gl->shader_program = glCreateProgram();
    glAttachShader( gl->shader_program, vertexShader );
    glAttachShader( gl->shader_program, fragmentShader );
    glLinkProgram( gl->shader_program );
#ifdef DEBUG
    int l_success;
    glGetProgramiv( gl->shader_program, GL_LINK_STATUS, &l_success );
    printf("Shader program link: %d\n", l_success );
#endif
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );
}

void render_primatives( double primatives[] ){

}

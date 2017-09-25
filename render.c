#define GLEW_STATIC

#include "deepnet.h"
#include <stdio.h>
#include <pthread.h>

//reset viewport based on window size
void framebuffer_size_callback( GLFWwindow* window, int width, int height ) {
    glViewport( 0, 0, width, height );
}

void* init_opengl( void* arg ){
    
    Opengl gl = (Opengl_s *) arg;
    //init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    //create the window
    gl->window = glfwCreateWindow( 1000, 1000, "Deepnet", NULL, NULL );
    
    glfwMakeContextCurrent( gl->window );

    glViewport( 0, 0, 1000, 1000 );

    glfwSetFramebufferSizeCallback( gl->window, framebuffer_size_callback );

    glewInit();

    //Init Shaders
    const char *vertexShaderSource = "#version 330 core\n"
         "layout (location = 0) in vec3 pos;\n"
         "layout (location = 1) in vec3 color;\n"
         "out vec3 out_color;\n"
         "void main()\n"
         "{\n"
         "   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
         "   out_color = color;\n"
         "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
         "out vec4 FragColor;\n"
         "in vec3 out_color;\n"
         "void main()\n"
         "{\n"
         "   FragColor = vec4(out_color, 1.0);\n"
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
    
    //set up objects
    glGenVertexArrays( 1, &gl->VAO );
    glGenBuffers( 1, &gl->VBO );
    return NULL;
}

void render_primatives( double primatives[], Opengl gl, int size ){
    //fill up the VBO
    glBindVertexArray( gl->VAO );
    glBindBuffer( GL_ARRAY_BUFFER, gl->VBO );
    glBufferData( GL_ARRAY_BUFFER, size, primatives, GL_DYNAMIC_DRAW );
    //link the attribs //TODO: try setting to normalized
    glVertexAttribPointer( 0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof( double ), (void*)0 );
    glEnableVertexAttribArray( 0 );
    //colors
    glVertexAttribPointer( 1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof( double ), (void*)( 3 * sizeof( double ) ) );
    glEnableVertexAttribArray( 1 );
    //unbind
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    //render the synapse weights
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glUseProgram( gl->shader_program );
    glBindVertexArray( gl->VAO );
    glDrawArrays( GL_POINTS, 0, size );

    //swap the buffers
    glfwSwapBuffers( gl->window );
    glfwPollEvents();

}

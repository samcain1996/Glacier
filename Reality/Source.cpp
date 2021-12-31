// Disable warnings about default library conflicts, 
// missing pdb for glew (because glew was compiled with
// an earlier vs version?), and allocating dynamically
// on the stack
#pragma warning( disable : 6255; disable : 4098 99)

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define LAUNCH_IN_FULLSCREEN 0
#define DEBUG_MODE 1

// File path to generic shaders
static const char* GENERIC_VERTEX_SHADER_PATH   = "Shaders/generic_vertex_shader.vert";
static const char* GENERIC_FRAGMENT_SHADER_PATH = "Shaders/generic_fragment_shader.frag";

/**
* @brief            Callback to print error messages that
*                   occur if DEBUG_MODE flag is set to 1
*
*/
void APIENTRY HandleErrors(unsigned int source, unsigned int type, 
    unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {
    std::cerr << message << std::endl;
}

/**
* @brief            Compile shader of type type with source code source
* 
* @param type       Type of shader to compile
* @param source     source code of shader
*/
static unsigned int CompileShader(unsigned int type, const std::string& source) 
{
    // Create shader of type type and get its id
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();  // Convert source code to c_string

    /*
    * Link shader id with the source code and the length
    * of bytes to read.
    * 
    * source code is double pointer because
    * c_strings are pointers to begin with
    */
    glShaderSource(id, 1, &src, nullptr); 

    glCompileShader(id);  // Compile shader

    // Check if shader compiled successfully
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        // Get the length of the error message
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        /*
        * Get the message as a stack allocated char pointer.
        * Stack allocations are faster than heap allocations 
        * but arrays cannot be initialized with variables on
        * the stack. Therefore, alloca must be used
        */
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        // Print error
        //std::cout << "Shader: " << id << " failed to compiled...\n" << message << std::endl;

        // Cleanup
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/**
* @brief                Compiles vertex and fragment shader
                        from source code
* 
* @param vertexShader   source code to vertex shader
* @param fragmentShader source code to fragment shader
*/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();  // Program to run on GPU
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);  // Compile vertex shader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);  // Compile fragnment shader

    // Attach shaders to program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // Link and compile program
    glLinkProgram(program);
    glValidateProgram(program);

    // Cleanup
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Make the window 720p unless LAUNCH_IN_FULLSCREEN flag is set
    int resX = 1280, resY = 720;

#if LAUNCH_IN_FULLSCREEN
    const GLFWvidmode* screen = glfwGetVideoMode(glfwGetPrimaryMonitor());
    resX = screen->width;
    resY = screen->height;
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(resX, resY, "Turquoise Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize glew*/
    if (glewInit() != GLEW_OK) { 
        std::cerr << "glewInit failed to complete :(";
        return -2;
    }

    // Print OpenGL version
    std::cout << glGetString(GL_VERSION) << std::endl;

#if DEBUG_MODE == 1
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(HandleErrors, nullptr);
#endif
    // 2D points representing a triangle
    float triangleVerts[8] {
        -0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f
    };

    //CheckErrors();

    // Index buffer defining which vertices to use for each triangle
    // Using an index buffer prevent storing duplicate vertices
    unsigned int indices[6]{
        1, 2, 3,
        0, 1, 3
    };

    // Create buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), triangleVerts, GL_STATIC_DRAW);
    //CheckErrors();

    glEnableVertexAttribArray(0);  // Enable drawing of vertex

    // Define data structure 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); 

    unsigned int indexBufferObj;
    glGenBuffers(1, &indexBufferObj);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    //CheckErrors();

    std::ostringstream ss;  // string stream to get the entire source code of each shader in one pass

    // Read in source code for generic vertex shader
    std::ifstream shaderStream(GENERIC_VERTEX_SHADER_PATH);
    ss << shaderStream.rdbuf();
    std::string vertexShader = ss.str();

    shaderStream.close();
    ss = std::ostringstream();

    // Read in source code for generic fragment shader
    shaderStream = std::ifstream(GENERIC_FRAGMENT_SHADER_PATH);
    ss << shaderStream.rdbuf();
    std::string fragmentShader = ss.str();

    // Create and use shader
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    //CheckErrors();

    glUseProgram(shader);
    //CheckErrors();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);  // Delete shader when done using it

    glfwTerminate();
    return 0;
}
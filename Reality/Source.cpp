// Disable warnings about default library conflicts, 
// missing pdb for glew (because glew was compiled with
// an earlier VS version?), and allocating dynamically
// on the stack
#pragma warning ( disable : 6255; disable : 4098 99 )

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Colors.h"
#include "Move.h"

// 0:   Launch in quarter resolution
// 1:   Launch fullscreen, native resolution
#define LAUNCH_IN_FULLSCREEN 0

// 0:   No error alerts/logging
// 1:   Enable error alerts/logging
#define DEBUG_MODE 1

// 0:   Show Every Debug Message
// 1:   Show Only Warning Messages and More Severe
#define DEBUG_MESSAGE_SEVERITY 0

// File path to generic shaders
const char* GENERIC_VERTEX_SHADER_PATH   = "Shaders/generic_vertex_shader.vert";
const char* GENERIC_FRAGMENT_SHADER_PATH = "Shaders/generic_fragment_shader.frag";

/**
* @brief            Callback to print error messages that
*                   occur if DEBUG_MODE flag is set to 1
*
*/
void APIENTRY HandleErrors(unsigned int source, unsigned int type,
    unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) 
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION && DEBUG_MESSAGE_SEVERITY) { return; }
    std::cerr << message << std::endl;
}

/**
* @brief                Returns the source for a generic vertex
*                       shader and a generic fragment shader
*                       ( probably will not remain in code
*                       for long )
* @param vertexShader   string to store vertex shader source code
* @param fragmentShader string to store fragment shader source code
*/
static void GetGenericShadersSource(std::string& vertexShader,
    std::string& fragmentShader) 
{
    // Ensure strings are empty
    vertexShader.clear(); fragmentShader.clear();

    std::ostringstream ss;  // string stream to get the entire source code of each shader in one pass

   // Read in source code for generic vertex shader
    std::ifstream shaderStream(GENERIC_VERTEX_SHADER_PATH);
    ss << shaderStream.rdbuf();
    vertexShader = ss.str();

    shaderStream.close();
    ss = std::ostringstream();

    // Read in source code for generic fragment shader
    shaderStream = std::ifstream(GENERIC_FRAGMENT_SHADER_PATH);
    ss << shaderStream.rdbuf();
    fragmentShader = ss.str();
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

    const GLFWvidmode* screen = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const float aspectRatio = screen->width / screen->height;
    // Make the window quarter resolution if LAUNC_IN_FULLSCREEN is false
    int resX = screen->width / 2, resY = screen->height / 2;
#if LAUNCH_IN_FULLSCREEN
    resX *= 2;
    resY *= 2;
#endif

    // Set Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(resX, resY, "Turquoise Triangle", NULL, NULL);
    if (!window)
    {
        std::cerr << "Damn, you did something to prevent the window from even being created. SMH 😔" << std::endl;
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

#if DEBUG_MODE
    glEnable(GL_DEBUG_OUTPUT);

#if DEBUG_MESSAGE_SEVERITY > 0
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
#endif

    glDebugMessageCallback(HandleErrors, nullptr);
#endif

    PositionVertex2D a(-0.5f,  0.5f);
    PositionVertex2D b(-0.5f, -0.5f);
    PositionVertex2D c( 0.5f,  0.5f);
    Triangle2D triangle(a, b, c);

    // 2D points representing triangles
    // PositionVertex2D triangleVerts[4]{
    //         PositionVertex2D(-0.5f,  0.5f),
    //         PositionVertex2D(-0.5f, -0.5f),
    //         PositionVertex2D( 0.5f, -0.5f),

    // };

    // Index buffer defining which vertices to use for each triangle
    // Using an index buffer prevent storing duplicate vertices
    unsigned int indices[3]{
        // 1, 2, 3,
        0, 1, 3
    };

    /*
    *
    *  FLOW: 1. Create & bind vertex array object to store the format for vertex buffer
    *        2. Create & bind vertex buffer to store data
    *        3. Create & bind index buffer to store location data
    *        4. Create shaders
    *        5. Define color
    * 
    */

    // Create vertex array object to define data format
    unsigned int vertexArrObj;
    glGenVertexArrays(1, &vertexArrObj);
    glBindVertexArray(vertexArrObj);

    // Create vertex buffer to define geometry
    unsigned int vertexBuffer;
    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), triangle.verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);  // Enable drawing of vertex

    // Define data structure 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); 

    // Create index buffer
    unsigned int indexBufferObj;
    glGenBuffers(1, &indexBufferObj);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // Get source code for vertex and fragment shaders
    std::string vertexShader, fragmentShader;
    GetGenericShadersSource(vertexShader, fragmentShader);

    // Create and use shader
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    
    // Define the color to draw in the fragment shader
    colors::Color color = colors::Black;
    int colorUniformLocation = glGetUniformLocation(shader, "u_Color");
   
    // Clear buffer bindings
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    int deg = 0;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        PositionVertex2D temp(0, 0);
        move::rotate(triangle, temp, ++deg);
        if (deg >= 360) { deg = 0; }

        // Bind buffers
        glUseProgram(shader);
        glUniform4fv(colorUniformLocation, 1, color);

        glBindVertexArray(vertexArrObj);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObj);

        // Change the color
        colors::RotateColor_s(color, Vec3f(0.001f, 0.0012f, 0.00016f));
        glUniform4fv(colorUniformLocation, 1, color);

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
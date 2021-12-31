#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
        std::cerr << "Shader: " << id << " failed to compiled...\n" << message << std::endl;

        // Cleanup
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/**
* @brief                    
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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    // 2D points representing a triangle
    float triangleVerts[6] {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    // Create buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), triangleVerts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);  // Enable drawing of vertex

    // Define data structure
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); 

    std::string vertexShader =
        "#version 330 core\n"  // Use OpenGL shader language version 330 and do not allow deprecated funcs
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n;";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.0, 1.0, 0.5, 1.0);\n"
        "}\n";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
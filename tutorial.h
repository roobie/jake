#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader
{
public:
  std::string ReadFile(const char* file)
  {
    // Open file
    std::ifstream t(file);

    // Read file into buffer
    std::stringstream buffer;
    buffer << t.rdbuf();

    // Make a std::string and fill it with the contents of buffer
    std::string fileContent = buffer.str();

    return fileContent;
  }

  void BindAttributeLocation(int index, const std::string &attribute)
  {
    // Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color
    // Attribute locations must be setup before calling glLinkProgram
    glBindAttribLocation(shaderProgram, index, attribute.c_str());
  }

  void UseProgram()
  {
    // Load the shader into the rendering pipeline
    glUseProgram(shaderProgram);
  }

  bool Init()
  {
    // Generate our shader. This is similar to glGenBuffers() and glGenVertexArray(), except that this returns the ID
    shaderProgram = glCreateProgram();

    // Bind the location of our attributes
    BindAttributeLocation(0, "in_Position");
    BindAttributeLocation(1, "in_Color");

    if (!LoadVertexShader("tutorial2.vert"))
      return false;

    if (!LoadFragmentShader("tutorial2.frag"))
      return false;

    // All shaders has been create, now we must put them together into one large object
    return LinkShaders();
  }


  bool LoadVertexShader(const std::string &filename)
  {
    std::cout << "Linking Vertex shader" << std::endl;

    // Read file as std::string
    std::string str = ReadFile(filename.c_str());

     // c_str() gives us a const char*, but we need a non-const one
    char* src = const_cast<char*>( str.c_str());
    int32_t size = str.length();

    // Create an empty vertex shader handle
    vertexshader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to OpenGL
    glShaderSource(vertexshader, 1, &src, &size);

    // Compile the vertex shader
    glCompileShader(vertexshader);

    int wasCompiled = 0;
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &wasCompiled );

    if (wasCompiled == 0)
    {
      PrintShaderCompilationErrorInfo(vertexshader);
      return false;
    }

    glAttachShader(shaderProgram, vertexshader);
    return true;
  }

  bool LoadFragmentShader(const std::string &filename)
  {
    std::cout << "Loading Fragment Shader" << std::endl;

    // Read file as std::string
    std::string str = ReadFile(filename.c_str());

     // c_str() gives us a const char*, but we need a non-const one
    char* src = const_cast<char*>( str.c_str());
    int32_t size = str.length();

    // Create an empty vertex shader handle
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the vertex shader source code to OpenGL
    glShaderSource(fragmentShader, 1, &src, &size);

    // Compile the vertex shader
    glCompileShader(fragmentShader);

    int wasCompiled = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &wasCompiled );

    if (wasCompiled == false)
    {
      PrintShaderCompilationErrorInfo(fragmentShader);
      return false;
    }

    glAttachShader(shaderProgram, fragmentShader);
    return true;
  }

  bool LinkShaders()
  {
    // Link. At this point, our shaders will be inspected/optized and the binary code generated
    // The binary code will then be uploaded to the GPU
    glLinkProgram(shaderProgram);

    // Verify that the linking succeeded
    int isLinked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);

    if (isLinked == false)
      PrintShaderLinkingError(shaderProgram);

    return isLinked != 0;
  }

  void PrintShaderLinkingError(int32_t shaderId)
  {
    std::cout << "=======================================\n";
    std::cout << "Shader linking failed : " << std::endl;

    // Find length of shader info log
    int maxLength;
    glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

    std::cout << "Info Length : " << maxLength << std::endl;

    // Get shader info log
    char* shaderProgramInfoLog = new char[maxLength];
    glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

    std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

    /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
    /* In this simple program, we'll just leave */
    delete[] shaderProgramInfoLog;
    return;
  }

  // If something went wrong whil compiling the shaders, we'll use this function to find the error
  void PrintShaderCompilationErrorInfo(int32_t shaderId)
  {
    std::cout << "=======================================\n";
    std::cout << "Shader compilation failed : " << std::endl;

    // Find length of shader info log
    int maxLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

    // Get shader info log
    char* shaderInfoLog = new char[maxLength];
    glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog );

    // Print shader info log
    std::cout << "\tError info : " << shaderInfoLog << std::endl;

    std::cout << "=======================================\n\n";
    delete[] shaderInfoLog;
  }

  void CleanUp()
  {
    /* Cleanup all the things we bound and allocated */
    glUseProgram(0);
    glDetachShader(shaderProgram, vertexshader);
    glDetachShader(shaderProgram, fragmentShader);

    glDeleteProgram(shaderProgram);


    glDeleteShader(vertexshader);
    glDeleteShader(fragmentShader);
  }

  // The handle to our shader program
  GLuint shaderProgram;

  // The handles to the induvidual shader
  GLuint vertexshader, fragmentShader;

};


// Our wrapper to simplify the shader code
Shader shader;

// Our object has 4 points
const uint32_t points = 4;

// Each poin has three values ( x, y, z)
const uint32_t floatsPerPoint = 3;

// Each color has 4 values ( red, green, blue, alpha )
const uint32_t floatsPerColor = 4;

// This is the object we'll draw ( a simple square
const GLfloat diamond[points][floatsPerPoint] = {
  { -0.5,  0.5,  0.5 }, // Top left
  {  0.5,  0.5,  0.5 }, // Top right
  {  0.5, -0.5,  0.5 }, // Bottom right
  { -0.5, -0.5,  0.5 }, // Bottom left
};

// This is the object we'll draw ( a simple square
const GLfloat colors[points][floatsPerColor] = {
  { 0.0, 1.0, 0.0, 1.0 }, // Top left
  { 1.0, 1.0, 0.0, 1.0  }, // Top right
  { 1.0, 0.0, 0.0, 1.0  }, // Bottom right
  { 0.0, 0.0, 1.0, 1.0  }, // Bottom left
};

// Create variables for storing the ID of our VAO and VBO
GLuint vbo[2];
GLuint vao[1];

// The positons of the position and color data within the VAO
const uint32_t positionAttributeIndex = 0, colorAttributeIndex = 1;
bool SetupBufferObjects()
{
  // Generate and assign two Vertex Buffer Objects to our handle
  glGenBuffers(2, vbo);

  std::cout << "=== [ HERE ] ===" << std::endl;
  // Generate and assign a Vertex Array Object to our handle
  glGenVertexArrays(1, vao);
  std::cout << "=== [ HERE ] ===" << std::endl;

  // Bind our Vertex Array Object as the current used object
  glBindVertexArray(vao[0]);

  // Positions
  // ===================
  // Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  // Copy the vertex data from diamond to our buffer
  glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerPoint) * sizeof(GLfloat), diamond, GL_STATIC_DRAW);

  // Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
  glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // Enable our attribute within the current VAO
  glEnableVertexAttribArray(positionAttributeIndex);

  // Colors
  // =======================
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

  // Copy the vertex data from diamond to our buffer
  glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerColor) * sizeof(GLfloat), colors, GL_STATIC_DRAW);

  // Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
  glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, 0, 0);

  // Note : We didn't enable the colors here!

  // Set up shader ( will be covered in the next part )
  // ===================
  if (!shader.Init())
    return false;

  shader.UseProgram();

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return true;
}

void Render(SDL_Window *mainWindow)
{
  // First, render a square without any colors ( all vertexes will be black )
  // ===================
  // Make our background grey
  //glClearColor(0.5, 0.5, 0.5, 1.0);
  //glClear(GL_COLOR_BUFFER_BIT);

  // Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes
  glDrawArrays(GL_LINE_LOOP, 0, 4);

  // Swap our buffers to make our changes visible
  //SDL_GL_SwapWindow(mainWindow);


  // Second, enable the colors and draw a solid square
  // ===================
  // Enable our attribute within the current VAO
  glEnableVertexAttribArray(colorAttributeIndex);

  // Make our background black
  //glClearColor(0.0, 0.0, 0.0, 0.0);
  //glClear(GL_COLOR_BUFFER_BIT);

  // Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // Swap our buffers to make our changes visible
  //SDL_GL_SwapWindow(mainWindow);

}

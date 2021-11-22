#ifndef __EMSCRIPTEN__
  #error Use Emscripten for building for web, if you are, something is mad sus
#endif

#include "core/core.h"
#include "core/thread/thread.h"
#include <emscripten/emscripten.h>
#include "core/gll/gll.h"
#include "core/window/window.h"

extern struct JIN_Window *root;
extern unsigned int shaderProgram;
extern unsigned int VAO;
extern float r;
extern float g;
extern float b;

const char *vertexShaderSourceWeb = "#version 300 es\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSourceWeb = "#version 300 es\n"
    "precision highp float;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

#include <stdio.h>
void web_loop(void)
{
  JIN_window_gl_set(root);
  JIN_gll();

/* Learn OpenGL #1 triangle, THIS IS NOT MY CODE, JUST FOR TEMPORARY TESTING (checkout src, good stuff in there) */
// build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceWeb, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceWeb, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Shader linking failed: %s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.0f,  0.5f, 0.0f,  // top
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
    };
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

  r = 0.2f, g = 0.3f, b = 0.3f;

  JIN_tick();
  JIN_dialog("Starting game loop");
  JIN_tick();
  JIN_dialog("Really long string. Let's try to get some overflow and see what happens...");

  
  emscripten_set_main_loop(JIN_tick, 60, EM_TRUE);

  JIN_window_gl_unset(root);
}

int main(int argc, char *args[])
{
  EM_ASM(alert('Confirm to begin'));
  JIN_init();

  web_loop();

  JIN_input_loop();

  JIN_quit();

  return 0;
}

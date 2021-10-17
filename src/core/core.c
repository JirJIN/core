#include "core.h"
#include "gll/gll.h"
#include "time.h"
#include "thread/thread.h"

#include "window/window.h"
#include "env/env.h"

struct JIN_Window *root; /* Root window */
struct JIN_Env     env; /* Environment variables */

struct JIN_Input JIN_inputv = {0};
struct JIN_Input JIN_input  = {0};

/* CORE FUNCTIONS */

/*
 * JIN_init
 *
 * @desc
 *   Initialize JIN
 * @return
 *    0 on success
 *   !0 on failure
 */
int JIN_init(void)
{
  if (JIN_logger_init(JIN_LOGGER_CONSOLE, JIN_LOGGER_ERR)) return -1;
  if (JIN_env_init(&JIN_env)) ERR_EXIT(-1, "Could not initialize the environment");
  if (!(root = JIN_window_create())) ERR_EXIT(-1, "Could not create the root window");

  return 0;
}

/*
 * JIN_quit
 *
 * @desc
 *   Quit JIN
 * @return
 *   0 on success
 */
int JIN_quit(void)
{
  JIN_window_destroy(root);
  JIN_env_quit(&JIN_env);
  JIN_logger_quit();

  return 0;
}

#define FPS         30
#define FRAME_DELAY (1000 / FPS)
int JIN_tick(void)
{
  clock_t frame_start, frame_end;
  double  frame_time;

  frame_start = clock();

  JIN_input = JIN_inputv;
  JIN_update();
  JIN_draw();

  frame_end = clock();
  frame_time = (frame_end - frame_start) / CLOCKS_PER_SEC / 1000;

  if (FRAME_DELAY > frame_time) {
    JIN_sleep(FRAME_DELAY - frame_time);
  }

  return 0;
}

float r, g, b;

/*
 * JIN_update
 * 
 * @desc
 *   Update stuff
 * @return
 *   0 on success
 */
int JIN_update(void)
{
  if (JIN_input.keys.f1) {
    r = 0.2f, g = 0.3f, b = 0.3f;
  }
  if (JIN_input.keys.f2) {
    r = 0.2f, g = 0.6f, b = 0.8f;
  }
  if (JIN_input.keys.f3) {
    JIN_window_size_set(root, 480, 320);
  }
  if (JIN_input.keys.f4) {
    JIN_window_size_set(root, 960, 640);
  }

  return 0;
}

/*
 * JIN_draw
 *
 * @desc
 *   Draw stuff
 * @return
 *   0 on success
 */
unsigned int VAO;
unsigned int shaderProgram;
int JIN_draw(void)
{
  glClearColor(r, g, b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  int w, h;
  JIN_window_size_get(root, &w, &h);
  glViewport(0, 0, w, h);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);

        JIN_window_buffer_swap(root);

  JIN_window_buffer_swap(root);

  return 0;
}

#include <stdio.h>
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
/* This shouldn't be needed but is for some reason, GLAPIENTRY is not defined */
#ifdef _WIN32
#define GLAPIENTRY __stdcall
#endif
void GLAPIENTRY gl_err_callback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *usr_param)
{
  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
    LOG(ERR, "GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n", type, severity, msg);
}
JIN_THREAD_FN JIN_game_thread(void *data)
{
  JIN_window_gl_set(root);
  JIN_gll();

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_err_callback, 0);

/* Learn OpenGL #1 triangle, THIS IS NOT MY CODE, JUST FOR TEMPORARY TESTING (checkout src, good stuff in there) */
// build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
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
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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

  while (1) {
    if (JIN_input.quit) break;
    JIN_tick();
  }

  JIN_window_gl_unset(root);

  return 0;
}

int JIN_dialog(const char* msg)
{
  JIN_window_dialog(root, msg);
  return 0;
}

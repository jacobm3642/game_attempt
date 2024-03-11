#include <X11/Xlib.h>
#include <GL/glew.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./include/utils.h"

char *read_shader(char *file) {
  stringStream *ss = initStringStream();
  readInFile(ss, file);
  char *shader = ss->string;
  free(ss);
  return shader;
}

bool running = true;

GLuint createShader(GLenum shaderType, const char* source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* errorLog = (char*)malloc(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);
        fprintf(stderr, "Shader compilation error: %s\n", errorLog);
        free(errorLog);

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        char* errorLog = (char*)malloc(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, errorLog);
        fprintf(stderr, "Program linking error: %s\n", errorLog);
        free(errorLog);

        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void drawTriangle() {
    char *vertexShaderSource = read_shader("tri.vert");
    char *fragmentShaderSource = read_shader("tri.frag");
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    if (vertexShader == 0 || fragmentShader == 0) {
        // Error creating shaders
        return;
    }

    GLuint shaderProgram = createProgram(vertexShader, fragmentShader);

    if (shaderProgram == 0) {
        // Error creating program
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void rendering_pass(Display *display, XEvent *event, GLXContext glc, Window window) {
  glXMakeCurrent(display, window, glc);
  glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  drawTriangle();
  glXSwapBuffers(display, window);
}

void handleEvent(Display *display, XEvent *event, GLXContext glc, Window window) {
  switch (event->type) {
  case Expose:
    rendering_pass(display, event, glc,  window);
    break;
  case ConfigureNotify:
    glXMakeCurrent(display, window, glc);
    glViewport(0, 0, event->xconfigure.width, event->xconfigure.height);
    break;
  case KeyPress:
    char buffer[1];
    int code;
    code = XLookupString(&event->xkey, buffer, 1, NULL, NULL);
    if (code == 1 && buffer[0] == 'q') {
      running = false;
    }
    break;
  }
}

int main() {
    Display *display;
    Window window;
    int screen;
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi;
    Colormap cmap;
    XSetWindowAttributes swa;
    GLXContext glc;

    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    // Get the screen
    screen = DefaultScreen(display);

    // Get a suitable visual
    vi = glXChooseVisual(display, screen, att);
    if (vi == NULL) {
        fprintf(stderr, "No suitable visual found\n");
        exit(1);
    }
    cmap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);

    // Set window attributes
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;

    // Create a window
    window = XCreateWindow(display, RootWindow(display, screen), 10, 10, 1080, 720, 0,
                           vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

    // Set the window title
    XSetStandardProperties(display, window, "Test Window", "Test Window", None, NULL, 0, NULL);

    // Map the window to make it visible
    XMapWindow(display, window);

    // Create an OpenGL context
    glc = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, window, glc);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
        exit(1);
    }

    XEvent event;
    while (running) {
      XNextEvent(display, &event);
      handleEvent(display, &event, glc, window);
    }
    // Clean up
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, glc);
    XFreeColormap(display, cmap);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}

#include <X11/Xlib.h>
#include <GL/glew.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./include/utils.h"
#include "./include/window.h"

bool running = true;

char *read_shader(char *file) {
  stringStream *ss = initStringStream();
  readInFile(ss, file);
  char *shader = ss->string;
  free(ss);
  return shader;
}

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
    char *vertexShaderSource = read_shader("shaders/tri.vert");
    char *fragmentShaderSource = read_shader("shaders/tri.frag");
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

    free(vertexShaderSource);
    free(fragmentShaderSource);
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
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // rendering goes right here VVVV
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
    WindowData *data = startWindow();
    
    XEvent event;
    while (running) {
      XNextEvent(data->display, &event);
      handleEvent(data->display, &event, data->glc, data->window);
    }

    cleanup(data);

    return 0;
}

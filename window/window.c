#include <X11/Xlib.h>
#include <GL/glew.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>

#include "window.h"

void initAttributes(WindowData *data) {
    data->att = (GLint[]){GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
}

void openDisplay(WindowData *data) {
    data->display = XOpenDisplay(NULL);
    if (data->display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
}

void getScreen(WindowData *data) {
    data->screen = DefaultScreen(data->display);
}

void chooseVisual(WindowData *data) {
    data->vi = glXChooseVisual(data->display, data->screen, data->att);
    if (data->vi == NULL) {
        fprintf(stderr, "No suitable visual found\n");
        exit(1);
    }
}

void createColormap(WindowData *data) {
    data->cmap = XCreateColormap(data->display, RootWindow(data->display, data->vi->screen),
                                 data->vi->visual, AllocNone);
}

void setWindowAttributes(WindowData *data) {
    data->swa.colormap = data->cmap;
    data->swa.event_mask = ExposureMask | KeyPressMask;
}

void createWindow(WindowData *data) {
    data->window = XCreateWindow(data->display, RootWindow(data->display, data->screen),
                                 10, 10, 1080, 720, 0, data->vi->depth, InputOutput,
                                 data->vi->visual, CWColormap | CWEventMask, &data->swa);
}

void setWindowTitle(WindowData *data) {
    XSetStandardProperties(data->display, data->window, "Test Window", "Test Window",
                           None, NULL, 0, NULL);
}

void mapWindow(WindowData *data) {
    XMapWindow(data->display, data->window);
}

void createGLContext(WindowData *data) {
    data->glc = glXCreateContext(data->display, data->vi, NULL, GL_TRUE);
    glXMakeCurrent(data->display, data->window, data->glc);
}

void initGLEW() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
        exit(1);
    }
}

void cleanup(WindowData *data) {
    glXMakeCurrent(data->display, None, NULL);
    glXDestroyContext(data->display, data->glc);
    XFreeColormap(data->display, data->cmap);
    XDestroyWindow(data->display, data->window);
    XCloseDisplay(data->display);
    free(data);
} 

WindowData *startWindow() {
    WindowData *data;
    data = malloc(sizeof(WindowData));

    initAttributes(data);
    openDisplay(data);
    getScreen(data);
    chooseVisual(data);
    createColormap(data);
    setWindowAttributes(data);
    createWindow(data);
    setWindowTitle(data);
    mapWindow(data);
    createGLContext(data);
    initGLEW();

    return data;
}

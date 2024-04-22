#ifndef WINDOW_H
#define WINDOW_H

typedef struct {
    Display *display;
    Window window;
    int screen;
    GLint *att;
    XVisualInfo *vi;
    Colormap cmap;
    XSetWindowAttributes swa;
    GLXContext glc;
} WindowData;

WindowData *startWindow();
void cleanup(WindowData *data);

#endif

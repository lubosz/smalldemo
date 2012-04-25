#include <GL/glx.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>

typedef enum { false, true } bool;

  bool quit;
    bool fullscreen;
    bool grab;
    char* programTile;
    unsigned width, height;


  int visualID;
  int default_screen;

  Display *display;

  xcb_screen_t *screen;
  xcb_connection_t *connection;
  xcb_window_t window;
  xcb_colormap_t colormap;
  xcb_cursor_t cursor; /* blank cursor */

  GLXWindow glxwindow;
  GLXContext context;
  GLXDrawable drawable;
  GLXFBConfig fb_config;

  // Input

  uint32_t eventmask;

int main(int argc, char** argv) {
  return 0;
}



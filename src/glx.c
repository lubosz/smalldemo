#include <GL/glx.h>
#include "gl3.h"
#include <stdio.h>

enum bool {false, true};

int main ()
{
    Display *display = XOpenDisplay(0);

    static int visual_attribs[] =
    {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, true,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        None
     };

    int fbcount;
    GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
    XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[0]);


    XSetWindowAttributes swa;
    swa.colormap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    Window win = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, 100, 100, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);

    XMapWindow(display, win);

    static int attribs[] =
    {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        None
    };

      const GLubyte *procname = (const GLubyte*) "glXCreateContextAttribsARB";

      PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs =
                  (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress(procname);
      if (!glXCreateContextAttribs) {
        printf("GL 3.x is not supported");
      }

        GLXContext ctx = glXCreateContextAttribs(display, fbc[0], NULL, True,attribs);

    glXMakeCurrent(display, win, ctx);

        glClearColor (1, 0.5, 0, 1);
        glClear (GL_COLOR_BUFFER_BIT);
        glXSwapBuffers (display, win);
        sleep(1);

    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, ctx);
    
    return 0;
}

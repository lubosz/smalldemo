// Needed for GL3 Context Creation
#define GLX_GLXEXT_PROTOTYPES 1
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

void createBlankCursor() {
  cursor = xcb_generate_id(connection);
  xcb_pixmap_t pix = xcb_generate_id(connection);

  xcb_create_pixmap(connection, 1, pix, screen->root, 1, 1);
  xcb_create_cursor(connection, cursor, pix, pix, 0, 0, 0, 0, 0, 0, 1, 1);
}

void initScreen() {
    /* Open Xlib Display */
    display = XOpenDisplay(0);
    if (!display) printf("Can't open display");

    default_screen = ((_XPrivDisplay)display)->default_screen;
    /* Get the XCB connection from the display */
    connection = XGetXCBConnection(display);
    if (!connection) printf("Can't get xcb connection from display");

    /* Acquire event queue ownership */
    XSetEventQueueOwner(display, XCBOwnsEventQueue);

    /* Find XCB screen */
    screen = 0;
    xcb_screen_iterator_t screen_iter =
            xcb_setup_roots_iterator(xcb_get_setup(connection));
    for (int screen_num = default_screen;
        screen_iter.rem && screen_num > 0;
        --screen_num, xcb_screen_next(&screen_iter));
    screen = screen_iter.data;

/*
  width = screen->width_in_pixels;
  height = screen->height_in_pixels;
*/
  width = 250;
  height = 260;

}

void initFrameBuffer() {
    /* Query framebuffer configurations */
    GLXFBConfig *fb_configs = 0;
    int num_fb_configs = 0;
    fb_configs = glXGetFBConfigs(display, default_screen, &num_fb_configs);
    if (!fb_configs || num_fb_configs == 0)
     printf("glXGetFBConfigs failed");
    fb_config = fb_configs[0];
}

void createGLContext() {
  /* Initialize window and OpenGL context */
  visualID = 0;

  /* Select first framebuffer config and query visualID */
  glXGetFBConfigAttrib(display, fb_config, GLX_VISUAL_ID, &visualID);

  //vector<int> glContext = Config::Instance().values<int>("GLcontext");

  int attribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB,
      3,
      GLX_CONTEXT_MINOR_VERSION_ARB,
      0,
      0
  };

//  /* Get a pointer to the context creation function for GL 3.0 */

//  const GLubyte *procname =
//          reinterpret_cast<const GLubyte*>("glXCreateContextAttribsARB");

//  PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs =
//              reinterpret_cast<PFNGLXCREATECONTEXTATTRIBSARBPROC>
//              (glXGetProcAddress(procname));
//  if (!glXCreateContextAttribs) {
//    printf("GL 3.x is not supported");
//  }

  XVisualInfo * foo = glXGetVisualFromFBConfig( display, fb_config );

//  context = glXCreateContextAttribs(display, fb_config, NULL, True,
//      attribs);

  context = glXCreateContext(display, foo, NULL, True);

//  extern GLXContext glXCreateContext( Display *dpy, XVisualInfo *vis,
//                      GLXContext shareList, Bool direct );

//  extern GLXContext glXCreateContextAttribsARB (Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);

  if (!context) printf("glXCreateNewContext failed");
}

void createColorMap() {
  /* Create XID's for colormap and window */
  colormap = xcb_generate_id(connection);
  window = xcb_generate_id(connection);

  /* Create colormap */
  xcb_create_colormap(
      connection,
      XCB_COLORMAP_ALLOC_NONE,
      colormap,
      screen->root,
      visualID);
}

void setWindowTitle(char* title) {


    xcb_change_property(
      connection,
      XCB_PROP_MODE_REPLACE,
      window,
    XCB_ATOM_WM_NAME,
    XCB_ATOM_STRING,
    8,
    //sizeof(title)/sizeof(char),
    //title
    3,
    "Foo"
    );
}

void createWindow() {
  eventmask =
      XCB_EVENT_MASK_KEY_PRESS |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_KEY_RELEASE;
  uint32_t valuelist[] = { eventmask, colormap };
  uint32_t valuemask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

  xcb_create_window(
      connection,
      XCB_COPY_FROM_PARENT,
      window,
      screen->root,
      0, 0,
      width, height,
      0,
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      visualID,
      valuemask,
      valuelist);

  // NOTE: window must be mapped before glXMakeContextCurrent
  xcb_map_window(connection, window);

  // Create GLX Window
  glxwindow = glXCreateWindow(display, fb_config, window, 0);

  if (!glxwindow)
    printf("glXCreateWindow failed");

  drawable = glxwindow;

  // make OpenGL context current
  if (!glXMakeContextCurrent(display, drawable, drawable, context))
    printf("glXMakeContextCurrent failed");

  // Set swap interval
#ifdef NVIDIA_GL
  PFNGLXSWAPINTERVALSGIPROC
    glXSwapInterval =
          reinterpret_cast<PFNGLXSWAPINTERVALSGIPROC> (glXGetProcAddress(
                  reinterpret_cast<GLubyte const *>("glXSwapIntervalSGI")));
  if (!glXSwapInterval) {
    printf("VSync is not supported");
  } else {
    glXSwapInterval(1);
  }
#endif

  setWindowTitle(programTile);
  
}

void swapBuffers() {
  glXSwapBuffers(display, drawable);
}

xcb_intern_atom_cookie_t getCookieForAtom(char* state_name) {
  return xcb_intern_atom(
    connection, 0, sizeof(state_name)/sizeof(char), state_name);
}

xcb_atom_t getReplyAtomFromCookie(xcb_intern_atom_cookie_t cookie) {
  xcb_generic_error_t * error;
  xcb_intern_atom_reply_t *reply =
          xcb_intern_atom_reply(connection, cookie, &error);
  if (error) {
    printf("Can't set the screen. Error Code: %s", error->error_code);
  }
  return reply->atom;
}

void toggleFullScreen() {
  fullscreen = !fullscreen;

  xcb_intern_atom_cookie_t wm_state_ck =
          getCookieForAtom("_NET_WM_STATE");
  xcb_intern_atom_cookie_t wm_state_fs_ck =
          getCookieForAtom("_NET_WM_STATE_FULLSCREEN");

#define _NET_WM_STATE_REMOVE        0    // remove/unset property
#define _NET_WM_STATE_ADD           1    // add/set property
#define _NET_WM_STATE_TOGGLE        2    // toggle property

  xcb_client_message_event_t ev;
  // memset (&ev, 0, sizeof (ev));
  ev.response_type = XCB_CLIENT_MESSAGE;
  ev.type = getReplyAtomFromCookie(wm_state_ck);
  ev.format = 32;
  ev.window = window;
  // ev.data.data32[0] = fullscreen ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
  ev.data.data32[0] = _NET_WM_STATE_TOGGLE;
    ev.data.data32[1] = getReplyAtomFromCookie(wm_state_fs_ck);
    ev.data.data32[2] = XCB_ATOM_NONE;
    ev.data.data32[3] = 0;
    ev.data.data32[4] = 0;

  xcb_send_event(
      connection,
      1,
      window,
      XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
      (const char*)(&ev));
}

void updateWindowTitle() {
  // TODO(bmonkey): should be per secound, and not per frame (breaks mouse input)
  //windowTitle << programTile << " - FPS: " << fps_current;
  //windowTitle << "FPS: " << Timer::Instance().getFPS();
  setWindowTitle("FPS: ");
}

void renderFrame() {
  updateWindowTitle();
  swapBuffers();
}



void init(char* title) {

  programTile = title;


  //input = new XCBInput(connection, this);

  // Toggle mouse at init
  unsigned halfWidth = width/2;
  unsigned halfHeight = height/2;
  //toggleMouseGrab();
  if (grab)
      XWarpPointer(
              display, None, window, 0, 0,
              width, height, halfWidth, halfHeight);
/*
  Scene::Instance().setResolution(width, height);

  Scene::Instance().getCurrentCamera()->setAspect(
          static_cast<float>(width)/
          static_cast<float>(height));
*/
}

int main(int argc, char** argv) {
  fullscreen = false;
  grab = false;
  quit = false;
  initScreen();
  initFrameBuffer();
  createGLContext();
  createColorMap();

  createWindow();
  
  createBlankCursor();
  
  int x = 0;
  
  while(x<5000) {
    renderFrame();
    updateWindowTitle();
    x++;
    }
  
  
      /* Cleanup 
    glXDestroyWindow(display, glxwindow);
    xcb_destroy_window(connection, window);
    glXDestroyContext(display, context);
    XCloseDisplay(display);
      */
  
  return 0;
}



#include <xcb/xcb.h>

int main(){
    int screen_num;
    xcb_connection_t   *conn;
    xcb_window_t          window;
    xcb_drawable_t        root_drawable;
    uint32_t          value_mask, value_list[4];
    xcb_screen_t          *root;
    
    conn = xcb_connect(0, &screen_num);
    
    root = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
    root_drawable = root->root;

    window = xcb_generate_id(conn);

    /* initialize the value list */
    value_mask = XCB_CW_EVENT_MASK;
    value_list[0] = XCB_EXPOSE;

    /* Create the window */
    xcb_create_window(conn, /* xcb_connection_t */
            0,  /* depth, 0 means it will copy it from the parent */
            window, root_drawable, /* window and parent */
            0, 0,   /* x and y */
            600, 600,   /* width and height */
            0,  /* border width */
            XCB_WINDOW_CLASS_INPUT_OUTPUT,    /* class */
            root->root_visual,   /* xcb_visualid_t */
            value_mask, value_list); /* LISTofVALUES */

    xcb_map_window(conn, window);

    xcb_flush(conn);
    sleep(2);
    
    xcb_disconnect(conn);
    return 0;
}

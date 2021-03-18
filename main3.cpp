#include <stdio.h>
#include <stdlib.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int screen;
Window window;
Display * display;
GC graphics_context;
unsigned long black, white, red, blue;

void init();
void close();
void draw();

unsigned long RGB(int red, int green, int blue);

struct dot {
    int x;
    int y;
} dot;

/*
 * Main function of the program
 */
int main()
{
    init();

    XEvent event;
    KeySym key;
    char text[255];

    while (true)
    {
        XNextEvent(display, &event);

        if (event.type == Expose && event.xexpose.count == 0) {
            draw();
        }

        if (event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, 0) == 1) {
            if (text[0] == 'q') {
                close();
            }

            printf("You pressed the q key");
        }

        if (event.type == ButtonPress) {
            int x = event.xbutton.x;
            int y = event.xbutton.y;

            XSetForeground(display, graphics_context, red);
            XDrawLine(display, window, graphics_context, dot.x, dot.y, x, y);
            XSetForeground(display, graphics_context, blue);

            strcpy(text, "Hello");

            XDrawString(display, window, graphics_context, x, y, text, strlen(text));

            dot.x = x;
            dot.y = y;
        }
    }

    return 0;
}

/*
 * Initialize the program
 */
void init()
{
    dot.x = 100;
    dot.y = 100;

    display = XOpenDisplay((char *) 0);
    screen = DefaultScreen(display);
    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);
    red = RGB(255, 0, 0);
    blue = RGB(0, 0, 255);

    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 300, 300, 5, white, black);
    XSetStandardProperties(display, window, "My title 1", "My title 2", None, NULL, 0, NULL);
    XSelectInput(display, window, ExposureMask | ButtonPressMask | KeyPressMask);

    graphics_context = XCreateGC(display, window, 0, 0);
    XSetBackground(display, graphics_context, white);
    XSetForeground(display, graphics_context, black);
    XClearWindow(display, window);
    XMapRaised(display, window);
}

/*
 * Close the window
 */
void close()
{
    XFreeGC(display, graphics_context);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    exit(0);
}

/*
 * Draw the window
 */
void draw()
{
    XClearWindow(display, window);
}

/*
 * Generate a RGB color value
 */
unsigned long RGB(int red, int green, int blue)
{
    return blue + (green << 8) + (red << 16);
}
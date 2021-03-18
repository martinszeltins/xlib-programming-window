#include <stdio.h>
#include <stdlib.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display * dis;
int screen;
Window win;
GC gc;
unsigned long black, white, red, blue;

void init();
void close();
void draw();

unsigned long RGB(int r, int g, int b);

struct coord {
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
        XNextEvent(dis, &event);

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

            XSetForeground(dis, gc, red);
            XDrawLine(dis, win, gc, dot.x, dot.y, x, y);
            XSetForeground(dis, gc, blue);

            strcpy(text, "Hello");

            XDrawString(dis, win, gc, x, y, text, strlen(text));

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

    dis = XOpenDisplay((char *) 0);
    screen = DefaultScreen(dis);
    black = BlackPixel(dis, screen);
    white = WhitePixel(dis, screen);
    red = RGB(255, 0, 0);
    blue = RGB(0, 0, 255);

    win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 300, 300, 5, white, black);
    XSetStandardProperties(dis, win, "My title 1", "My title 2", None, NULL, 0, NULL);
    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);

    gc = XCreateGC(dis, win, 0, 0);
    XSetBackground(dis, gc, white);
    XSetForeground(dis, gc, black);
    XClearWindow(dis, win);
    XMapRaised(dis, win);
}

/*
 * Close the window
 */
void close()
{
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    XCloseDisplay(dis);
    exit(0);
}

/*
 * Draw the window
 */
void draw()
{
    XClearWindow(dis, win);
}

/*
 * Generate a RGB color value
 */
unsigned long RGB(int r, int g, int b)
{
    return b + (g<<8) + (r<<16);
}
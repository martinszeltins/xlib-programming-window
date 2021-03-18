#include <X11/Xlib.h>
#include <X11/Xutil.h>

Window window;
Display * display;

/*
 * Main function of the program
 */
int main()
{
    display = XOpenDisplay((char *) 0);

    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 300, 300, 5, 10, 50);
    XSetStandardProperties(display, window, "My title", "My icon", None, NULL, 0, NULL);
    XMapRaised(display, window);

    XEvent event;

    while (true) {
        XNextEvent(display, &event);
    }
}
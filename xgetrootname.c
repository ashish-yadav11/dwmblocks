#include <stdio.h>
#include <X11/Xlib.h>

int
main()
{
        char *name;
        Display *dpy;

        if (!(dpy = XOpenDisplay(NULL))) {
                fputs("Error: could not open display.\n", stderr);
                return 1;
        }
        if (XFetchName(dpy, DefaultRootWindow(dpy), &name) && name[0])
                puts(name);
        else
                fputs("No name has been set for the root window.\n", stderr);
        XFree(name);
        XCloseDisplay(dpy);
        return 0;
}

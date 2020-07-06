#include <stdio.h>
#include <X11/Xlib.h>

int
main()
{
        Display *dpy;
        char *name;

        if (!(dpy = XOpenDisplay(NULL))) {
                fputs("Error: could not open display.\n", stderr);
                return 1;
        }
        if (XFetchName(dpy, DefaultRootWindow(dpy), &name) && name[0])
                printf("%s\n", name);
        else
                puts("No name has been set for the root window.");
        XFree(name);
        XCloseDisplay(dpy);
        return 0;
}

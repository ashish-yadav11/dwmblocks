#include <stdio.h>
#include <X11/Xlib.h>

int
main()
{
        Display *dpy;
        Window root;
        char *name;

        if (!(dpy = XOpenDisplay(NULL))) {
                fputs("Error: could not open display.\n", stderr);
                return 1;
        }
        root = RootWindow(dpy, DefaultScreen(dpy));
        if (XFetchName(dpy, root, &name) && name[0])
                printf("%s\n", name);
        else
                puts("No name has been set for the root window.");
        XFree(name);
        XCloseDisplay(dpy);
        return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

Window find_window(Display *display, Window root, const char *window_class) {
    Window parent;
    Window *children;
    unsigned int nchildren;
    XClassHint class_hint;

    if (XQueryTree(display, root, &root, &parent, &children, &nchildren)) {
        for (unsigned int i = 0; i < nchildren; i++) {
            if (XGetClassHint(display, children[i], &class_hint)) {
                if (strcmp(class_hint.res_class, window_class) == 0) {
                    XFree(class_hint.res_name);
                    XFree(class_hint.res_class);
                    return children[i];
                }
                XFree(class_hint.res_name);
                XFree(class_hint.res_class);
            }
        }
        for (unsigned int i = 0; i < nchildren; i++) {
            Window found = find_window(display, children[i], window_class);
            if (found) {
                XFree(children);
                return found;
            }
        }
        if (children) {
            XFree(children);
        }
    }
    return 0;
}

void list_windows(Display *display, Window root) {
    Window parent;
    Window *children;
    unsigned int nchildren;
    XClassHint class_hint;

    if (XQueryTree(display, root, &root, &parent, &children, &nchildren)) {
        for (unsigned int i = 0; i < nchildren; i++) {
            if (XGetClassHint(display, children[i], &class_hint)) {
                printf("Window ID: %ld, Class: %s\n", children[i], class_hint.res_class);
                XFree(class_hint.res_name);
                XFree(class_hint.res_class);
            }
        }
        for (unsigned int i = 0; i < nchildren; i++) {
            list_windows(display, children[i]);
        }
        if (children) {
            XFree(children);
        }
    }
}

void toggle_window(Display *display, Window window) {
    XWindowAttributes attrs;
    XGetWindowAttributes(display, window, &attrs);

    if (attrs.map_state == IsViewable) {
        XUnmapWindow(display, window);
    } else {
        XMapWindow(display, window);
        XRaiseWindow(display, window);
        XGetWindowAttributes(display, window, &attrs);
        if (attrs.map_state == IsViewable) {
            XSetInputFocus(display, window, RevertToParent, CurrentTime);
        }
    }
}

void start_process(const char *path, char *const argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(path, argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

int find_and_toggle_windows(Display *display, Window root, const char *window_class) {
    Window parent;
    Window *children;
    unsigned int nchildren;
    XClassHint class_hint;
    int found = 0;

    if (XQueryTree(display, root, &root, &parent, &children, &nchildren)) {
        for (unsigned int i = 0; i < nchildren; i++) {
            if (XGetClassHint(display, children[i], &class_hint)) {
                if (strcmp(class_hint.res_class, window_class) == 0) {
                    XFree(class_hint.res_name);
                    XFree(class_hint.res_class);
                    toggle_window(display, children[i]);
                    found = 1;
                } else {
                    XFree(class_hint.res_name);
                    XFree(class_hint.res_class);
                }
            }
        }
        for (unsigned int i = 0; i < nchildren; i++) {
            found |= find_and_toggle_windows(display, children[i], window_class);
        }
        if (children) {
            XFree(children);
        }
    }
    return found;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <window_class> [<path_to_process> [process_args...]]\n", argv[0]);
        fprintf(stderr, "%s list - to get a list of windows", argv[0]);
        return EXIT_FAILURE;
    }

    const char *window_class = argv[1];
    const char *path = argv[2];
    char **process_args = &argv[2];

    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open X display\n");
        return EXIT_FAILURE;
    }

    if (strcmp(window_class, "list") == 0) {
        list_windows(display, DefaultRootWindow(display));
        XCloseDisplay(display);
        return EXIT_SUCCESS;
    }

    if (argc < 3) {
        fprintf(stderr, "Path to process is required when window_class is not 'list'\n");
        XCloseDisplay(display);
        return EXIT_FAILURE;
    }

    int found = find_and_toggle_windows(display, DefaultRootWindow(display), window_class);
    if (found) {
        XCloseDisplay(display);
        return EXIT_SUCCESS;
    } else {
        if (argc < 3) {
            fprintf(stderr, "No windows found and no process specified to start\n");
            XCloseDisplay(display);
            return EXIT_FAILURE;
        }
        start_process(path, process_args);
    }

    XCloseDisplay(display);
    return EXIT_SUCCESS;
}

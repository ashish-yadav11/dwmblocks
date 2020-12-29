#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>

#define CMDLENGTH                       50
#define STTLENGTH                       256
#define NILL                            INT_MIN
#define LOCKFILE                        "/tmp/dwmblocks.pid"

typedef struct {
        char *const pathu;
        char *const pathc;
        const int interval;
        const int signal;
        char cmdoutcur[CMDLENGTH];
        char cmdoutprv[CMDLENGTH];
} Block;

#include "blocks.h"

static void buttonhandler(int sig, siginfo_t *info, void *ucontext);
static void cleanup();
static void getcmd(Block *block, int sigval);
static void setroot();
static void setupsignals();
static void sighandler(int sig, siginfo_t *si, void *ucontext);
static void statusloop();
static void termhandler(int sig);
static int updatestatus();
static void writepid();

static char statustext[STTLENGTH];
static char *delim;
static size_t delimlength;
static Display *dpy;
static sigset_t blocksigmask;

void
buttonhandler(int sig, siginfo_t *info, void *ucontext)
{
        sig = info->si_value.sival_int >> 8;
        for (Block *block = blocks; block->pathu; block++)
                if (block->signal == sig)
                        switch (fork()) {
                                case -1:
                                        perror("buttonhandler - fork");
                                        break;
                                case 0:
                                {
                                        char button[] = { '0' + (info->si_value.sival_int & 0xff), '\0' };
                                        char *arg[] = { block->pathc, button, NULL };

                                        close(ConnectionNumber(dpy));
                                        setsid();
                                        execv(arg[0], arg);
                                        perror("buttonhandler - child - execv");
                                        _exit(127);
                                }
                        }
}

void
cleanup()
{
        unlink(LOCKFILE);
        XStoreName(dpy, DefaultRootWindow(dpy), "");
        XCloseDisplay(dpy);
}

void
getcmd(Block *block, int sigval)
{
        int fd[2];

        if (pipe(fd) == -1) {
                perror("getcmd - pipe");
                exit(1);
        }
        switch (fork()) {
                case -1:
                        perror("getcmd - fork");
                        exit(1);
                case 0:
                        close(ConnectionNumber(dpy));
                        close(fd[0]);
                        if (fd[1] != STDOUT_FILENO) {
                                if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
                                        perror("getcmd - child - dup2");
                                        exit(1);
                                }
                                close(fd[1]);
                        }
                        if (sigval == NILL) {
                                char *arg[] = { block->pathu, NULL };

                                execv(arg[0], arg);
                        } else {
                                char buf[12];
                                char *arg[] = { block->pathu, buf, NULL };

                                snprintf(buf, sizeof buf, "%d", sigval);
                                execv(arg[0], arg);
                        }
                        perror("getcmd - child - execv");
                        _exit(127);
                default:
                        close(fd[1]);
                        if (read(fd[0], block->cmdoutcur, CMDLENGTH) == -1) {
                                perror("getcmd - read");
                                exit(1);
                        }
                        close(fd[0]);
        }
}

void
setroot()
{
        if (updatestatus()) {
                XStoreName(dpy, DefaultRootWindow(dpy), statustext);
                XSync(dpy, False);
        }
}

void
setupsignals()
{
        struct sigaction sa;

        /* populate blocksigmask */
        sigemptyset(&blocksigmask);
        sigaddset(&blocksigmask, SIGHUP);
        sigaddset(&blocksigmask, SIGINT);
        sigaddset(&blocksigmask, SIGTERM);
        for (Block *block = blocks; block->pathu; block++)
                if (block->signal > 0)
                        sigaddset(&blocksigmask, SIGRTMIN + block->signal);

        /* setup signal handlers */
        /* to handle HUP, INT and TERM */
        sa.sa_flags = SA_RESTART;
        sigemptyset(&sa.sa_mask);
        sa.sa_handler = termhandler;
        sigaction(SIGHUP, &sa, NULL);
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGTERM, &sa, NULL);

        /* to ignore unused realtime signals */
        // sa.sa_flags = SA_RESTART;
        // sigemptyset(&sa.sa_mask);
        sa.sa_handler = SIG_IGN;
        for (int i = SIGRTMIN + 1; i <= SIGRTMAX; i++)
                sigaction(i, &sa, NULL);

        /* to prevent forked children from becoming zombies */
        sa.sa_flags = SA_NOCLDSTOP | SA_NOCLDWAIT | SA_RESTART;
        // sigemptyset(&sa.sa_mask);
        sa.sa_handler = SIG_DFL;
        sigaction(SIGCHLD, &sa, NULL);

        /* to handle signals generated by dwm on click events */
        sa.sa_flags = SA_RESTART | SA_SIGINFO;
        // sigemptyset(&sa.sa_mask);
        sa.sa_sigaction = buttonhandler;
        sigaction(SIGRTMIN, &sa, NULL);

        /* to handle update signals for individual blocks */
        sa.sa_flags |= SA_NODEFER;
        sa.sa_mask = blocksigmask;
        sa.sa_sigaction = sighandler;
        for (Block *block = blocks; block->pathu; block++)
                if (block->signal > 0)
                        sigaction(SIGRTMIN + block->signal, &sa, NULL);
}

void
sighandler(int sig, siginfo_t *info, void *ucontext)
{
        sig -= SIGRTMIN;
        for (Block *block = blocks; block->pathu; block++)
                if (block->signal == sig)
                        getcmd(block, info->si_value.sival_int);
        setroot();
}

void
statusloop()
{
        int i;
        struct timespec t;

        /* first run */
        sigprocmask(SIG_BLOCK, &blocksigmask, NULL);
        for (Block *block = blocks; block->pathu; block++)
                if (block->interval >= 0)
                        getcmd(block, NILL);
        setroot();
        sigprocmask(SIG_UNBLOCK, &blocksigmask, NULL);
        t.tv_sec = INTERVALs, t.tv_nsec = INTERVALn;
        while (nanosleep(&t, &t) == -1)
                if (errno != EINTR) {
                        perror("statusloop - nanosleep");
                        exit(1);
                }
        i = 1;
        /* main loop */
        for (;; i++) {
                sigprocmask(SIG_BLOCK, &blocksigmask, NULL);
                for (Block *block = blocks; block->pathu; block++)
                        if (block->interval > 0 && i % block->interval == 0)
                                getcmd(block, NILL);
                setroot();
                sigprocmask(SIG_UNBLOCK, &blocksigmask, NULL);
                t.tv_sec = INTERVALs, t.tv_nsec = INTERVALn;
                while (nanosleep(&t, &t) == -1);
        }
}

void
termhandler(int sig)
{
        cleanup();
        exit(0);
}

/* returns whether block outputs have changed and updates statustext if they have */
int
updatestatus()
{
        char *s = statustext;
        char *c, *p; /* for cmdoutcur and cmdoutprv */
        const char *d; /* for delimiter */
        Block *block = blocks;

        /* checking half of the function */
        /* find the first non-empty block */
        for (;; block++) {
                /* all blocks are empty */
                if (!block->pathu)
                        return 0;
                /* contents of the block changed */
                if (*block->cmdoutcur != *block->cmdoutprv)
                        goto update0;
                /* skip delimiter handler for the first non-empty block */
                if (*block->cmdoutcur != '\n' && *block->cmdoutcur != '\0')
                        goto skipdelimc;
        }
        /* main loop */
        for (; block->pathu; block++) {
                /* contents of the block changed */
                if (*block->cmdoutcur != *block->cmdoutprv)
                        goto update1;
                /* delimiter handler */
                if (*block->cmdoutcur != '\n' && *block->cmdoutcur != '\0')
                        s += delimlength;
                /* skip over empty blocks */
                else
                        continue;
skipdelimc:
                /* checking for the first byte has been done */
                c = block->cmdoutcur + 1, p = block->cmdoutprv + 1;
                for (; *c != '\n' && *c != '\0'; c++, p++)
                        /* contents of the block changed */
                        if (*c != *p) {
                                s += c - block->cmdoutcur;
                                goto update2;
                        }
                s += c - block->cmdoutcur;
                /* byte containing info about signal number for the block */
                if (block->pathc && block->signal)
                        s++;
        }
        return 0;

        /* updating half of the function */
        /* find the first non-empty block */
        for (;; block++) {
                /* all blocks are empty */
                if (!block->pathu)
                        return 1;
update0:
                /* don't add delimiter before the first non-empty block */
                if (*block->cmdoutcur != '\n' && *block->cmdoutcur != '\0')
                        goto skipdelimu;
                *block->cmdoutprv = *block->cmdoutcur;
        }
        /* main loop */
        for (; block->pathu; block++) {
update1:
                /* delimiter handler */
                if (*block->cmdoutcur != '\n' && *block->cmdoutcur != '\0') {
                        d = delim;
                        while (*d != '\0')
                                *(s++) = *(d++);
                        *(s++) = DELIMITERENDCHAR;
                /* skip over empty blocks */
                } else {
                        *block->cmdoutprv = *block->cmdoutcur;
                        continue;
                }
skipdelimu:
                c = block->cmdoutcur, p = block->cmdoutprv;
update2:
                do {
                        *(s++) = *c;
                        *p = *c;
                        c++, p++;
                } while (*c != '\n' && *c != '\0');
                if (block->pathc && block->signal)
                        *(s++) = block->signal;
        }
        *s = '\0';
        return 1;
}

void
writepid()
{
        int fd;
        struct flock fl;

        fd = open(LOCKFILE, O_RDWR|O_CREAT, 0644);
        if (fd == -1) {
                perror("writepid - open");
                exit(1);
        }
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if (fcntl(fd, F_SETLK, &fl) == -1) {
                if (errno == EACCES || errno == EAGAIN) {
                        fputs("Error: another instance of dwmblocks is already running.\n", stderr);
                        exit(2);
                }
                perror("writepid - fcntl");
                exit(1);
        }
        if (ftruncate(fd, 0) == -1) {
                perror("writepid - ftruncate");
                exit(1);
        }
        if (dprintf(fd, "%ld", (long)getpid()) < 0) {
                perror("writepid - dprintf");
                exit(1);
        }
}

int
main(int argc, char *argv[])
{
        writepid();
        if (argc == 3 && strcmp(argv[1], "-d") == 0) {
                delim = argv[2];
                delimlength = strlen(delim) + 1;
        } else {
                delim = DELIMITER;
                delimlength = sizeof DELIMITER;
        }
        if (!(dpy = XOpenDisplay(NULL))) {
                fputs("Error: could not open display.\n", stderr);
                return 1;
        }
        setupsignals();
        statusloop();
        cleanup();
        return 0;
}

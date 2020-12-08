#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NILL                            INT_MIN
#define LOCKFILE                        "/tmp/dwmblocks.pid"

int
parsesignal(char *c, int *s)
{
        for (*s = 0; *c != '\0'; c++)
                if (*c >= '0' && *c <= '9') {
                        *s *= 10;
                        *s += *c - '0';
                } else
                        return 0;
        if ((*s += SIGRTMIN) > SIGRTMAX)
                return 0;
        return 1;
}

int
parsesigval(char *c, int *v)
{
        int s = 1;

        if (*c == '-') {
                s = -1;
                c++;
        } else if (*c == '+')
                c++;
        for (*v = 0; *c != '\0'; c++)
                if (*c >= '0' && *c <= '9') {
                        *v *= 10;
                        *v += *c - '0';
                } else
                        return 0;
        *v *= s;
        return 1;
}

void
sendsignal(int sig, union sigval sv)
{
        int fd;
        struct flock fl;

        fd = open(LOCKFILE, O_RDONLY);
        if (fd == -1) {
                if (errno == ENOENT) {
                        fputs("Error: no running instance of dwmblocks.\n", stderr);
                        exit(3);
                }
                perror("sendsignal - open");
                exit(1);
        }
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if (fcntl(fd, F_GETLK, &fl) == -1) {
                perror("sendsignal - fcntl");
                exit(1);
        }
        if (fl.l_type == F_UNLCK) {
                fputs("Error: no running instance of dwmblocks.\n", stderr);
                exit(3);
        }
        if (sigqueue(fl.l_pid, sig, sv) == -1) {
                if (errno == ESRCH) {
                        fputs("Error: no running instance of dwmblocks.\n", stderr);
                        exit(3);
                } else {
                        perror("sendsignal - sigqueue");
                        exit(1);
                }
        }
}

int
main(int argc, char *argv[])
{
        int sig;
        union sigval sv;

        if (argc > 1 && parsesignal(argv[1], &sig)) {
                if (argc == 2) {
                        sv.sival_int = NILL;
                        sendsignal(sig, sv);
                        return 0;
                } else if (argc == 3 && parsesigval(argv[2], &(sv.sival_int))) {
                        sendsignal(sig, sv);
                        return 0;
                }
        }
        fprintf(stderr, "Usage: %s <signal> [<sigval>]\n", argv[0]);
        return 2;
}

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NILL                            INT_MIN
#define LOCKFILE                        "/tmp/dwmblocks.pid"

int
parsesignal(char *arg, int *s)
{
        int i = 0;

        for (; *arg != '\0'; arg++)
                if (*arg >= '0' && *arg <= '9')
                        i = 10 * i + *arg - '0';
                else
                        return 0;
        if ((i += SIGRTMIN) > SIGRTMAX)
                return 0;
        *s = i;
        return 1;
}

int
parsesigval(char *arg, int *v)
{
        int s = 1, i = 0;

        if (*arg == '-') {
                s = -1;
                arg++;
        } else if (*arg == '+')
                arg++;
        for (; *arg != '\0'; arg++)
                if (*arg >= '0' && *arg <= '9')
                        i = 10 * i + *arg - '0';
                else
                        return 0;
        *v = s * i;
        return 1;
}

void
sendsignal(int sig, union sigval sv)
{
        int fd;
        struct flock fl;

        if ((fd = open(LOCKFILE, O_RDONLY)) == -1) {
                if (errno == ENOENT) {
                        fputs("Error: no running instance of dwmblocks.\n", stderr);
                        exit(3);
                }
                perror("sendsignal - open");
                exit(1);
        }
        fl.l_type = F_WRLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = 0;
        fl.l_len = 0;
        if (fcntl(fd, F_GETLK, &fl) == -1) {
                perror("sendsignal - fcntl");
                close(fd);
                exit(1);
        }
        close(fd);
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

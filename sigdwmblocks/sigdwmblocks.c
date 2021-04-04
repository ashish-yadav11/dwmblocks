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
parsesignal(char *arg)
{
        int i = 0;

        for (; *arg != '\0'; arg++)
                if (*arg >= '0' && *arg <= '9')
                        i = 10 * i + *arg - '0';
                else {
                        fputs("Usage: sigdwmblocks <signal> [<sigval>]\n", stderr);
                        exit(2);
                }
        if ((i += SIGRTMIN) > SIGRTMAX) {
                fputs("Error: <signal> out of range.\n", stderr);
                exit(2);
        }
        return i;
}

int
parsesigval(char *arg)
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
                else {
                        fputs("Usage: sigdwmblocks <signal> [<sigval>]\n", stderr);
                        exit(2);
                }
        return s * i;
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

        if (argc < 2 || argc > 3) {
                fputs("Usage: sigdwmblocks <signal> [<sigval>]\n", stderr);
                return 2;
        }
        sig = parsesignal(argv[1]);
        sv.sival_int = argc == 2 ? NILL : parsesigval(argv[2]);
        sendsignal(sig, sv);
        return 0;
}

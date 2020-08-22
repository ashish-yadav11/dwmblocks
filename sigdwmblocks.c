#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LOCKFILE                        "/tmp/dwmblocks.pid"

void
sendsignal(int signum, union sigval sv)
{
        int fd;
        struct flock fl;

        fd = open(LOCKFILE, O_RDONLY);
        if (fd == -1) {
                if (errno == ENOENT) {
                        fputs("Error: no running instance of dwmblocks.\n", stderr);
                        exit(2);
                }
                perror("sendsignal - fd");
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
                exit(2);
        }
        if (sigqueue(fl.l_pid, signum, sv) == -1) {
                if (errno == EINVAL) {
                        fputs("Error: invalid signal provided in argument.\n", stderr);
                        exit(3);
                } else if (errno == ESRCH) {
                        fputs("Error: no running instance of dwmblocks.\n", stderr);
                        exit(2);
                } else {
                        perror("sendsignal - sigqueue");
                        exit(1);
                }
        }
}

int
main(int argc, char *argv[])
{
        if (argc > 1) {
                int signal;
                union sigval sv;

                if (sscanf(argv[1], "%d", &signal) == 1 &&
                    signal > 0 && (signal += SIGRTMIN) <= SIGRTMAX) {
                        if (argc == 2) {
                                sv.sival_int = INT_MIN;
                                sendsignal(signal, sv);
                                return 0;
                        } else if (argc == 3 &&
                                   sscanf(argv[2], "%d", &(sv.sival_int)) == 1) {
                                sendsignal(signal, sv);
                                return 0;
                        }
                }
        }

        fprintf(stderr, "Usage: %s <signal> [<sigval>]\n", argv[0]);
        return 3;
}

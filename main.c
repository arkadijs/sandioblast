#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <libevdev/libevdev-uinput.h>

#include "decl.h"


static volatile sig_atomic_t keep_running = 1;

static void handle_signal(int sig) {
    (void)sig;
    keep_running = 0;
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s /dev/hidrawN\n", argv[0]);
        return 1;
    }
    char *usbdev_name = argv[1];

    int hid = open(usbdev_name, O_RDONLY);
    if (hid < 0) {
        fprintf(stderr, "unable to open mouse hid device %s: %s\n", usbdev_name, strerror(errno));
        return errno;
    }

    int err;
    struct libevdev_uinput *uidev;

    err = uinput_init(&uidev);
    if (err != 0) {
        fprintf(stderr, "unable to init uinput device: %s\n", strerror(-err));
        close(hid);
        return -err;
    }

    struct sigaction sa = {
        .sa_handler = handle_signal,
        .sa_flags = SA_RESETHAND,
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    unsigned int activated = 0, history = 0;
    while (keep_running) {
        unsigned char buf[7]; // 7-axis "joystick"

        int r = read(hid, buf, sizeof(buf));
        if (r != sizeof(buf)) {
            fprintf(stderr, "read() failed, read %d bytes: %s\n", r, strerror(errno));
            if (r == -1) {
                if (errno == EINTR) {
                    continue;
                }
                err = errno;
                break;
            }
            if (r == 0) {
                break;
            }
            continue;
        }

        const struct sandio_state state = sandio_decode(buf);

        unsigned int packed = state.top | (state.right << 8) | (state.left << 16);
        if (packed == history) {
            continue;
        }

        history = packed;
        bool sync = false;
        for (const struct rule_t *rule = rules; rule->mask != 0; ++rule) {
            if (packed & rule->mask & ~activated) {
                libevdev_uinput_write_event(uidev, EV_KEY, rule->code, 1);
                activated |= rule->mask;
                sync = true;
            }
            if (~packed & rule->mask & activated) {
                libevdev_uinput_write_event(uidev, EV_KEY, rule->code, 0);
                activated &= ~rule->mask;
                sync = true;
            }
        }

        if (sync) {
            libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
        }
    }

    if (activated) {
        for (const struct rule_t *rule = rules; rule->mask != 0; ++rule) {
            if (activated & rule->mask) {
                libevdev_uinput_write_event(uidev, EV_KEY, rule->code, 0);
            }
        }
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }

    libevdev_uinput_destroy(uidev);
    close(hid);

    return err;
}

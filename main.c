#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <libevdev/libevdev-uinput.h>

#include "decl.h"


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
        return -err;
    }

    unsigned int activated = 0, history = 0;
    while (true) {
        unsigned char buf[7]; // 7-axis "joystick"
        int r = read(hid, buf, sizeof(buf));
        if (r != sizeof(buf)) {
            fprintf(stderr, "read() failed, read %d bytes: %s\n", r, strerror(errno));
            if (r == -1) {
                return errno;
            }
        }
        const struct sandio_state state = sandio_decode(buf);

        unsigned int packed = state.top | (state.right << 8) | (state.left << 16);
        if (packed == history) {
            continue;
        }
        history = packed;
        bool sync = false;
        for (int i = 0;; ++i) {
            const struct rule r = rules[i];
            if (!r.mask) {
                break; // end of rules
            }
            if (packed & r.mask & ~activated) {
                libevdev_uinput_write_event(uidev, EV_KEY, r.code, 1);
                activated |= r.mask;
                sync = true;
            }
            if (~packed & r.mask & activated) {
                libevdev_uinput_write_event(uidev, EV_KEY, r.code, 0);
                activated &= ~r.mask;
                sync = true;
            }
        }
        if (sync) {
            libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
        }
    }

    libevdev_uinput_destroy(uidev);
}

#include <errno.h>

#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include "decl.h"


static const char* uinput_device_name = "Sandio keyboard emulation";

/* init uinput keyboard device and enable common key codes */
int uinput_init(struct libevdev_uinput **uinput_dev) {
    int err;
    struct libevdev *dev;
    struct libevdev_uinput *uidev;

    dev = libevdev_new();
    if (!dev) {
        return -ENOMEM;
    }

    libevdev_set_name(dev, uinput_device_name);

    libevdev_enable_event_type(dev, EV_KEY);
    for (unsigned int key = KEY_ESC; key < KEY_MIN_INTERESTING; ++key) {
        libevdev_enable_event_code(dev, EV_KEY, key, 0);
    }

    err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
    libevdev_free(dev);
    if (err != 0) {
        return err;
    }

    *uinput_dev = uidev; 

    return 0;
}

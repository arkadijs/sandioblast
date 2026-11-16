#include <errno.h>
#include <stddef.h>

#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include "decl.h"


static const char *uinput_device_name = "Sandio keyboard emulation";

/* init uinput keyboard device and enable key codes of the rules */
int uinput_init(struct libevdev_uinput **uinput_dev) {
    int err;
    struct libevdev *dev;
    struct libevdev_uinput *uidev;

    if (!uinput_dev) {
        return -EINVAL;
    }

    dev = libevdev_new();
    if (!dev) {
        return -ENOMEM;
    }

    libevdev_set_name(dev, uinput_device_name);
    libevdev_set_id_bustype(dev, BUS_USB);
    libevdev_set_id_vendor(dev, 0x19ca);
    libevdev_set_id_product(dev, 0x0001);

    libevdev_enable_event_type(dev, EV_KEY);

    for (const struct rule_t *rule = rules; rule->mask != 0; ++rule) {
        libevdev_enable_event_code(dev, EV_KEY, rule->code, NULL);
    }

    err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
    libevdev_free(dev);
    if (err != 0) {
        return err;
    }

    *uinput_dev = uidev;

    return 0;
}

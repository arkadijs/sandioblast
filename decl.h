#ifndef DECL_H
#define DECL_H

#include <libevdev/libevdev-uinput.h>


struct sandio_state {
    unsigned char top, right, left;
};

struct sandio_state sandio_decode(const void *encoded);

struct rule_t {
    unsigned int mask;
    unsigned int code;
};

extern const struct rule_t rules[];

int uinput_init(struct libevdev_uinput **uinput_dev);

#endif /* DECL_H */

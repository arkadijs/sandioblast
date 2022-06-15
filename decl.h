#include <libevdev/libevdev-uinput.h>


struct sandio_state {
    unsigned char top, right, left;
};

struct sandio_state sandio_decode(const void*);

struct rule {
    unsigned int mask;
    unsigned int code;
};

extern const struct rule *rules;

int uinput_init(struct libevdev_uinput **uinput_dev);

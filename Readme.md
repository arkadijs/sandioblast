## SandioTech 3D Game O2 mouse

SandioTech mouse joysticks mapper to keyboard keys for Linux using [libevdev](https://www.freedesktop.org/software/libevdev/doc/latest/).

To build the software install GNU Make and libevdev development package, e.g. `libevdev-dev` on Debian.

    make

Run under `sudo` or play with `udev` and user's supplementary groups to give access to `/dev/uinput` and `/dev/hidraw` devices.

    sudo ./sandioblast /dev/hidraw1

Stop with Ctrl-C.

To find out which `hidraw` device search kernel messages.

    dmesg | grep hidraw.*Sandio

> hid-generic 0003:19CA:0001.0009: input,hidraw1: USB HID v1.10 Multi-Axis Controller [Mindtribe Sandio 3D HID Mouse] on usb-0000:01:00.0-1.1/input1

Configure in `config.h` and recompile. Key codes could be found in `/usr/include/linux/input-event-codes.h`.

Thanks to the author of https://github.com/EssentialNPC/SandioKeyMapper for an inspiration.

MIT license.

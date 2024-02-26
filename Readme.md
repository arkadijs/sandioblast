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

### systemd setup

Create `/etc/udev/rules.d/99-sandio.rules`:

    ACTION=="add", SUBSYSTEM=="hidraw", ENV{DEVPATH}=="*.1/0003:19CA:0001.*", SYMLINK+="sandiotech_joysticks", ENV{SYSTEMD_WANTS}+="sandioblast.service", TAG+="systemd"

Create `/etc/systemd/system/sandioblast.service`:

    [Unit]
    Description=SandioTech 3D Game O2 mouse joysticks mapper to keyboard keys
    BindsTo=dev-sandiotech_joysticks.device
    After=dev-sandiotech_joysticks.device

    [Service]
    ExecStart=-/usr/local/bin/sandioblast /dev/sandiotech_joysticks

    [Install]
    WantedBy=multi-user.target

Reconfigure, as root:

    systemctl daemon-reload
    systemctl enable sandioblast
    udevadm control --reload

Unplug and plug the mouse back. Check:

    systemctl status sandioblast

Enjoy!


Thanks to the author of [SandioKeyMapper](https://github.com/EssentialNPC/SandioKeyMapper) for an inspiration.

MIT license.

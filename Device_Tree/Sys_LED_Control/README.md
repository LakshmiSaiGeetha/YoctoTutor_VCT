# System LED Control

## Objective

Control a system LED from Linux userspace using the LED interface exposed by the kernel.

## Board

* Board: i.MX6ULL 14x14 EVK
* Operating System: Embedded Linux
* Interface: Linux LED class

## Verification

After booting Linux, check the available LEDs:

```bash
ls /sys/class/leds/
```

The LED should appear under the Linux LED class interface.

## LED Control

To turn the LED ON:

```bash
echo 1 > /sys/class/leds/user2/brightness
```

To turn the LED OFF:

```bash
echo 0 > /sys/class/leds/user2/brightness
```

To check the current brightness:

```bash
cat /sys/class/leds/user2/brightness
```

## Result

The system LED was successfully controlled from Linux userspace using the `/sys/class/leds/` interface.


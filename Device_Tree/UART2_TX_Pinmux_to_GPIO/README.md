# UART2 TX Pinmux to GPIO

## Objective

Configure the UART2 TX pin as a GPIO and use it to control the `user2` LED through the Device Tree.

## Board

* Board: i.MX6ULL 14x14 EVK
* Bootloader: U-Boot
* Kernel: Linux
* Configuration: Device Tree

## Changes

The UART2 TX pin was originally configured for the UART function.

The pin was changed from:

```text
UART2_TX_DATA
```

to:

```text
GPIO1_IO20
```

The GPIO was then connected to the `user2` LED in the Device Tree.

## Device Tree

The main Device Tree changes are present in:

```text
mys-6ul-14x14-evk.dtsi
```

The generated Device Tree Blob is:

```text
mys-6ull-14x14-gpmi-weim.dtb
```

## Verification

After booting the board, verify that the LED is available:

```bash
ls /sys/class/leds/
```

Expected:

```text
user2
```

The LED can be controlled using:

```bash
echo 1 > /sys/class/leds/user2/brightness
```

and:

```bash
echo 0 > /sys/class/leds/user2/brightness
```

## Files

| File                           | Description                 |
| ------------------------------ | --------------------------- |
| `main.c`                       | Test C program              |
| `mys-6ul-14x14-evk.dtsi`       | Modified Device Tree source |
| `mys-6ull-14x14-gpmi-weim.dtb` | Compiled Device Tree Blob   |
| `README.md`                    | Documentation               |

## Result

UART2 TX pin was successfully configured as `GPIO1_IO20` and connected to the `user2` LED through the Device Tree.


# U-Boot

## Objective

Understand the U-Boot bootloader, its source-code structure, configuration system, build system, and boot process on the i.MX6ULL 14x14 EVK board.

The work covered:

* U-Boot commands
* U-Boot source-code structure
* Kconfig
* Makefile
* `menuconfig`
* `defconfig`
* U-Boot build process
* U-Boot environment variables
* Loading the Linux kernel
* Loading the Device Tree
* Booting Linux from the SD card

---

# 1. U-Boot Overview

U-Boot is the bootloader used to initialize the board and load the Linux kernel.

The basic boot flow is:

```text
Power ON
   ↓
Boot ROM
   ↓
U-Boot
   ↓
Initialize hardware
   ↓
Load Linux Kernel
   ↓
Load Device Tree
   ↓
Set boot arguments
   ↓
Boot Linux
```

---

# 2. U-Boot Commands

U-Boot provides commands through the serial console.

### Check available MMC devices

```bash
mmc list
```

### Select MMC device

```bash
mmc dev 0
```

### List files in the FAT partition

```bash
fatls mmc 0:1
```

### Load the Linux kernel

```bash
fatload mmc 0:1 0x83000000 zImage
```

### Load the Device Tree

```bash
fatload mmc 0:1 0x84000000 <device-tree>.dtb
```

### Set the Device Tree address

```bash
fdt addr 0x84000000
```

### Set Linux boot arguments

```bash
setenv bootargs 'console=ttymxc0,115200 root=/dev/mmcblk0p2 rootwait rw'
```

### Boot Linux

```bash
bootz 0x83000000 - 0x84000000
```

---

# 3. U-Boot Environment Variables

U-Boot stores configuration and boot-related information in environment variables.

### Display all environment variables

```bash
printenv
```

### Display a specific variable

```bash
printenv bootargs
```

### Set an environment variable

```bash
setenv bootargs 'console=ttymxc0,115200 root=/dev/mmcblk0p2 rootwait rw'
```

### Save environment variables

```bash
saveenv
```

---

# 4. U-Boot Source Code Structure

The U-Boot source tree contains different directories and files for different parts of the bootloader.

Important areas include:

```text
u-boot/
├── arch/
├── board/
├── cmd/
├── common/
├── drivers/
├── include/
├── Kconfig
├── Makefile
└── configs/
```

### `arch/`

Contains architecture-specific code.

For example:

```text
arch/arm/
```

contains ARM-specific code.

### `board/`

Contains board-specific initialization and configuration.

### `cmd/`

Contains implementations of U-Boot shell commands.

Examples:

```text
mmc
fatload
printenv
setenv
bootz
```

### `drivers/`

Contains hardware drivers such as:

```text
MMC
UART
GPIO
I2C
SPI
USB
```

### `include/`

Contains U-Boot header files.

### `configs/`

Contains board-specific default configuration files.

---

# 5. Kconfig

U-Boot uses **Kconfig** to define configuration options.

Kconfig files describe configurable features and their dependencies.

For example, configuration options can be used to enable:

```text
MMC support
USB support
Network support
UART support
Device Tree support
Command support
```

The configuration system generates the final `.config` file used during the build.

The main configuration file is:

```text
.config
```

---

# 6. Makefile

U-Boot uses the **Makefile** build system.

The top-level `Makefile` controls the build process and invokes the appropriate Makefiles in different source directories.

Examples:

```bash
make <defconfig>
make menuconfig
make
```

The Makefile determines:

* What needs to be compiled
* Which architecture is being built
* Which board configuration is selected
* Which source files are compiled
* Which dependencies need to be built
* How the final U-Boot image is generated

---

# 7. `menuconfig`

`menuconfig` provides an interactive interface for configuring U-Boot.

Run:

```bash
make menuconfig
```

A menu-based configuration interface appears.

From here, configuration options can be enabled or disabled.

For example:

```text
Device Drivers
    ↓
MMC
    ↓
Serial
    ↓
GPIO
    ↓
USB
```

After making changes, the configuration can be saved.

The selected configuration is stored in:

```text
.config
```

---

# 8. Defconfig

A **defconfig** is a predefined default configuration for a particular board.

Instead of manually selecting hundreds of options through `menuconfig`, a board's defconfig can be loaded.

The available configurations are located under:

```text
configs/
```

A typical command is:

```bash
make <board>_defconfig
```

For example:

```bash
make mx6ull_14x14_evk_defconfig
```

The exact defconfig name depends on the U-Boot version and board.

The defconfig initializes the `.config` file with the default configuration required for the board.

---

# 9. Configuration Flow

The relationship between defconfig, Kconfig, menuconfig, and `.config` can be understood as:

```text
Board Defconfig
      ↓
    Kconfig
      ↓
  .config
      ↑
      │
  menuconfig
      ↓
 Modified configuration
      ↓
    .config
      ↓
     Make
      ↓
U-Boot Image
```

### Typical workflow

```bash
make <board>_defconfig
```

Then optionally modify the configuration:

```bash
make menuconfig
```

Then build:

```bash
make
```

---

# 10. U-Boot Build Process

A typical build process is:

```bash
make <board>_defconfig
make menuconfig
make -j$(nproc)
```

The generated U-Boot image depends on the board and configuration.

For the i.MX6ULL platform, the generated image can include:

```text
u-boot.imx
```

This image can then be written to the appropriate boot location on the SD card.

---

# 11. SD Card Boot

The SD card contains the files required to boot Linux.

Typical files include:

```text
zImage
*.dtb
u-boot.imx
```

The boot process is:

```text
SD Card
   ↓
U-Boot
   ↓
Load zImage
   ↓
Load DTB
   ↓
Set bootargs
   ↓
bootz
   ↓
Linux Kernel
```

---

# 12. Verification

U-Boot commands can be used to verify that the required files are available.

For example:

```bash
mmc list
```

```bash
mmc dev 0
```

```bash
fatls mmc 0:1
```

The FAT partition should contain the required boot files.

The kernel can then be loaded:

```bash
fatload mmc 0:1 0x83000000 zImage
```

and the Device Tree:

```bash
fatload mmc 0:1 0x84000000 <device-tree>.dtb
```

Finally:

```bash
bootz 0x83000000 - 0x84000000
```

---

# 13. Key Concepts Learned

| Topic           | Purpose                               |
| --------------- | ------------------------------------- |
| U-Boot          | Bootloader                            |
| U-Boot commands | Interact with hardware and boot Linux |
| `cmd/`          | U-Boot command implementations        |
| Kconfig         | Defines configuration options         |
| Makefile        | Controls the build process            |
| `menuconfig`    | Interactive configuration             |
| defconfig       | Board's default configuration         |
| `.config`       | Current U-Boot configuration          |
| `configs/`      | Board-specific default configurations |
| `drivers/`      | Hardware drivers                      |
| `arch/`         | Architecture-specific code            |
| `board/`        | Board-specific code                   |
| `zImage`        | Linux kernel image                    |
| DTB             | Device Tree Blob                      |
| `bootz`         | Boots a zImage                        |

---

# Result

Successfully studied the U-Boot source-code structure, configuration system, Kconfig, Makefile, `menuconfig`, defconfig, U-Boot commands, environment variables, SD-card boot process, Linux kernel loading, Device Tree loading, and Linux booting on the i.MX6ULL 14x14 EVK.


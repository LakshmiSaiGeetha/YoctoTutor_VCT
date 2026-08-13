# MYS-6ULL UART2 TX/RX Loopback Test

## 1. Purpose

This document explains how to test UART2 TX and RX communication on the MYS-6ULL / i.MX6ULL board.

For this test, UART2 TX and UART2 RX are physically connected together using a jumper wire.

The test works as follows:

```text
                    MYS-6ULL

Linux
  |
  v
/dev/ttymxc1
  |
  v
UART2_TX
  |
  |  Jumper Wire
  |
  v
UART2_RX
  |
  v
/dev/ttymxc1
  |
  v
Received Data
```

If we transmit:

```text
Hello UART2
```

we should receive:

```text
Hello UART2
```

This test verifies the basic UART2 transmit and receive functionality.

---

# 2. Hardware Required

* MYS-6ULL / i.MX6ULL board
* UART serial console
* Jumper wire

---

# 3. Hardware Connection

Connect UART2 TX and UART2 RX using a jumper wire.

```text
UART2_TX  ----------------+
                          |
                          |
UART2_RX  ----------------+
```

In short:

```text
UART2_TX <----> UART2_RX
```

This is called a **UART hardware loopback**.

> **Important:** Connect only TX and RX together. Do not connect TX or RX to VCC or GND.

---

# 4. Login to the Board

Connect to the board using the serial console.

Example:

```text
imx6ull14x14evk login: root
root@imx6ull14x14evk:~#
```

The commands below are executed on the MYS-6ULL board.

---

# 5. Check UART Devices

Run:

```bash
ls /dev/ttymxc*
```

Depending on the Device Tree configuration, the available UART devices may include:

```text
/dev/ttymxc0
/dev/ttymxc1
```

For this loopback test, UART2 is accessed through:

```text
/dev/ttymxc1
```

---

# 6. Find Which UART Is Used by the Console

Run:

```bash
cat /proc/cmdline
```

Example:

```text
console=ttymxc0,115200 root=/dev/mmcblk0p2 rw rootwait
```

The important part is:

```text
console=ttymxc0,115200
```

This means:

```text
/dev/ttymxc0
```

is being used as the Linux console.

Therefore, for the UART2 loopback test we use:

```text
/dev/ttymxc1
```

### UART Selection

```text
/dev/ttymxc0  -> Linux console
/dev/ttymxc1  -> UART2 loopback test
```

---

# 7. Confirm UART2 Is Detected

Run:

```bash
dmesg | grep -i ttymxc
```

Example:

```text
[    1.557560] 2020000.serial: ttymxc0 at MMIO 0x2020000 (irq = 29, base_baud = 5000000) is a IMX
[    2.335792] 21e8000.serial: ttymxc1 at MMIO 0x21e8000 (irq = 70, base_baud = 5000000) is a IMX
```

The important line is the one containing:

```text
ttymxc1
```

This confirms that UART2 is detected by Linux.

> The exact MMIO address and IRQ values can differ depending on the board configuration and kernel version. The important part for this test is that `ttymxc1` is present.

---

# 8. Configure UART2

Configure `/dev/ttymxc1` for 115200 8N1:

```bash
stty -F /dev/ttymxc1 115200 raw -echo
```

The configuration is:

```text
Baud Rate : 115200
Data Bits : 8
Parity    : None
Stop Bits : 1
```

Therefore:

```text
115200 8N1
```

---

# 9. Start the UART Receiver

Run:

```bash
cat /dev/ttymxc1 &
```

Example:

```text
[1] 609
```

The `cat` command is now running in the background.

It continuously waits for data received through:

```text
/dev/ttymxc1
```

---

# 10. Send a Character

Now send the character `a`:

```bash
echo "a" > /dev/ttymxc1
```

Because TX and RX are connected together, the character travels:

```text
echo "a"
   |
   v
/dev/ttymxc1
   |
   v
UART2_TX
   |
   | Loopback
   v
UART2_RX
   |
   v
/dev/ttymxc1
   |
   v
cat
   |
   v
a
```

Expected output:

```text
a
```

If you see `a`, the basic UART2 loopback is working.

---

# 11. Send a String

Test with a string:

```bash
echo "Hello UART2" > /dev/ttymxc1
```

Expected output:

```text
Hello UART2
```

---

# 12. Send Numbers

Test with numbers:

```bash
echo "123456789" > /dev/ttymxc1
```

Expected output:

```text
123456789
```

---

# 13. Why Are We Using `echo`?

We use `echo` because it is a simple Linux command for sending text to a device file.

Example:

```bash
echo "Hello UART2" > /dev/ttymxc1
```

The `>` operator redirects the output of `echo` to:

```text
/dev/ttymxc1
```

Normal `echo` also adds a newline at the end.

Therefore:

```bash
echo "Hello UART2" > /dev/ttymxc1
```

sends:

```text
Hello UART2 + newline
```

This is useful because the received data appears on a separate line.

---

# 14. Send Data Without a Newline

If we don't want a newline, use:

```bash
echo -n "Hello UART2" > /dev/ttymxc1
```

Difference:

```bash
echo "Hello UART2"
```

sends:

```text
Hello UART2 + newline
```

while:

```bash
echo -n "Hello UART2"
```

sends:

```text
Hello UART2
```

without a newline.

For this test, normal `echo` is easier to use.

---

# 15. Check UART Configuration

To verify the UART configuration, run:

```bash
stty -F /dev/ttymxc1 -a
```

Look for:

```text
speed 115200 baud
cs8
-parenb
-cstopb
-echo
```

This confirms:

```text
Baud Rate : 115200
Data Bits : 8
Parity    : None
Stop Bits : 1
```

---

# 16. Stop the Receiver

When the test is finished, stop the background `cat` process:

```bash
kill %1
```

Example:

```text
[1]+  Terminated              cat /dev/ttymxc1
```

---

# 17. Complete Test Procedure

## Step 1: Connect Hardware

Connect:

```text
UART2_TX <----> UART2_RX
```

---

## Step 2: Login

```text
imx6ull14x14evk login: root
```

---

## Step 3: Check UART Devices

```bash
ls /dev/ttymxc*
```

Expected for this configuration:

```text
/dev/ttymxc0
/dev/ttymxc1
```

---

## Step 4: Check Console UART

```bash
cat /proc/cmdline
```

Confirm:

```text
console=ttymxc0,115200
```

Use:

```text
/dev/ttymxc1
```

for the UART2 loopback test.

---

## Step 5: Confirm UART2 Detection

```bash
dmesg | grep -i ttymxc
```

Confirm that:

```text
ttymxc1
```

is detected.

---

## Step 6: Configure UART2

```bash
stty -F /dev/ttymxc1 115200 raw -echo
```

---

## Step 7: Start Receiver

```bash
cat /dev/ttymxc1 &
```

---

## Step 8: Send Test Character

```bash
echo "a" > /dev/ttymxc1
```

Expected:

```text
a
```

---

## Step 9: Send Test String

```bash
echo "Hello UART2" > /dev/ttymxc1
```

Expected:

```text
Hello UART2
```

---

## Step 10: Send Test Numbers

```bash
echo "123456789" > /dev/ttymxc1
```

Expected:

```text
123456789
```

---

## Step 11: Stop Receiver

```bash
kill %1
```

---

# 18. Quick Test

After the hardware connection is already made, the actual UART2 test can be performed using:

```bash
stty -F /dev/ttymxc1 115200 raw -echo
```

Then:

```bash
cat /dev/ttymxc1 &
```

Send:

```bash
echo "a" > /dev/ttymxc1
```

```bash
echo "Hello UART2" > /dev/ttymxc1
```

```bash
echo "123456789" > /dev/ttymxc1
```

Expected:

```text
a
Hello UART2
123456789
```

Finally:

```bash
kill %1
```

---

# 19. Expected Result

| Test      | Transmitted   | Received      |
| --------- | ------------- | ------------- |
| Character | `a`           | `a`           |
| String    | `Hello UART2` | `Hello UART2` |
| Numbers   | `123456789`   | `123456789`   |

If all three tests produce the expected output:

```text
UART2 TX/RX Loopback Test: PASS
```

---

# 20. UART2 Loopback Workflow

```text
              MYS-6ULL
                  |
                  v
          Check UART devices
                  |
                  v
         Identify console UART
                  |
                  v
         Select /dev/ttymxc1
                  |
                  v
           Configure UART2
                  |
                  v
         Start UART receiver
                  |
                  v
              Send data
                  |
                  v
             UART2_TX
                  |
                  |
               TX <-> RX
                Loopback
                  |
                  v
             UART2_RX
                  |
                  v
            Receive data
                  |
                  v
            Verify output
                  |
                  v
            Stop receiver
```

---

# 21. Important Commands Reference

| Purpose              | Command                                 |
| -------------------- | --------------------------------------- |
| List UART devices    | `ls /dev/ttymxc*`                       |
| Check console UART   | `cat /proc/cmdline`                     |
| Check UART detection | `dmesg \| grep -i ttymxc`               |
| Configure UART       | `stty -F /dev/ttymxc1 115200 raw -echo` |
| Start receiver       | `cat /dev/ttymxc1 &`                    |
| Send character       | `echo "a" > /dev/ttymxc1`               |
| Send string          | `echo "Hello UART2" > /dev/ttymxc1`     |
| Send numbers         | `echo "123456789" > /dev/ttymxc1`       |
| Check UART settings  | `stty -F /dev/ttymxc1 -a`               |
| Stop receiver        | `kill %1`                               |

---

# 22. Troubleshooting

## No `/dev/ttymxc1`

Check:

```bash
ls /dev/ttymxc*
```

Then:

```bash
dmesg | grep -i ttymxc
```

If `ttymxc1` is not present, check the Linux Device Tree and UART2 configuration.

---

## No Data Received

First check the physical connection:

```text
UART2_TX <----> UART2_RX
```

Then configure UART2 again:

```bash
stty -F /dev/ttymxc1 115200 raw -echo
```

Start the receiver:

```bash
cat /dev/ttymxc1 &
```

Then send:

```bash
echo "test" > /dev/ttymxc1
```

Expected:

```text
test
```

---

## Data Appears on the Same Line

Use normal `echo`:

```bash
echo "Hello UART2" > /dev/ttymxc1
```

instead of:

```bash
echo -n "Hello UART2" > /dev/ttymxc1
```

Normal `echo` adds the newline character.

---

## Console Is Not Working Correctly

Check:

```bash
cat /proc/cmdline
```

If it shows:

```text
console=ttymxc0,115200
```

then:

```text
/dev/ttymxc0
```

is the Linux console.

Do not use `/dev/ttymxc0` for the loopback test.

Use:

```text
/dev/ttymxc1
```

for UART2.

---

# 23. Final Result

The UART2 hardware loopback test is successful when the data transmitted through UART2 TX is received correctly through UART2 RX.

Example:

```text
Transmit:

Hello UART2

Receive:

Hello UART2
```

Therefore:

```text
UART2 TX/RX Loopback Test = PASS
```

This test verifies the basic UART2 transmit and receive functionality of the MYS-6ULL / i.MX6ULL board.


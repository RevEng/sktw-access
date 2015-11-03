General notes
=============

BBB image
---

Using Debian 7.9 with Linux kernel 3.8.13-bone79 from
https://rcn-ee.com/rootfs/bb.org/release/2015-11-03/lxde-4gb/BBB-eMMC-flasher-debian-7.9-lxde-armhf-2015-11-03-4gb.img.xz.

Enabling pins
---

Installed `beaglebone-universal-io` using the following:

````
cd /opt/source/beaglebone-universal-io
make install
````

Enabled PWM on P9_14 (EHRPWM1A) using the following:

````
config-pin overlay cape-universaln
config-pin P9.14 pwm
echo 3 > /sys/class/pwm/export
echo 1000000 > /sys/class/pwm/pwm3/period_ns
echo 500000 > /sys/class/pwm/pwm3/duty_ns
echo 1 > /sys/class/pwm/pwm3/run
````

Note that the duty needs to be set back to 0 before setting the period, or else setting the period may fail due to
being smaller than the duty.


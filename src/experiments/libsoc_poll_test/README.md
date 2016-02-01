libsoc poll test
===

Setup
---

This experiment tests the maximum speed and accuracy and capturing a sequence of pulses in a GPIO pin using
libsoc by continually reading a pin's level. P9_14 (EHRPWM1A) is externally configured to generate a square wave
with a configurable period and a 50% duty cycle. P9_12 (GPIO_60) is configured as input. The program continually
loops, checking the current time and stopping after 5 seconds has elapsed. If time has not yet expired, it reads
the current level of the pin and compares that to the previous level. If the level has changed, it increments the
count of the number of transitions seen. The loop ends after 5 seconds have elapsed. The total number of transitions
is divided by 2 (to get the number of full cycles) and again by 5 (to get the number of full cycles per second) and
this is written to the console.

Observations
---

````
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 10000000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 5000000 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 1 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# ./test
99.600000
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 5000000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 2500000 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 1 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# ./test
199.000000
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 2500000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 1250000 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 1 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# ./test
396.000000
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 1000000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 500000 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 1 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# ./test
990.400000
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 0 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 100000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 50000 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 1 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# ./test
9869.200000
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 50000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 25000 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# ./test
19705.000000
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 2500 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# echo 5000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_poll_test# ./test
54685.900000

Conclusion
---

This method is able to capture transitions accurately at speeds of 10-20KHz. It appears to breakdown by the 50KHz
mark, though it's difficult to tell whether it missed transitions or if the number of cycles in the experiment
was just poorly controlled.


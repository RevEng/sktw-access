libsoc interrupt test
===

Setup
---

This experiment tests the maximum speed and accuracy and capturing a sequence of pulses in a GPIO pin using
libsoc with an interrupt callback function. P9_14 (EHRPWM1A) is externally configured to generate a square wave
with a configurable period and a 50% duty cycle. P9_12 (GPIO_60) is configured as input with an edge-triggered
interrupt on both transitions. When the interrupt is triggered, the program increments a counter. It also checks
that the state of the pin has changed since the last interrupt and warns if it has not (since this would indicate
that a transition has been missed). After 5 seconds, the interrupt callback is stopped. The total number of
transitions is divided by 2 (to get the number of full cycles) and again by 5 (to get the number of full cycles
per second) and this is written to the console.

Observations
---
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 10000000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 5000000 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 1 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# ./test
100.000000
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 0 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 0 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 1000000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 500000 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 1 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# ./test
Got pin change but value is same; missed one?
[repeated 14 more times]
Got pin change but value is same; missed one?
980.900000
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 0 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 0 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 5000000 > /sys/class/pwm/pwm3/period_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 2500000 > /sys/class/pwm/pwm3/duty_ns
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# echo 1 > /sys/class/pwm/pwm3/run
root@beaglebone:/home/jamesc/sktw-access/src/experiments/libsoc_test# ./test
Got pin change but value is same; missed one?
Got pin change but value is same; missed one?
Got pin change but value is same; missed one?
Got pin change but value is same; missed one?
Got pin change but value is same; missed one?
199.100000

Conclusion
---

At 100Hz it was successfully able to measure every cycle, but with as little as 200Hz, it was already beginning
to miss transitions. This would not be feasible for use with the card reader.

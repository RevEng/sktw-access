libsocpy GPIO interrupt test
===

Setup
---

This experiment measures the maximum speed at which pin changes can reliably be detected
using the [libsocpy][] wrapper around [libsoc][].

Pin P9_14 (EHRPWM1A) is used as a square wave generator. This is connected via a wire to
Pin P9_12. An edge-triggered interrupt is configured to execute a callback function on
both rising and falling edges. The callback function counts the number of transitions
that occur. It also checks to ensure that the state of the pin is the inverse of the
previous state and warns if this is not the case (which would indicate that a transition
has been missed). This experiment runs for 5 seconds. After the experiment is over, the
number of transitions is divided by 2 (to get the number of full cycles) and again by
5 (to get the number of cycles per second) and this value is written to the console.

Observations
---

````
jamesc@beaglebone:~/sktw-access/src/experiments/libsocpy_test$ sudo python -u test.py 10  
9.800000
jamesc@beaglebone:~/sktw-access/src/experiments/libsocpy_test$ sudo python -u test.py 100
Traceback (most recent call last):
  File "test.py", line 53, in <module>
    start(float(sys.argv[1]))
  File "test.py", line 25, in start
    pwm_period.close()
IOError: [Errno 22] Invalid argument
jamesc@beaglebone:~/sktw-access/src/experiments/libsocpy_test$ sudo python -u test.py 100
Got pin change by value is same; missed one?
Got pin change by value is same; missed one?
Got pin change by value is same; missed one?
99.400000
jamesc@beaglebone:~/sktw-access/src/experiments/libsocpy_test$ sudo python -u test.py 200
Got pin change by value is same; missed one?
[repeated 10 more times]
Got pin change by value is same; missed one?
197.400000
jamesc@beaglebone:~/sktw-access/src/experiments/libsocpy_test$ sudo python -u test.py 300
Got pin change by value is same; missed one?
[repeated 17 more times]
Got pin change by value is same; missed one?
294.800000
jamesc@beaglebone:~/sktw-access/src/experiments/libsocpy_test$ sudo python -u test.py 500
Got pin change by value is same; missed one?
[repeated 93 more times]
Got pin change by value is same; missed one?
470.500000
sys.excepthook is missing
lost sys.stderr
````

Conclusion
---
At as little as 100Hz we already see some transitions being missed. The count remains
fairly accurate up to 300Hz, but by 400Hz there is already considerable error. This
will not be able to reliably read the 400us pulses from the card reader.

References
---

 * [libsoc]: https://github.com/jackmitch/libsoc
 * [libsocpy]: https://github.com/tonyarkles/libsocpy

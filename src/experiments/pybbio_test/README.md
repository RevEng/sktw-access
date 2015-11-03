PyBBIO GPIO test
===

Setup
---

This experiments tests using the PyBBIO GPIO module to capture pin changes using
interrupts and callbacks. I used the preinstalled PyBBIO 0.10 with the system
Python 2.7.3.

Pin P9.14 (EHRPWM1A) is used as a programmable square wave generator. PyBBIO is
then used to add an interrupt handler to both rising and falling edges on pin
P9.12 (GPIO_60). The interrupt handler increments a count of the number of transitions
seen so far. It also checks to see if the current state is different from the last seen
and prints a message warning if this is not so (which would indicate that a transition
has been missed). The process sleeps for 5 seconds while this runs. After 5 seconds,
the number of transitions is divided by 2 (to get the number of full cycles) and again
by 5 (to get the number of cycles per second) and this is displayed on the console.

Observations
---

````
jamesc@beaglebone:~/sktw-access/src/experiments/pybbio_test$ sudo python -u test.py 10 
9.800000
jamesc@beaglebone:~/sktw-access/src/experiments/pybbio_test$ sudo python -u test.py 100
Traceback (most recent call last):
  File "test.py", line 50, in <module>
    start(float(sys.argv[1]))
  File "test.py", line 25, in start
    pwm_period.close()
IOError: [Errno 22] Invalid argument
jamesc@beaglebone:~/sktw-access/src/experiments/pybbio_test$ sudo python -u test.py 100
Got pin change by value is same; missed one?
Got pin change by value is same; missed one?
99.400000
jamesc@beaglebone:~/sktw-access/src/experiments/pybbio_test$ sudo python -u test.py 200
Got pin change by value is same; missed one?
[repeated 10 more times]
Got pin change by value is same; missed one?
197.400000
jamesc@beaglebone:~/sktw-access/src/experiments/pybbio_test$ sudo python -u test.py 300
Got pin change by value is same; missed one?
[repeated 27 more times]
Got pin change by value is same; missed one?
293.400000
````

Conclusion
---

At as little as 100Hz, transitions were already being missed. It was also surprising to see
that the count was not exact, even at as little as 10Hz. It's not understood why there was
an IOError setting period in one of the runs, but the error was only observed when running
100 after running 10. This was repeatable. Running 100 again after 100 failed would succeed.
This would not be reliable for reading the 400us pulses from the card reader.

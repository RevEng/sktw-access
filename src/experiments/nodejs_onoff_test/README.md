Node.js on-off library test
===

Setup
---

This experiment tests using the GPIO portion of the on-off library within nodejs.
Pin P9_14 (EHRPWM1A) is used as a programmable square wave generator. Pin P9_12 (GPIO_60)
is used as an edge-triggered interrupt input. The interrupt is captured using the `onoff.Gpio.watch()`
command with a callback function. The callback function increments a counter every time a successful
transition is seen. The callback also checks if the new value seen is the same as the existing value
already recorded and logs a warning message if so. This would indicate that a transition was missed
in between, since each transition should alternate high-low-high-low-etc. The PWM is run and the
transitions counted for 5 seconds. The number of transitions is then divided by 2 to get the number
of cycles, and again by 5 to get the number of cycles per second (the frequency). This is displayed
on the console.

Observations
---

````
jamesc@beaglebone:~/sktw-access/src/experiments/nodejs_onoff_test$ sudo nodejs test.js 10 
9.8

jamesc@beaglebone:~/sktw-access/src/experiments/nodejs_onoff_test$ sudo nodejs test.js 100
Got pin change but value is same; missed one?
[repeated 7 more times]
Got pin change but value is same; missed one?
95.7

jamesc@beaglebone:~/sktw-access/src/experiments/nodejs_onoff_test$ sudo nodejs test.js 200
Got pin change but value is same; missed one?
[repeated 11 more times]
Got pin change but value is same; missed one?
192.1

jamesc@beaglebone:~/sktw-access/src/experiments/nodejs_onoff_test$ sudo nodejs test.js 300
Got pin change but value is same; missed one?
[repeated 35 more times]
Got pin change but value is same; missed one?
277.4
````

Conclusion
---

At as little as 100Hz, the GPIO edge-triggered callback mechanism of the on-off module for node.js
is unable to reliably capture inputs. I was also surprised to find that the count was not exact
at 10Hz. This would not be reliable for reading the 400us pulses from the card reader.

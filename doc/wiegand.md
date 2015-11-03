Wiegand protocol
===

Card reader
---

We are using a HID Prox Point Plus 6005B, which uses a variant of the Wiegand protocol. It works with 125kHz cards.


Wiegand signalling protocol
---

The protocol uses two lines, D0 and D1, both normally low. To signal the transmission of a 0 or 1, the reader will
send a positive-going pulse on the D0 or D1 line, respectively. The pulses appear to be 5V, regardless of the voltage
of the V+ given to the card reader. The pulse width is 400us followed by a 2000us idle time.

Example:

````
   5V     |---------|
D0 0V ----|<-400us->|------------------------------------------------------------------------------
                     <--------------------2000us---------------------->
   5V                                                                  |---------|
D1 0V -----------------------------------------------------------------|<-400us->|------------------
````

This represents sending a 0, followed by a 1.


Wiegand data protocol
---

When a card is presented to the reader, the reader will send a bitstream whose size is dependent on the contents of
the card.


Wiegand card format
---

### Standard 26-bit format

Our cards are all standard 26-bit Wiegand format, encoded as follows:

````
       12345678901234567890123456
Leading^^ Fac. ^^     Card     ^^Trailing
Parity || Code ||     Number   ||Parity
````

The 26-bit sequence is actually a 24-bit numeric sequence with a single leading and a single trailing parity bit.
The first 8-bits of the numeric sequence represent the facility code. The remaining 16-bits represent the card
number. For our purposes, we can treat this as a single 24-bit value. There are no gaurantees that cards will share
facility codes or even that card values are globally unique.

The parity bits are calculated based on splitting the 26-bit sequence into the trailing and leading 13-bit sequences:

```
       12345678901234567890123456
Leading^-----------^^-----------^Trailing
```

The leading parity bit is chosen such that the leading 13-bits have an even number of ones. Conversely, the trailing
parity bit is chosen such that the trailing 13-bits have an odd number of ones. This can be used to error detection,
but not for error correction.


### Other formats

Other card formats are possible and they may have more or fewer bits. The HID 6005B is specified to accept card formats
up to 85 bits in length.


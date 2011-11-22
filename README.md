# WiserClock
This is an alternative firmware for the awesome [WiseClock3][] by Florin C.

I'm developing this firmware to provide a cleaner, easier-extendable codebase which enables people to write awesome stuff for the WiseClock3 without much fuss. I started this because I found the codebase of the original firmware way too messy and confusing (no offense!).

## Configuration
The Clock has a serial interface. Every command is a single line ended by a newline (\n). The following commands are available:

- beep
Makes the clock emit a beep. Used to test the serial connection
- time hh:mm
Sets a new clock time.
- date dd/mm/yyyy
Sets a new date
- beepEnabled <1 or 0>
Enables (1) or disables (0) beeping on full- and half-hour.

## License
Mostly BSD and public domain. Authors:

- Much stuff from WiseClock3: FlorinC (http://timewitharduino.blogspot.com/)

- HT1632: Bill Westfield ("WestfW") (http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1225239439/0)

- DS3231: matt.joyce@gmail.com

- Pong algorithm: Nick (http://123led.wordpress.com/about/);

[WiseClock3]: http://timewitharduino.blogspot.com/2011/01/introducing-wise-clock-3.html

<img src="/assets/logo.png" width="300" />

# Open Source R2D2 Periscope Lighting PCB(s)

This open source project is a place to house all information around:
- Purchasing PCBs
- Constructing PCBs
- Uploading Code / Testing PCBs

This project assumes you have and are prepared to do entry level soldering to complete your periscope lighiting. 

You will need the following:

- 1x - Arduino pro-mini (with serial uploader or a second Arduino Mega/Uno)
- 10x - 90 deg pin headers
- 15x - standard pin headers
- 1x - R-Series Periscope housing (https://www.thingiverse.com/thing:3580234)
- 1x - Computer to upload code / test
- 3x - Standard 5mm red leds (for back of housing)
- 6x - Square 5mm x 7mm leds (for front diffuse pattern)

## Features
- x32 5v WS2812B-2020 RGB LEDs 
- RX/TX Communication
- Dedicated digital pins to each PCB side.

## PCB Ordering

The open source r-series periscope PCB(s) have been designed in a online application called EasyEDA that allows for a direct connection to JLCPCB manufacture. 

To being, click the following link below to open the EasyEDA PCB project:

[coming soon]

Once open, click the "fabrication" menu and choose "One click order PCB/SMT" and choose "ok". With any luck, you should have a new browser tab open to JLCPCB manufacture and our project file(s) ready to make some choices.

First is PCB color, while there is no actual cannon color for the PCBs I would recommend picking white as has the highest photon refraction index and assists in defusing color. Color options do not add additional cost, only extra time to manufacture.

Next, down in the "SMT Assembly" roll out, leave "Tooling holes" as "Added by JLCPCB" and click "confirm". This will take you through JLCPCB's surface mounted options / confirmation views. Just click "next" through all the steps until you get to a "add to cart" button. (As of 3/16/22, the price of a order costs $49.41).


## PCB Assembly

Start by breaking the PCBs at their perforated edges down into individual PCBs. Do a simple dry fit of all the PCBs ensuring that you have their orientations correct and the large pads where PCB faces meet can ultimately be soldered.

Once ready, begin by soldering in our 90 deg pins use for back 5mm standard lights (top PCB) and our VCC/GND & RX/TX (bottom PCB). 

Next break down our 15x pin headers into individual pins (not rows or two or more) and drop each standard pin header into the top PCB's Arduino pins. 

*DO NOT SOLDER ARDUINO HEADER PIN TO PCB YET* 

Next stack the Arduino pro-mini on to the pins and solder the pro-mini to these pins *ONLY; DO NOT SOLDER PINS TO PCB YET*. Once done pull the pro-mini & pins out of the PCB and carefully remove each pin header's little black standoff(s) to allow the pro-mini to sit flush with the top PCB.

Once pin header standoff(s) are all removed put the pro-mini back into the top PCB (now flush) and finally solder the pins to the PCB.

Last step is to identify all the touching edges of each PCB side where you will find large pads needing to be soldered.

That's it!

## PCB Code Upload

Its important to keep code upload and load power separate so we will first focus on uploading our code devoid of actually giving power to the bottom PCB's 5V/GND pins, but instead supply power to pro-mini alone as part of the code upload process. THESE TWO ARE DIFFERNT AND SHOULD NOT BE MIXED!

Reviewing the code, you have different options such as if sequences should be looped over, or retain a sequence until told to change, communication address etc. Make sure you read each option and are happy with your changes before we move onto uploading the code.

Here is how to upload code using a USB to Serial module (FTDI adapter) with the pro-mini:

https://www.youtube.com/watch?v=tKCiNepc6_4

Here is how to upload code using another Arduno Mega or Uno instead (my preferred method):

https://www.youtube.com/watch?v=YPW29i6IIh0

Both methods require you to hold down the reset button of the pro-mini prior to clicking upload to the code. Having clicked upload (while holding reset), you should see a message about code being compiled and then uploading; you want to release this reset button as soon as you see the code being uploaded, not while its being compiled.


## PCB Testing

Once you have successfully uploaded your code to the pro-mini its now a matter of simply running a 5V/GND through the bottom PCB hole(s) and into our 5V/GND pins (like wise with RX/TX pins).

You should now have a fully functional lighting PCB ready to be dropped into your periscope.


Thanks!
<img src="/assets/logo.png" width="300" />

# Open Source R2D2 Periscope Lighting PCB(s)

This open source project is a place to house all information around:
- Purchasing R2D2 periscope lighting PCBs
- Constructing the PCBs
- Uploading Code / Testing PCBs

While there are different assembily options based on cost, all assembily options assumes you are prepared to do entry level soldering to complete your periscope lighiting.

## Assembily options

Before you get started on purchasing a set of PCBs, you must first decide if you or the manufacturer will be surface mounting the PCBs. Its recommended you review both assembily options first before making any purchase.

- Minimal amount of soldering / Most Expensive
- Most amount of soldering / Least Expensive route

Minimal amount of soldering assembily option is the most expensive option cause the PCB circiut board manufacturer will be doing all the component (RGB LED, capacitors, resitors) for you under a 7 design x5 board(s). This leaves you with only having to break out each housing sides (tops, bottom, back, front, left and right), dry fitting them and soldering the large pads where each side edges meet and the arduino to the top. Apply power and away you go.

Most amount of soldering is the least expensive route cause you are requesting the PCB manfacturer to not surface mount anything for you. Instead (along with blank PCBs) you are given a metal stencile that is used in conjunction with low temp solder and heat gun to surface mount self purchased components. This all before even assembiling the sides together, mounting arduino and applying power.

You will need the following:

- 23x - standard pin headers
- 5x - standard 90deg pin headers
- 6x - Square 2mm x 7mm leds (for front https://www.mouser.com/ProductDetail/749-R7HD)
- 3x - Standard 5mm flat top red leds (for back https://lighthouseleds.com/5mm-diffused-flat-top-led-red-ultra-bright.html)
- 1x - Arduino pro-mini (with serial uploader or a second Arduino Mega/Uno)
- 1x - R-Series Periscope housing (https://www.thingiverse.com/thing:3580234)

## Features
- R2 Uppity Spinner 3 pin communication
- Optional RX/TX communication
- 26x 5v WS2812B-2020 RGB LEDs
- 8 different lighting modes
- Reverse polarity protection

## PCB Ordering

The open source r-series periscope PCB(s) have been designed in a online application called EasyEDA that allows for a direct connection to JLCPCB manufacture. 

To being, click the following link below to open the EasyEDA PCB project:

https://easyeda.com/editor#id=|f2d6e47b44a243a790c3440f8382e354|e78da6a5fc364d829aff3906f2ea11d9|08bb938e8ee346a4aacb5f3062c3c0c5|aa58631963ea423db45d047d47d0b54d|968a05f5e63642cc88bd50dabc5d8882

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
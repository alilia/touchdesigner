# TouchDesigner X Arduino snippets

![Arduino CI status bedge](https://github.com/alilia/touchdesigner/actions/workflows/main.yml/badge.svg)

The current repository contains snippets I build based on my personal needs. I expect those to be cross-component snippets: Touchdesinger, Python and Arduino (R4 Wifi).

Since I want to reuse them in the future, I am sharing them here, which makes me generalise the implementation. Hence, making them available for public use.

Feedback is appreciated either here, or via Instagram DM: [@ilia.feelin.good](https://www.instagram.com/ilia.feelin.good/).

## TDComm

Base class to abstract communications setup between Arduino and Touchdesigner.

### Arduino object custom init parameters

Data can come and go to and from Arduino in two ways:

1. Via serial, so data start marker to be published to serial first
1. Via pin, so pin number to be set

Therefore, a class constructor can accept

* `inputDataPinCustom` : pin number, where Arduino will expect data to come in
* `inputMarkerCustom` : what data marker to look for before starting processing serial data
* `outputDataPinCustom` : pin number, where Arduino will publish data to
* `outputMarkerCustom` : what data marker to look for before publishing to serial
* `timeoutMiliSecCustom` : in case no data is processed for that time, the object is reset and will wait for the marker

### Touchdesigner component custom parameters

* `Globalserial` : Touchdesigner can handle one Serial OUT DAT per Arduino, so this DAT's serial will be used for communication
* markers : what marker to wait for or send based on the type of class (see: `Arduino object custom init parameters`)
* `Amtofdevices` : reduces the upper limit of 0-255 range by that amount for values published to serial, so there is no clash between markers and values (note: TachInput is not considered, as both base speed and difference published to serial are higher than 255)

## RGBLed

Goal of this implementation is to be able to stream TOP to an RGB LED Panel via Serial. My RGB LEDs are:
* [8x8 panel](https://www.aliexpress.com/item/1005003901833984.html?spm=a2g0o.order_list.order_list_main.11.4db81802bIbykX): WS2812B-powered, 5V snake-shaped strip (also available: 16x16, 8x32, strip)
* [10(x3) strip](https://leddiszkont.hu/led-szalag-led-szalag-magic-1.-magyarorszagon-a-legolcsobb/LLSZ505048L2EVRGBMAG/adatlap.html): SM16703-powered, 12V strip with 3 RGB LEDs / chip (note, that this chip is GRB)

Implementation is based on [FastLED](https://github.com/FastLED/FastLED).

### Arduino object custom init parameters

* `matrixWidthCustom` / `matrixHeightCustom` : the size of the matrix, compatible with RGB Led strip (1x32)
* `rgbResolution` : compresses (bit packing) RGB values for the sake of data transmission speed, therefore FPS

## FanControl

The goal of this implementation is to be able to (PWM) control basic fans' speed by a CHOP. My fan is a [two-wire](https://youtu.be/UJK2JF8wOu8?t=67), 12V cooling fan ([JF1238B1SR-R](https://lomex.hu/pdf/jam/(jam)_jf1238-13.pdf)).

## TachInput

The goal of this implementation is to be able to detect when a user blows into a tube. The tube has a [three-wire](https://youtu.be/UJK2JF8wOu8?t=208) 12V cooling fan ([KF0610H1HM-R](https://lomex.hu/pdf/jam/(jam)_kf0610-01.pdf)) in it. So, the third, tachometer wire connected to Arduino with a pull-up resistor, receives two signals per rotation.

However, when accelerating the fan from a static position, only hard blows were detected. Therefore, a base speed is set to the fan. That way a slight change in speed is detected.

### Touchdesigner component custom parameters

* `Basespeed` : 12V cooling fan has a base speed powered by Arduino's 5V, which can vary. Therefore, base speed is set, speed above what value should be published to the serial by Arduino

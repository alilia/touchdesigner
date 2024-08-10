# TouchDesigner X Arduino snippets

Curent repository contains snippets I build based on my personal needs. I expect those to be cross-component snippets: Touchdesinger, Python and Arduino (R4 Wifi).

Since I want to reuse them in the future, I am sharing them here, which makes me to generalise the implementation. Hence, making them available for public use.

Feedback is appreciated either here, or via Instagram DM: [@ilia.feelin.good](https://www.instagram.com/ilia.feelin.good/).

## RGBLed

Goal of this implementation is to be able to stream TOP to an RGB LED Panel via Serial. [My panel](https://www.aliexpress.com/item/1005003901833984.html?spm=a2g0o.order_list.order_list_main.11.4db81802bIbykX) is a 16x16, WS2812B-powered snake-shaped strip. Implementation is based on [FastLED](https://github.com/FastLED/FastLED).

## FanControl

Goal of this implementation is to be able to (PWM) control basic fans' speed by a CHOP. My fan is a [two-wire](https://youtu.be/UJK2JF8wOu8?t=67), 12V cooling fan ([JF1238B1SR-R](https://lomex.hu/pdf/jam/(jam)_jf1238-13.pdf)).

## TachInput

Goal of this implementation is to be able to detect when a user blows into a tube. The tube has a [three-wire](https://youtu.be/UJK2JF8wOu8?t=208) 12V cooling fan ([KF0610H1HM-R](https://lomex.hu/pdf/jam/(jam)_kf0610-01.pdf)) in it. So, the third, tachometer wire connected to Arduino with a pull-up resistor, receives two signals per rotation.

However, when accelerating the fan from static position, only hard blows were detected. Therefore, a base speed is set to the fan. That way a slight change in speed is detected.

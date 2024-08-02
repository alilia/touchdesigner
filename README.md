Curent repository contains snippets I build based on my personal needs. I expect those to be cross-component snippets: Touchdesinger, Python and Arduino (R4 Wifi).

Since I want to reuse them in the future, I am sharing them here, which makes me to generalise the implementation. Hence, making them available for public use.

Feedback is appreciated either here, or via Instagram DM: [@ilia.feelin.good](https://www.instagram.com/ilia.feelin.good/).

## RGBLed

Goal of this implementation is to be able to stream TOP to an RGB LED Panel via Serial. [My panel](https://www.aliexpress.com/item/1005003901833984.html?spm=a2g0o.order_list.order_list_main.11.4db81802bIbykX) is a 16x16, WS2812B-powered snake-shaped strip. Implementation is based on [FastLED](https://github.com/FastLED/FastLED).

p.s.: since generalisation in progress, currently only one panel/strip is supported.

## FanControl

Goal of this implementatino is to be able to (PWM) control basic fans' speed by a CHOP. My fan is a [two-wire](https://youtu.be/UJK2JF8wOu8?t=67), 12V cooling fan ([JF1238B1SR-R](https://lomex.hu/pdf/jam/(jam)_jf1238-13.pdf)).

p.s.: since generalisation in progress, currently only one fan is supported.

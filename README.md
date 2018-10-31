photon-desk
==============

[![Desk Controller](http://img.youtube.com/vi/VQ_Kc9S85WM/0.jpg)](http://www.youtube.com/watch?v=VQ_Kc9S85WM "Desk Controller")

A wifi enabled adjustable height desk.

Using a Particle Photon, this project allows you to remotely control your [V3 GeekDesk](http://www.geekdesk.com/geekdesk-v3-frame-only) The V3 only comes with a simple up/down button panel and doesn't allow you to have any presets.

Using an HC-SR04 Ultrasonic distance sensor attached to the bottom of the desk and pointed at the floor, we can measure the height of the desk. Using a pair of opto-isolators tapped into the button panel we can control the desk. The buttons are connected to the desk controller through an rj45 cable. This allows up to connect to the buttons without doing any permanant modifications to the desk itself.

This project gives you the ability to get and set the height of your desk (in centimeters) using the Particle's REST api

For a great compatible Mac app that works with this controller, checkout https://github.com/bdolman/StandUp

```
$ particle call Desk getHeight
97

$ particle call Desk setHeight 50
```

## Parts List

* (1) [GeekdeskV3 Controller PCB](https://github.com/Hypnopompia/photon-desk/tree/master/pcb)
* (1) [Particle Photon](https://store.particle.io/collections/photon)
* (1) [HC-SR04 Sonic Ping Sensor](http://www.amazon.com/s/ref=nb_sb_ss_c_0_3?url=search-alias%3Daps&field-keywords=hc-sr04&sprefix=hc-sr04%2Caps%2C202)
* (1) [2-channel opto-isolator](https://www.digikey.com/product-detail/en/LTV-826/160-1362-5-ND/385832)
* (1) [Right-angle 4 pin female header](https://www.digikey.com/product-detail/en/PPTC041LGBN-RC/S5440-ND/775898)
* (2) [12 pin female header](https://www.digikey.com/product-detail/en/PPTC121LFBN-RC/S6100-ND/807231)
* (2) [220Ω resistors](https://www.digikey.com/product-detail/en/CFR-50JB-52-220R/220H-ND/1291)
* (2) [RJ45 8-pin connector](https://www.sparkfun.com/products/643)

## Assembly Instructions
Assembly instructions can be found here: [http://imgur.com/a/hMbX6](http://imgur.com/a/hMbX6)

## Geekdesk control panel rj45 pinout

* White (3) - GND
* Red (8) - Down Button (+5V)
* Brown (7) - Up Button (+5V)

![Assembled Board View](https://github.com/Hypnopompia/photon-desk/blob/master/pcb/assembled.jpg?raw=true "Assembled Board View")
![Top Board View](https://github.com/Hypnopompia/photon-desk/blob/master/pcb/board-top.png?raw=true "Top Board View")

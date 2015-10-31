photon-desk
==============

A wifi enabled adjustable height desk.

Using a Particle Photon, this project allows you to remotely control your [V3 GeekDesk](http://www.geekdesk.com/geekdesk-v3-frame-only) The V3 only comes with a simple up/down button panel and doesn't allow you to have any presets.

Using an HC-SR04 Ultrasonic distance sensor attached to the bottom of the desk and pointed at the floor, we can measure the height of the desk. Using a pair of opto-isolators tapped into the button panel we can control the desk. The buttons are connected to the desk controller through an rj45 cable. This allows up to connect to the buttons without doing any permanant modifications to the desk itself.

This project gives you the ability to get and set the height of your desk (in centimeters) using the Particle's REST api

```
$ particle call Desk getHeight
97

$ particle call Desk setHeight 50
```

## Parts List

* Particle Photon
* HC-SR04 Sonic Ping Sensor

## Geekdesk control panel rj45 pinout

* White (3) - GND
* Red (8) - Down Button (+5V)
* Brown (7) - Up Button (+5V)

![Top Board View](https://github.com/Hypnopompia/photon-desk/blob/master/pcb/board-top.png?raw=true "Top Board View")

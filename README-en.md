<h1 align="center">
  <a href="ссылка на видео"><img src="https://github.com/TrashRobotics/HalloweenPumpkin/blob/main/img/pumpkin.jpg" alt="HalloweenPumpkin" width="800"></a>
  <br>
    Halloween Pumpkin
  <br>
</h1>

<p align="center">
  <a href="https://github.com/TrashRobotics/HalloweenPumpkin/blob/main/README.md">Русский(Russian)</a> •
  <a href="https://github.com/TrashRobotics/HalloweenPumpkin/blob/main/README-en.md">English</a> 
</p>

# Description
Halloween pumpkin in a hurry

# Main parts
* arduino pro mini;
* tp4056 with protection;
* ckcs bs01 (or other) step-up power module 3В-4.2В (li-ion) -> 5В;
* rocker switch KCD1-11;
* 18650 battery;
* sg90 servo;
* 8 LED module WS2812 (ring);
* small speaker (I removed it from the old phone);
* hw-517 or other transistor switch; 
* [pumpkin, body and etc.](ссылка на thingiverse)

### Fasteners
* Self-tapping screw DIN7981 2.9x9.5 x4;
* Self-tapping screw DIN7982 2.2x9.5 x5;

# Wiring diagram
![Wiring diagram](https://github.com/TrashRobotics/HalloweenPumpkin/blob/main/img/schematic.png)

# Sketch  and sound
Sketch for arduino pro mini: **halloween_pumpkin/halloween_pumpkin.ino**     
There is a header file next to it **sound.h** with a playable audio track.   
If you want, you can replace it with your audio. Using the program
[Audacity](https://www.audacityteam.org) and script **sound2h.py**. 
See the video for details.


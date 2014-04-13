enigmatikViewer
====================
An image slider software that transitions with interaction and glitches. Build for #Rpi  

Details
=======
Enigmatik is a small stand-alone DIY box carying many buttons on it's faces letting you play a brainbreaker game. A RaspberryPi (hidden inside the box) reveals an image, and it's up to you to find the right button states to reveal the next image trough glitches.  
[more info soon]


This software handles all user interaction of the box. 
- It reads the (physical) button's state using it's GPIO interface.  
- Loads images from USB keys and stores them on local memory.  
- Shows the image slideshow and handles glitchy transitions depending on the buttons' states.  


Compiles on a RaspberryPi with Raspbian, OpenFrameworks 0.8.1, wiringpi and pigpio.  


- - - -

By Daan de Lange  
http://daandelange.com/  

[RaspberryPi](http://www.raspberrypi.org/)  
[_Framework_] [OpenFrameworks](http://openframeworks.cc/)  
[_Lib_] [pigpio](http://abyz.co.uk/rpi/pigpio/)  
[WiringPi](http://wiringpi.com/)  
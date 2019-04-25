This project creates a Matrix Arcade of sorts from a 64x64 RGB LED Matrix. 
=========================================================================

Hardware  
--------

4x &nbsp; &nbsp; &nbsp;[64x64 RGB LED Matrix](https://www.adafruit.com/product/1484)  
1x &nbsp; &nbsp; &nbsp;[Adafruit RGB Matrix HAT](https://www.adafruit.com/product/2345)&nbsp; &nbsp; &nbsp; &nbsp;  *Must solder pins 4 and 18 together  
1-4x &nbsp; [USB Gamepad](https://www.amazon.com/Buffalo-iBuffalo-Classic-USB-Gamepad/dp/B002B9XB0E/ref=sr_1_2?keywords=ibuffalo+snes+controller&qid=1555696140&s=gateway&sr=8-2)  
1x &nbsp; &nbsp; &nbsp;[15A AC to DC Power Supply ](https://www.amazon.com/gp/product/B01LXN7MN3/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)  
1x &nbsp; &nbsp; &nbsp;[5V 2A power supply](https://www.adafruit.com/product/276)  
1x &nbsp; &nbsp; &nbsp;[Raspberry Pi 3 B+](https://www.adafruit.com/product/3775)&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; *Any revision of Raspberry Pi board should work  

Setup  
-----
1. `sudo apt-get update`
2. `git clone --recursive https://github.com/Bhuemann/rpi-led-game-system.git`  
3. `cd rpi-led-game-system`  
4. `make`  
5. `sudo rmmod snd_bcm2835` The library we use requires this driver be unloaded  

To Run  
-------
1. `sudo matrix-arcade`  

Adding games or anything else
-----------------------------
The menu follows directory names inside `menu_root` folder. You may add any number of subdirectories to be displayed. 
It will automatically search for executables inside these directories and run them.

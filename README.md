Matrix Arcade
=============
This project creates a Matrix Arcade of sorts from a 64x64 RGB LED Matrix.  It is a project I have been working on in my free time to learn new areas of low level/embedded development. I plan on adding additonal things such as different software concepts presented in a visual way. 

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
5. `sudo rmmod snd_bcm2835` *The library we use to draw to the matrix requires this driver be unloaded  

To Run  
-------
1. `sudo matrix-arcade`  

Adding games or anything else
-----------------------------
Matrix-Arcade automatically reads the `menu_root` folder and displays it's directories as menu entries. 
Before this happens however, it will check if the directory contains an executable. If it does, it will run that
instead of displaying the directory as a menu entry.

So to add something of your own...
1. Create a folder structure inside `menu_root` that you want displayed
2. Inside this structure, pick a location where your executable will reside
3. Add your sources & headers somewhere inside this directory
4. Create a `Makefile` that compiles and places your executable inside this directory

Note: You will need to add relative paths in your makefile to indicate where certain header/source/libraries you are using reside inside the repo. You may checkout out existing sources inside `menu_root` as an example.

Thats it. You can either compile using your makefile from your directory or use the `matrix-arcade` makefile to automatically find and run it for you.

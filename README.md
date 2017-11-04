* Make sure you have pins 4 and 18 on the Adafruit LED Matrix Hat soldered together. Otherwise, the LED refresh has a lot of blinking.
* If you have the sound module snd_bcm2835 loaded, unload it using "sudo rmmod snd_bcm2835". That module and the LED library are not compatible.
* Add the rpi-rgb-led-matrix library from Github as a folder in the root directory of this project.
* Add a games folder with games in the root directory of this project.
* Compile the project using make.
* Run the ./build/gameSystem file, which starts the controllers and menu by running "sudo ./build/gameSystem --led-gpio-mapping=adafruit-hat-pwm".

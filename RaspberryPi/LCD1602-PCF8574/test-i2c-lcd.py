# Make sure i2c is enabled with sudo raspi-config
# pip install RPi-GPIO-I2C-LCD

from RPi_GPIO_i2c_LCD import lcd
from time import sleep

## Address of backpack
i2c_address = 0x27

## Initalize display
lcdDisplay = lcd.HD44780(i2c_address)

## Set string value to buffer
lcdDisplay.set("Hello",1)
lcdDisplay.set("World",2)

while(True):
    lcdDisplay.backlight("off")
    sleep(1)
    lcdDisplay.backlight("on")
    sleep(1)

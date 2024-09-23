import board
import neopixel
pixel_pin = board.NEOPIXEL
pixels = neopixel.NeoPixel(pixel_pin, 1, brightness=0.2, auto_write=False, pixel_order=neopixel.GRB)
pixels[0] = (24,3,46)
pixels.show()

print("i2c + pcd8544") # on serial

import busio
i2c_bus = busio.I2C(board.GP7, board.GP6) # RP2040
i2c_bus.try_lock()
i2c_scan = str(i2c_bus.scan())
print("i2c-scan:", i2c_scan)
i2c_bus.unlock()

import digitalio
import adafruit_pcd8544 # imports adafruit_framebuf
print("init PCD8544")
spi = busio.SPI(clock=board.GP2, MOSI=board.GP3)
dc = digitalio.DigitalInOut(board.GP4)  # data/command
cs = digitalio.DigitalInOut(board.GP1)  # Chip select
reset = digitalio.DigitalInOut(board.GP0)  # reset
backlight = digitalio.DigitalInOut(board.GP5)  # backlight - active low
display = adafruit_pcd8544.PCD8544(spi, dc, cs, reset)
display.bias = 4
display.contrast = 60
backlight.switch_to_output()
backlight.value = False
#display.fill(0) # Clear the display. (adafruit_framebuf)
display.text("found on I2C:", 0, 0, 1)
display.text(i2c_scan, 0, 8, 1, size=2)
#display.text("**************", 0, 0, 1) # text(string, x, y, color)
#display.text("12345678901234", 0, 8, 1)
#display.text("14x6 character", 0, 16, 1)
#display.text("display with", 0, 24, 1)
#display.text("PCD8544", 0, 32, 1)
#display.text("**************", 0, 40, 1)
display.show()

import time
while True:
  pixels[0] = (24,35,46)
  pixels.show()
  time.sleep(1)
  pixels[0] = (124,70,80)
  pixels.show()
  time.sleep(1)

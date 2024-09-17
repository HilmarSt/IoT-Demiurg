import time
import board
import neopixel

pixel_pin = board.NEOPIXEL
# The order of the pixel colors - RGB or GRB. Some NeoPixels have red and green reversed!
# For RGBW NeoPixels, simply change the ORDER to RGBW or GRBW.
ORDER = neopixel.GRB
pixels = neopixel.NeoPixel(pixel_pin, 1, brightness=0.2, auto_write=False, pixel_order=ORDER)

while True:
  pixels[0] = (24,35,46)
  pixels.show()
  time.sleep(1)
  pixels[0] = (124,70,80)
  pixels.show()
  time.sleep(1)

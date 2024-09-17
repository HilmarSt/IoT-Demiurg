# Feather M0 Express IO demo
# Welcome to CircuitPython 2.2 :)

import board
import gc
import time
import neopixel

gc.collect()   # make some rooooom

# One pixel connected internally!
dot = neopixel.NeoPixel(board.NEOPIXEL, 1, brightness=0.25)

# Built in red LED
led = DigitalInOut(board.D13)
led.direction = Direction.OUTPUT

# Helper to give us a nice color swirl
def wheel(pos):
    # Input a value 0 to 255 to get a color value.
    # The colours are a transition r - g - b - back to r.
    if (pos < 0):
        return [0, 0, 0]
    if (pos > 255):
        return [0, 0, 0]
    if (pos < 85):
        return [int(pos * 3), int(255 - (pos*3)), 0]
    elif (pos < 170):
        pos -= 85
        return [int(255 - pos*3), 0, int(pos*3)]
    else:
        pos -= 170
        return [0, int(pos*3), int(255 - pos*3)]


######################### MAIN LOOP ##############################

i = 0
while True:
  # spin internal LED around! autoshow is on
  dot[0] = wheel(i & 255)

  i = (i+1) % 256  # run from 0 to 255
  time.sleep(0.01) # make bigger to slow down

  print(".")

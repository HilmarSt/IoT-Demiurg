# coding=utf-8

"""This file shows some use of the rpi_TM1638 librairy"""

from time import sleep
from rpi_TM1638 import TMBoards

# my GPIO settings
# (one TM1638 board connected to GPIO19 for dataIO, GPIO13 for Clock, and GPIO26 for the STB)
CLK = 13  # Pin 33
DIO = 19  # Pin 35
STB = 26  # Pin 37
          # Pin 39 = GND (letzten 4 Pins innen)
# STB = 6, 26   # in case you have two TM1638 boards, connected to GPIO06 and GPIO26

# instanciante my TMboards
TM = TMBoards(DIO, CLK, STB, 0)

TM.clearDisplay()

# some LEDs manipulation
TM.leds[0] = True       # turn on led 0 (1st led of the 1st board)
TM.leds[5] = False      # turn off led 5 (5th led of the 1st board, since there is 8 leds per board)
# nur wenn 2nd board :
#TM.leds[12] = True      # turn on led 12 (5th led of the 2nd board, since there is 8 leds per board)


TM.segments[1] = '0'        # display '0' on the display 1 (2nd 7-segment display of the 1st board)
TM.segments[4] = '98.76'     # display '9876' on the 7-segment display number 4, 5, 6 and 7 (the point is on segment 5)
TM.segments[3, 1] = True     # turn on the segment #1 of the 7-segment number 3
# nur wenn 2nd board :
#TM.segments[8] = '01234567'


# while True:
# 	TM.segments[0] = 'OK  ' if TM.switches[0] else 'OK  '
# 	TM.segments[4] = 'OK  ' if TM.switches[1] else 'OK  '
# 	sleep(0.01)

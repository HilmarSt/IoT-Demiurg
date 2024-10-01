TXversion="main-Tx6.py"
### HW-adaption:
valFREQ0 = 0x00 # SMA-Antenne (geht 17 kHz vor)
# valFREQ0 = 0x30 # soldered Antenne
# valFREQ0 = 0xA9 # soldered Antenne: receiving telegrams from noisy transmitter ?

repeatAfter = 5 # send every ... seconds

# refactored code_tx.py from github.com/unixb0y/cpy-cc1101
from CC1101_common import *
import time

# print("Tx mode ASK     ") # string size = 16
# Feather M0 without gc.collect() : print("Tx mode ASK") string size < 16 ->
#   MemoryError: memory allocation failed @ line "print(''.join(list ..."
# possibly due to CircuitPython 5.3.0

if valFREQ0 == 0x00:
  print(" SMA antenna module configured")
else:
  print(" Soldered antenna module configured")

initSPI()
strobe(SRES)
initRegisters_TX()
writeSingleByte(FREQ0, valFREQ0)
print(" MDMCFG2:", hex(readSingleByte(MDMCFG2)), "(0x3? = ASK mode)")

if is_RPi_Pico() == True:
  import digitalio
  led = digitalio.DigitalInOut(board.LED)
  led.direction = digitalio.Direction.OUTPUT
  # else: led = DigitalInOut(board.D13) # Feather M0 built-in LED is on SPI SCK !

bitstring = "10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010100110011001101010100101011010100101101010101010101001101010010101010110101001011010011010011010101010101001010110011001011001101010011010100110100101100110101010010110011001011010101010010101101000"

data = []
for i in range(0,len(bitstring)/8):
    data.append(int(bitstring[i*8:i*8+8], 2))

import gc
gc.collect()

packet_nr = 0
while True:
    if is_RPi_Pico():
      led.value = False
    writeSingleByte(PKTLEN, len(data))
    strobe(SRX) # STX doesn't work
    while True:
        marcstate = readSingleByte(MARCSTATE)
        if (marcstate & 0x1F) == 0x0D: # Main Radio Control State (Machine State)
            break
        #else:
        #    print("MARCSTATE %d" % marcstate, end='')

    print("Sending packet of", len(data), "bytes")
    writeBurst(TXFIFO, data)
    strobe(STX)
    remaining_bytes = 1
    while remaining_bytes != 0:
        remaining_bytes = readSingleByte(TXBYTES) & 0x7F
    packet_nr += 1
    print("Packets sent:", packet_nr)
    strobe(SFTX) # flush TX FIFO
    strobe(SFRX)
    if is_RPi_Pico():
      led.value = True

    print(gc.mem_free())
    time.sleep(repeatAfter)

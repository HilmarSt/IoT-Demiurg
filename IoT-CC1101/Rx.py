### HW-adaption:
# valFREQ0 = 0x00 # SMA-Antenne (geht 17 kHz vor)
valFREQ0 = 0x30 # soldered Antenne
# valFREQ0 = 0xA9 # soldered Antenne: receiving telegrams from noisy transmitter ?

data_len = 0x17 # PKTLEN is programmed to 0x17, add 2 status bytes

# refactored code_rx.py from github.com/unixb0y/cpy-cc1101
from CC1101_common import *

print("Rx mode ASK") # MDMCFG2 = 0x3? = ASK
if valFREQ0 == 0x00:
  print(" SMA antenna module configured")
else:
  print(" Soldered antenna module configured")

initSPI()

if is_RPi_Pico() == False:
  import gc
  gc.collect()
  print("Memory used:", gc.mem_alloc(), " free:", gc.mem_free())

strobe(SRES)
initRegisters_RX()
writeSingleByte(FREQ0, valFREQ0)
writeSingleByte(MDMCFG2, 0x32) # ASK =0x32: with 16/16 sync word; =0x30: no sync word
writeSingleByte(PKTLEN, data_len - 2) # 0x32: 0x15; 0x30: 0x17 geht nicht

print("Chip version should be 0x14:", hex(readSingleByte(VERSION)))
print(" MDMCFG2:", hex(readSingleByte(MDMCFG2)))
if is_RPi_Pico() == False:
  print("Memory free:", gc.mem_free(), end='')
  gc.collect()
  print("  after gc.collect():", gc.mem_free())
print("timestamp (+1000): ", end='')
timestamp()
print("")

while True:
  strobe(SRX)
  print("waiting for data")
  gdo0wait()

  data = readBurst(RXFIFO, data_len)
  dataStr = ''.join(list(map(lambda x: "{0:0>8}".format(x[2:]), list(map(bin, data)))))
  newStr = dataStr[8:]
  timestamp()
  print(" Data: ", newStr)
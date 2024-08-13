### HW-adaption:
valFREQ0 = 0x00 # SMA-Antenne (geht 17 kHz vor)
# valFREQ0 = 0x30 # soldered Antenne
# valFREQ0 = 0xA9 # soldered Antenne: receiving telegrams from noisy transmitter ?

repeatAfter = 5 # send every ... seconds

# refactored code_tx.py from github.com/unixb0y/cpy-cc1101
from CC1101_common import *
import time

print("Tx mode ASK     ") # string size = 16
# Feather M0 without gc.collect() : print("Tx mode ASK") string size < 16 ->
#   MemoryError: memory allocation failed @ line "print(''.join(list ..."
# possibly due to CircuitPython 5.3.0

if valFREQ0 == 0x00:
  print(" SMA antenna module configured")
else:
  print(" Soldered antenna module configured")

initSPI()

if is_RPi_Pico() == False:
  import gc
  gc.collect()
  #print("Memory used:", gc.mem_alloc(), " free:", gc.mem_free())

strobe(SRES)
initRegisters_TX()

writeSingleByte(FREQ0, valFREQ0)

print("Chip version should be 0x14:", hex(readSingleByte(VERSION)))
print(" MDMCFG2:", hex(readSingleByte(MDMCFG2)))
if is_RPi_Pico() == False:
  #print("Memory free:", gc.mem_free(), end='')
  gc.collect()
  #print("  after gc.collect():", gc.mem_free())

bitstring = "10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010100110011001101010100101011010100101101010101010101001101010010101010110101001011010011010011010101010101001010110011001011001101010011010100110100101100110101010010110011001011010101010010101101000"

while True:
    data = []
    for i in range(0,len(bitstring)/8):
        data.append(int(bitstring[i*8:i*8+8], 2))

    writeSingleByte(PKTLEN, len(data))
    strobe(SRX)

    marcstate = readSingleByte(MARCSTATE) & 0x1F
    dataToSend = []

    #print("Main Radio Control State Machine State:",end='')
    while (marcstate != 0x0D):
        marcstate = readSingleByte(MARCSTATE) & 0x1F
        #print(" ",hex(marcstate) ,end='',sep='')
    #print(" MARCSTATE = 0x0D")

    # enable next line: "MemoryError: memory allocation failed"
    #print(''.join(list(map(lambda x: "{0:0>8}".format(str(bin(x)[2:])), data))))

    print("Sending packet of", len(data), "bytes")

    writeBurst(TXFIFO, data)
    time.sleep(0.002)
    strobe(STX)

    remaining_bytes = readSingleByte(TXBYTES) & 0x7F
    while remaining_bytes != 0:
        time.sleep(0.001)
        remaining_bytes = readSingleByte(TXBYTES) & 0x7F
        #print("Waiting until all bytes are transmited, remaining bytes: %d" % remaining_bytes)

    if (readSingleByte(TXBYTES) & 0x7F) == 0:
        print("Packet sent!\n")

    else:
        print(readSingleByte(TXBYTES) & 0x7F)

    time.sleep(repeatAfter)

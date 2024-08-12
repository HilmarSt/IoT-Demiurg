# not yet adapted to latest CC1101_common.py : remove gdo0 and use gdo0wait()

# refactored code_tx.py from github.com/unixb0y/cpy-cc1101
repeatAfter = 10 # send every ... seconds

from digitalio import DigitalInOut
import time

from CC1101_common import *

print("CPY-CC1101/code_tx.py")

#import gc
#gc.collect()
#print("Memory used:", gc.mem_alloc(), "free:", gc.mem_free())

initSPI()
gdo0 = DigitalInOut(board.D10)

strobe(SRES)
initRegisters_TX()

print("Chip version should be 0x14:", hex(readSingleByte(VERSION)))

bitstring = "10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010100110011001101010100101011010100101101010101010101001101010010101010110101001011010011010011010101010101001010110011001011001101010011010100110100101100110101010010110011001011010101010010101101000"
data = []
for i in range(0,len(bitstring)/8):
    data.append(int(bitstring[i*8:i*8+8], 2))

# while True:
writeSingleByte(PKTLEN, len(data))
strobe(SRX)

marcstate = readSingleByte(MARCSTATE) & 0x1F
dataToSend = []

#print("Main Radio Control State Machine State:",end='')
while (marcstate != 0x0D):
    marcstate = readSingleByte(MARCSTATE) & 0x1F
    #print(" ",hex(marcstate) ,end='',sep='')
#print(" MARCSTATE = 0x0D")

print(''.join(list(map(lambda x: "{0:0>8}".format(str(bin(x)[2:])), data))))

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

# time.sleep(repeatAfter)

### HW-adaption:
valFREQ0 = 0x00 # SMA-Antenne (geht 17 kHz vor)
# valFREQ0 = 0x30 # soldered Antenne
# valFREQ0 = 0xA9 # soldered Antenne: receiving telegrams from noisy transmitter ?

repeatAfter = 2 # send every ... seconds

# refactored code_tx.py from github.com/unixb0y/cpy-cc1101
from CC1101_common import *
import time
import digitalio
led = DigitalInOut(board.D13)
led.direction = digitalio.Direction.OUTPUT

print("Tx mode ASK     ") # string size = 16
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
print(" MDMCFG2:", hex(readSingleByte(MDMCFG2)))

if is_RPi_Pico() == False:
  import gc
  gc.collect()

bitstring = "10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010100110011001101010100101011010100101101010101010101001101010010101010110101001011010011010011010101010101001010110011001011001101010011010100110100101100110101010010110011001011010101010010101101000"
packet_nr = 0

while True:
    led.value = False
    data = []
    for i in range(0,len(bitstring)/8):
        data.append(int(bitstring[i*8:i*8+8], 2))

    writeSingleByte(PKTLEN, len(data))
    strobe(SRX)

    while True:
        marcstate = readSingleByte(MARCSTATE)
        if (marcstate & 0x1F) == 0x0D: # Main Radio Control State (Machine State)
            break
        #else:
        #    print("MARCSTATE %d" % marcstate, end='')

    # enable next line: "MemoryError: memory allocation failed"
    # geht, wenn Zeile 90: #gc.collect()
    #print(''.join(list(map(lambda x: "{0:0>8}".format(str(bin(x)[2:])), data))))

    print("Sending packet of", len(data), "bytes")

    writeBurst(TXFIFO, data)
    #time.sleep(0.002) # 0.002 = 2 ms - nicht größer machen!
    strobe(STX)

    remaining_bytes = readSingleByte(TXBYTES) & 0x7F
    while remaining_bytes != 0:
        #time.sleep(0.002) # 0.001 = 1 ms
        remaining_bytes = readSingleByte(TXBYTES) & 0x7F
        #print(" %d" % remaining_bytes, end='')

    if (readSingleByte(TXBYTES) & 0x7F) == 0:
        packet_nr += 1
        print("Packets sent:", packet_nr)
    else:
        print(readSingleByte(TXBYTES) & 0x7F)

    if packet_nr == 15: # bug: FIFO overflow?
        print("bug: FIFO overflow? -> re-init")
        strobe(SRES)
        initRegisters_TX()
        writeSingleByte(FREQ0, valFREQ0)
        packet_nr = 0

    led.value = True
    gc.collect()
    time.sleep(repeatAfter)

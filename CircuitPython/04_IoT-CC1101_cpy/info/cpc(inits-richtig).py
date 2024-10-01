class CC1101:
    def __init__(self, spi, cs, gdo0, baudrate, frequency, syncword, offset=0): #optional frequency offset in Hz
        self.gdo0 = gdo0
        self.device = SPIDevice(spi, cs, baudrate=baudrate, polarity=0, phase=0)
        self.strobe(SRES) # reset

        self.setFrequency(frequency, offset)

        assert len(syncword) == 4
        self.writeSingleByte(SYNC1, int(syncword[:2], 16))
        self.writeSingleByte(SYNC0, int(syncword[2:], 16))

        self.writeBurst(PATABLE, PA_TABLE)
        self.strobe(SFTX) # flush TX FIFO
        self.strobe(SFRX) # flush RX FIFO


    def setupCheck(self):
        self.strobe(SFRX)
        self.strobe(SRX)
        print("ready to detect data")

    def receiveData(self, length):
        self.writeSingleByte(PKTLEN, length)
        self.strobe(SRX)
        print("waiting for data")

        while self.gdo0.value == False:
            pass
        #detected rising edge

        while self.gdo0.value == True:
            pass
        #detected falling edge

        data_len = length#+2 # add 2 status bytes
        data = self.readBurst(RXFIFO, data_len)
        dataStr = ''.join(list(map(lambda x: "{0:0>8}".format(x[2:]), list(map(bin, data)))))
        newStr = dataStr[8:]
        print("Data: ", newStr)
        self.strobe(SIDLE)
        while (self.readSingleByte(MARCSTATE) != 0x01):
            pass
        self.strobe(SFRX)
        return newStr

    def sendData(self, bitstring, syncword):
        print("TXBYTES before sendData:", self.readSingleByte(TXBYTES))
        paddingLen = math.floor((512-16-len(bitstring))/8) # 16 Bits sync word
        bitstring = paddingLen*"10101010"+"{0:0>16}".format(bin(int(syncword, 16))[2:])+bitstring

        #print("the bitstring is", len(bitstring), "bits long")

        data = []
        for i in range(0,len(bitstring)/8):
            data.append(int(bitstring[i*8:i*8+8], 2))

        self.writeSingleByte(PKTLEN, len(data))

        self.strobe(SIDLE)
        while (self.readSingleByte(MARCSTATE) & 0x1F != 0x01): # wait for CC to enter idle state
            pass
        self.strobe(SFTX) # flush TX FIFO
        time.sleep(0.05)

        #print(''.join(list(map(lambda x: "{0:0>8}".format(str(bin(x)[2:])), data))))
        #print("Sending packet of", len(data), "bytes")
        #print("Data in TXFIFO:\n", self.readBurst(TXFIFO, 64), "\nTXBYTES:", self.readSingleByte(TXBYTES))
        self.writeBurst(TXFIFO, data)
        #print("Data in TXFIFO:\n", self.readBurst(TXFIFO, 64), "\nTXBYTES:", self.readSingleByte(TXBYTES))
        self.strobe(STX)

        remaining_bytes = self.readSingleByte(TXBYTES) & 0x7F
        while remaining_bytes != 0:
            time.sleep(0.1)
            print("Waiting until all bytes are transmited, remaining bytes: %d" % remaining_bytes)
            remaining_bytes = self.readSingleByte(TXBYTES) & 0x7F

        self.strobe(SFTX)
        self.strobe(SFRX)
        time.sleep(0.05)

        if (self.readSingleByte(TXBYTES) & 0x7F) == 0:
            print("Packet sent!\n\n")
            return True

        else:
            print(self.readSingleByte(TXBYTES) & 0x7F)
            return False

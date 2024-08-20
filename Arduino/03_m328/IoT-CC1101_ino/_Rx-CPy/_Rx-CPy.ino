//Serial.begin(115200);
//SS_PIN = 10;
#define GDO0_PIN 2
/// HW-adaption:
#define valFREQ0 0x2B // <- 434.4 (theory)
//#define valFREQ0 0x00 // SMA-Antenne (geht 17 kHz vor)
//#define valFREQ0 = 0x30 // angeloetete Antenne
//#define valFREQ0 = 0xA9 // noise ?

//#define New_Receive_minimal // ELECHOUSE

#define data_len 0x17 // PKTLEN is programmed to 0x17, add 2 status bytes

//https://github.com/LSatan/SmartRC-CC1101-Driver-Lib\examples\CC1101 default examples\New_Method_without_GDO\cc1101_New_Receive_method_minimal

#include "ELECHOUSE_CC1101_SRC_DRV.h"
#include "CC1101_common.h"

void initRegisters1(){
    ELECHOUSE_cc1101.SpiWriteReg(FIFOTHR, 0x47);
    ELECHOUSE_cc1101.SpiWriteReg(SYNC1, 0x66);
    ELECHOUSE_cc1101.SpiWriteReg(SYNC0, 0x6A);
    //ELECHOUSE_cc1101.SpiWriteReg(PKTLEN, 0x15); // RX
    //ELECHOUSE_cc1101.SpiWriteReg(PKTLEN, 0x0A); // TX
    ELECHOUSE_cc1101.SpiWriteReg(PKTCTRL1, 0x04);
    ELECHOUSE_cc1101.SpiWriteReg(PKTCTRL0, 0x04);
    ELECHOUSE_cc1101.SpiWriteReg(ADDR, 0x00);
    ELECHOUSE_cc1101.SpiWriteReg(CHANNR, 0x00);
    ELECHOUSE_cc1101.SpiWriteReg(FSCTRL1, 0x06);
    ELECHOUSE_cc1101.SpiWriteReg(FSCTRL0, 0x00);
    ELECHOUSE_cc1101.SpiWriteReg(FREQ2, 0x10);	  //. 0x10 <- 434.4 (theory); // 0x10 <- exactly on 434.4 (measured); .//
    ELECHOUSE_cc1101.SpiWriteReg(FREQ1, 0xB5);	  //. 0xB5 <- 434.4 (theory); // 0xB5 <- exactly on 434.4 (measured); .//
    // ELECHOUSE_cc1101.SpiWriteReg(FREQ0, 0xA9);	  //. 0x2B <- 434.4 (theory); // 0xA9 <- exactly on 434.4 (measured); .//
    ELECHOUSE_cc1101.SpiWriteReg(FREQ0, 0x30); // angeloetete Antenne
    // ELECHOUSE_cc1101.SpiWriteReg(FREQ0, 0x00); // SMA-Antenne (geht 17 kHz vor););
}

void initRegisters2(){
    ELECHOUSE_cc1101.SpiWriteReg(WOREVT1, 0x87);
    ELECHOUSE_cc1101.SpiWriteReg(WOREVT0, 0x6B);
    ELECHOUSE_cc1101.SpiWriteReg(WORCTRL, 0xFB);
    ELECHOUSE_cc1101.SpiWriteReg(FREND1, 0x56);
    ELECHOUSE_cc1101.SpiWriteReg(FREND0, 0x11);
    ELECHOUSE_cc1101.SpiWriteReg(FSCAL3, 0xE9);
    ELECHOUSE_cc1101.SpiWriteReg(FSCAL2, 0x2A);
    ELECHOUSE_cc1101.SpiWriteReg(FSCAL1, 0x00);
    ELECHOUSE_cc1101.SpiWriteReg(FSCAL0, 0x1F);
    ELECHOUSE_cc1101.SpiWriteReg(RCCTRL1, 0x41);
    ELECHOUSE_cc1101.SpiWriteReg(RCCTRL0, 0x00);
    ELECHOUSE_cc1101.SpiWriteReg(FSTEST, 0x59);
    ELECHOUSE_cc1101.SpiWriteReg(PTEST, 0x7F);
    ELECHOUSE_cc1101.SpiWriteReg(AGCTEST, 0x3F);
    ELECHOUSE_cc1101.SpiWriteReg(TEST2, 0x81);
    ELECHOUSE_cc1101.SpiWriteReg(TEST1, 0x35);
    //ELECHOUSE_cc1101.SpiWriteReg(TEST0, 0x09); // RX
    //ELECHOUSE_cc1101.SpiWriteReg(TEST0, 0x0B); // TX
}

void initRegisters_RX(){
    ELECHOUSE_cc1101.SpiWriteReg(IOCFG2, 0x29);
    ELECHOUSE_cc1101.SpiWriteReg(IOCFG1, 0x2E);
    ELECHOUSE_cc1101.SpiWriteReg(IOCFG0, 0x06);
    initRegisters1();
    ELECHOUSE_cc1101.SpiWriteReg(PKTLEN, 0x15);
    ELECHOUSE_cc1101.SpiWriteReg(MDMCFG4, 0x87);
    ELECHOUSE_cc1101.SpiWriteReg(MDMCFG3, 0x10);
    ELECHOUSE_cc1101.SpiWriteReg(MDMCFG2, 0x32);
    ELECHOUSE_cc1101.SpiWriteReg(MDMCFG1, 0x22);
    ELECHOUSE_cc1101.SpiWriteReg(MDMCFG0, 0xF8);
    ELECHOUSE_cc1101.SpiWriteReg(DEVIATN, 0x00);
    ELECHOUSE_cc1101.SpiWriteReg(MCSM2, 0x07);
    ELECHOUSE_cc1101.SpiWriteReg(MCSM1, 0x30);
    ELECHOUSE_cc1101.SpiWriteReg(MCSM0, 0x18);
    ELECHOUSE_cc1101.SpiWriteReg(FOCCFG, 0x16);
    ELECHOUSE_cc1101.SpiWriteReg(BSCFG, 0x6C);
    ELECHOUSE_cc1101.SpiWriteReg(AGCCTRL2, 0x04);
    ELECHOUSE_cc1101.SpiWriteReg(AGCCTRL1, 0x00);
    ELECHOUSE_cc1101.SpiWriteReg(AGCCTRL0, 0x91);
    initRegisters2();
    ELECHOUSE_cc1101.SpiWriteReg(TEST0, 0x09);
}

void gdo0wait(){
  while (!digitalRead(GDO0_PIN)); //detect rising edge
  while (digitalRead(GDO0_PIN)); //detect falling edge
/*
  while gdo0.value == False:
      pass
  #detected rising edge

  while gdo0.value == True:
      pass
  #detected falling edge
*/
}

void setup(){
  Serial.begin(115200);
  Serial.println(__FILE__);

  if (valFREQ0 == 0x00)
    Serial.println(" SMA antenna module configured");
  else
    Serial.println(" Soldered antenna module configured");

  ELECHOUSE_cc1101.setSpi(); //  SCK_PIN = 13; MISO_PIN = 12; MOSI_PIN = 11; SS_PIN = 10;
  if (ELECHOUSE_cc1101.SpiReadStatus(0x31)>0)
    Serial.println("Connection OK");
  else
    Serial.println("Connection Error");

  ELECHOUSE_cc1101.Init();
/*  ELECHOUSE_cc1101.setCCMode(1);       // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    Serial.println("Rx Mode 2-FSK");
    ELECHOUSE_cc1101.setMHZ(433.92);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    ELECHOUSE_cc1101.setCrc(1);      // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
*/

  initRegisters_RX(); // CPY-CC1101
  pinMode(GDO0_PIN, INPUT);

  ELECHOUSE_cc1101.SpiWriteReg(FREQ0, valFREQ0);
  ELECHOUSE_cc1101.SpiWriteReg(MDMCFG2, 0x32); // ASK =0x32: with 16/16 sync word; =0x30: no sync word
  Serial.println("Rx Mode ASK/OOK");
  ELECHOUSE_cc1101.SpiWriteReg(PKTLEN, data_len - 2); // 0x32: 0x15; 0x30: 0x17 geht nicht

  Serial.print("Chip version should be 0x14 (20): "); Serial.println(ELECHOUSE_cc1101.SpiReadReg(VERSION));
  Serial.print(" MDMCFG2: "); Serial.println(ELECHOUSE_cc1101.SpiReadReg(MDMCFG2));
}
byte buffer[61] = {0};

void loop(){
#ifndef New_Receive_minimal // CPY-CC1101
  ELECHOUSE_cc1101.SpiStrobe(CC1101_SRX);
  Serial.println("waiting for data");
  gdo0wait();
  Serial.print("Rssi: ");  Serial.println(ELECHOUSE_cc1101.getRssi());
/*
  data = readBurst(RXFIFO, data_len)
    ->
def readBurst(start_address, length):
    databuffer = []
    ret = bytearray(length+1)
    for x in range(length + 1):
        addr = (start_address + (x*8)) | READ_BURST
        databuffer.append(addr)
    # NOK: device.write_readinto(bytearray(databuffer), ret)
    # OK:
    with device as d:
      d.write_readinto(bytearray(databuffer), ret)
    return ret

"write_readinto" = SPI


*/
#ifdef New_Receive_minimal
    //Checks whether something has been received.
    //When something is received we give some time to receive the message in full.(time in millis)
    if (ELECHOUSE_cc1101.CheckRxFifo(100)){
    Serial.print(".");

    if (ELECHOUSE_cc1101.CheckCRC()){    //CRC Check. If "setCrc(false)" crc returns always OK!
    Serial.print("Rssi: ");
    Serial.println(ELECHOUSE_cc1101.getRssi());
    Serial.print("LQI: ");
    Serial.println(ELECHOUSE_cc1101.getLqi());

    int len = ELECHOUSE_cc1101.ReceiveData(buffer);
    buffer[len] = '\0';
    Serial.println((char *) buffer);
    for (int i = 0; i<len; i++){
    Serial.print(buffer[i]);
    Serial.print(",");
    }
    Serial.println();
    }
    }
#endif
}

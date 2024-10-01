#define WRITE_SINGLE_BYTE  0x00
#define WRITE_BURST  0x40
#define READ_SINGLE_BYTE  0x80
#define READ_BURST  0xC0

#define IOCFG2  0x00  // GDO2 Output Pin Configuration
#define IOCFG1  0x01  // GDO1 Output Pin Configuration
#define IOCFG0  0x02  // GDO0 Output Pin Configuration
#define FIFOTHR  0x03  // RX FIFO and TX FIFO Thresholds
#define SYNC1  0x04  // Sync Word, High Byte
#define SYNC0  0x05  // Sync Word, Low Byte
#define PKTLEN  0x06  // Packet Length
#define PKTCTRL1  0x07  // Packet Automation Control
#define PKTCTRL0  0x08  // Packet Automation Control
#define ADDR  0x09  // Device Address
#define CHANNR  0x0A  // Channel Number
#define FSCTRL1  0x0B  // Frequency Synthesizer Control
#define FSCTRL0  0x0C  // Frequency Synthesizer Control
#define FREQ2  0x0D  // Frequency Control Word, High Byte
#define FREQ1  0x0E  // Frequency Control Word, Middle Byte
#define FREQ0  0x0F  // Frequency Control Word, Low Byte
#define MDMCFG4  0x10  // Modem Configuration
#define MDMCFG3  0x11  // Modem Configuration
#define MDMCFG2  0x12  // Modem Configuration
#define MDMCFG1  0x13  // Modem Configuration
#define MDMCFG0  0x14  // Modem Configuration
#define DEVIATN  0x15  // Modem Deviation Setting
#define MCSM2  0x16  // Main Radio Control State Machine Configuration
#define MCSM1  0x17  // Main Radio Control State Machine Configuration
#define MCSM0  0x18  // Main Radio Control State Machine Configuration
#define FOCCFG  0x19  // Frequency Offset Compensation Configuration
#define BSCFG  0x1A  // Bit Synchronization Configuration
#define AGCCTRL2  0x1B  // AGC Control
#define AGCCTRL1  0x1C  // AGC Control
#define AGCCTRL0  0x1D  // AGC Control
#define WOREVT1  0x1E  // High Byte Event0 Timeout
#define WOREVT0  0x1F  // Low Byte Event0 Timeout
#define WORCTRL  0x20  // Wake On Radio Control
#define FREND1  0x21  // Front End RX Configuration
#define FREND0  0x22  // Front End TX Configuration
#define FSCAL3  0x23  // Frequency Synthesizer Calibration
#define FSCAL2  0x24  // Frequency Synthesizer Calibration
#define FSCAL1  0x25  // Frequency Synthesizer Calibration
#define FSCAL0  0x26  // Frequency Synthesizer Calibration
#define RCCTRL1  0x27  // RC Oscillator Configuration
#define RCCTRL0  0x28  // RC Oscillator Configuration

//// Configuration Register Details - Registers that Loose Programming in SLEEP State

#define FSTEST  0x29  // Frequency Synthesizer Calibration Control
#define PTEST  0x2A  // Production Test
#define AGCTEST  0x2B  // AGC Test
#define TEST2  0x2C  // Various Test Settings
#define TEST1  0x2D  // Various Test Settings
#define TEST0  0x2E  // Various Test Settings

//// Command Strobe Registers

#define SRES  0x30  // Reset chip
#define SFSTXON  0x31  // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL1).
//// If in RX (with CCA): Go to a wait state where only the synthesizer
//// is running (for quick RX / TX turnaround).

#define SXOFF  0x32  // Turn off crystal oscillator.
#define SCAL  0x33  // Calibrate frequency synthesizer and turn it off.
//// SCAL can be strobed from IDLE mode without setting manual calibration mode.

#define SRX  0x34  // Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL1.
#define STX  0x35  // In IDLE state: Enable TX. Perform calibration first
//// if MCSM0.FS_AUTOCAL1.
//// If in RX state and CCA is enabled: Only go to TX if channel is clear.

#define SIDLE  0x36  // Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable.
#define SWOR  0x38  // Start automatic RX polling sequence (Wake-on-Radio)
//// as described in Section 19.5 if WORCTRL.RC_PD0.

#define SPWD  0x39  // Enter power down mode when CSn goes high.
#define SFRX  0x3A  // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states.
#define SFTX  0x3B  // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states.
#define SWORRST  0x3C  // Reset real time clock to Event1 value.
#define SNOP  0x3D  // No operation. May be used to get access to the chip status byte.

#define PATABLE  0x3E  // PATABLE
#define TXFIFO  0x3F  // TXFIFO
#define RXFIFO  0x3F  // RXFIFO

//// Status Register Details

#define PARTNUM  0xF0  // Chip ID
#define VERSION  0xF1  // Chip ID
#define FREQEST  0xF2  // Frequency Offset Estimate from Demodulator
#define LQI  0xF3  // Demodulator Estimate for Link Quality
#define RSSI  0xF4  // Received Signal Strength Indication
#define MARCSTATE  0xF5  // Main Radio Control State Machine State
#define WORTIME1  0xF6  // High Byte of WOR Time
#define WORTIME0  0xF7  // Low Byte of WOR Time
#define PKTSTATUS  0xF8  // Current GDOx Status and Packet Status
#define VCO_VC_DAC  0xF9  // Current Setting from PLL Calibration Module
#define TXBYTES  0xFA  // Underflow and Number of Bytes
#define RXBYTES  0xFB  // Overflow and Number of Bytes
#define RCCTRL1_STATUS  0xFC  // Last RC Oscillator Calibration Result
#define RCCTRL0_STATUS  0xFD  // Last RC Oscillator Calibration Result

//PA_TABLE  [0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00]

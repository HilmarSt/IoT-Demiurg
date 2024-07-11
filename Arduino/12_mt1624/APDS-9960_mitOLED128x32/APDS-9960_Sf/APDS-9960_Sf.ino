//Serial.begin(115200);
// Board: megaTinyCore
// Chip: ATtiny1624
// Clock: 20 MHz internal
// Programmer: SerialUPDI - 230400 (CH340: 460800)

#include "SparkFun_APDS9960.h" // #define DEBUG 1 -> viel Output!
#define GESTURE_GAIN GGAIN_1X // 1X/2X: Unterschiede bei FAR/NEAR, 4X = viel NONE, 8X geht nicht
/* ATtiny1624/1626/1627 - tinyAVR® 2 Family
  All pins can be used for external interrupt where pins Px2 and Px6 of each port have full asynchronous detection. */
#define APDS9960_INT PIN_PA4 // Needs to be an interrupt pin (geht auch mit offener Leitung durch durch Einstreuung von I2C!)

#include <Wire.h>
SparkFun_APDS9960 apds = SparkFun_APDS9960();
volatile int isr_flag = 0;

#define LED LED_BUILTIN // t1624 = PIN_PA7

#define OLED
#ifdef OLED  // Pull-up's ?
#include "OLED\ssd1306.cpp" // in .h: #define SCREEN_128X32
SSD1306 oled;
#endif

//#define ScanI2C
#ifdef ScanI2C
#include "I2C_Scan.c"
#endif


void setup() {
  pinMode(LED, OUTPUT);  digitalWrite(LED, HIGH);  delay(200);  digitalWrite(LED, LOW);

  Serial.begin(115200);
  Serial.println("OLED128x32.ino");

  #ifdef OLED
  Wire.begin();  oled.begin();
  oled.print("APDS-9960_Sf.ino 89012345-"); 
  oled.set_pos(0,1*FONT_HEIGHT);
  oled.print("                          ");
  oled.set_pos(0,2*FONT_HEIGHT);
  oled.print("                          ");
  //oled.set_pos(0,3*FONT_HEIGHT);
  //oled.print("                          ");
  #endif

  // Set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);

  // Initialize interrupt service routine
  attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    #ifdef USE_SERIAL
    Serial.println(F("APDS-9960 initialization complete"));
    #endif
  } else {
    #ifdef USE_SERIAL
    Serial.println(F("Something went wrong during APDS-9960 init!"));
    #endif
  }
  apds.setGestureGain(GESTURE_GAIN); // ohne diese Zeile geht es nicht zuverlässig #Zeile 34

  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) { // (bool interrupts)
    #ifdef USE_SERIAL
    Serial.println(F("Gesture sensor is now running"));
    #endif
  } else {
    #ifdef USE_SERIAL
    Serial.println(F("Something went wrong during gesture sensor init!"));
    #endif
  }
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    digitalWrite(LED, HIGH);  delay(10);  digitalWrite(LED, LOW);
    int Gesture = apds.readGesture();
    #ifdef USE_SERIAL
    switch (Gesture) {
      case DIR_UP:
        Serial.println("UP");
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        break;
      case DIR_FAR:
        Serial.println("FAR");
        break;
      default:
        Serial.println("NONE");
    }
  #endif
  #ifdef OLED
  oled.set_pos(0,0);  oled.print("                         -");
  oled.set_pos(0,1*FONT_HEIGHT);
    switch (Gesture) {
      case DIR_UP:
        oled.print(" UP    ");
        break;
      case DIR_DOWN:
        oled.print(" DOWN ");
        break;
      case DIR_LEFT:
        oled.print(" LEFT ");
        break;
      case DIR_RIGHT:
        oled.print(" RIGHT");
        break;
      case DIR_NEAR:
        oled.print(" NEAR ");
        break;
      case DIR_FAR:
        oled.print(" FAR  ");
        break;
      default:
        oled.print(" NONE ");
    }
  #endif
  }
}


void loop() {
  if( isr_flag == 1 ) {
    detachInterrupt(digitalPinToInterrupt(APDS9960_INT));
    handleGesture();
    isr_flag = 0;
    attachInterrupt(digitalPinToInterrupt(APDS9960_INT), interruptRoutine, FALLING);
  }
  digitalWrite(LED, HIGH); digitalWrite(LED, LOW);

  #ifdef ScanI2C
    I2C_Scan();
  #endif
}

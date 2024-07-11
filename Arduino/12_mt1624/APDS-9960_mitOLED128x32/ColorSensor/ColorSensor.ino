//Serial.begin(115200);
// Board: megaTinyCore
// Chip: ATtiny1624
// Clock: 20 MHz internal
// Programmer: SerialUPDI - 230400 (CH340: 460800)

//https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor
#include "SparkFun_APDS9960.h" // #define DEBUG 1 -> viel Output!
#include <Wire.h>
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;

#define LED LED_BUILTIN // t1624 = PIN_PA7

#define OLED
#ifdef OLED  // Pull-up's ?
#include "OLED\ssd1306.cpp" // in .h: #define SCREEN_128X32
SSD1306 oled;
#endif


void setup() {
  pinMode(LED, OUTPUT);  digitalWrite(LED, HIGH);  delay(200);  digitalWrite(LED, LOW);

  Serial.begin(115200);
  Serial.println("ColorSensor.ino");

  #ifdef OLED
  Wire.begin();  oled.begin();
  oled.print("ColorSensor.ino  89012345-"); 
  oled.set_pos(0,1*FONT_HEIGHT);
  oled.print("                          ");
  oled.set_pos(0,2*FONT_HEIGHT);
  oled.print("                          ");
  //oled.set_pos(0,3*FONT_HEIGHT);
  //oled.print("                          ");
  #endif

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }
  
  // Wait for initialization and calibration to finish
  delay(500);

  oled_clear();
}


void oled_clear() { // OLED 128x32 :
for (uint8_t i=0; i<4; i++ ) { // 4 Zeilen
oled.set_pos(0,i*FONT_HEIGHT);
  for (uint8_t j=0; j<13; j++ ) { // 25 Zeichen (+1 Rest) -> es bleiben trotzdem noch Pixel in den Zeilen 2..4
    oled.print("  ");
  }
}}

void loop() {
  // Read the light levels (ambient, red, green, blue)
  if (  !apds.readAmbientLight(ambient_light) ||
        !apds.readRedLight(red_light) ||
        !apds.readGreenLight(green_light) ||
        !apds.readBlueLight(blue_light) ) {
    Serial.println("Error reading light values");
  } else {
    Serial.print("Ambient: ");    Serial.print(ambient_light);
    Serial.print(" Red: ");    Serial.print(red_light);
    Serial.print(" Green: ");    Serial.print(green_light);
    Serial.print(" Blue: ");    Serial.println(blue_light);

  #ifdef OLED
    #define SCALE 38 // max. 37889 !? (Taschenlampe)
  oled.set_pos(0,0);
  if (ambient_light > 999) { ambient_light /= 38; oled.print("*"); }
    else oled.print(" ");
  oled.set_pos(0,1*FONT_HEIGHT);
  if (red_light > 999) { red_light /= 38; oled.print("*"); }
    else oled.print(" ");
  oled.set_pos(0,2*FONT_HEIGHT);
  if (green_light > 999) { green_light /= 38; oled.print("*"); }
    else oled.print(" ");
  oled.set_pos(0,3*FONT_HEIGHT);
  if (blue_light > 999) { blue_light /= 38; oled.print("*"); }
    else oled.print(" ");

  // ein Zeichen ist 7+1 Pixel breit
  oled.set_pos(64,0);             oled.print("Ambient:     ");
  oled.set_pos(128-16,0*FONT_HEIGHT);  oled.print(ambient_light);
  oled.set_pos(8,1*FONT_HEIGHT);  oled.print(" Red:     ");
  oled.set_pos(42,1*FONT_HEIGHT);  oled.print(red_light);
  oled.set_pos(8,2*FONT_HEIGHT);  oled.print(" Green:   ");
  oled.set_pos(42,2*FONT_HEIGHT);  oled.print(green_light);
  oled.set_pos(8,3*FONT_HEIGHT);  oled.print(" Blue:    ");
  oled.set_pos(42,3*FONT_HEIGHT);  oled.print(blue_light);
  #endif

  }

  // Wait 1 second before next reading
  delay(1000);
}

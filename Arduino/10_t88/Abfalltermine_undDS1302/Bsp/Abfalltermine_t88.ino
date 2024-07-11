// https://github.com/SpenceKonde/ATTinyCore
// Chip: ATtiny88
// Clock Source: 16 MHz external (Overclocked)
// Pin Mapping: "MH Tiny"

//#define USE_SERIAL //Serial.begin(115200);
#define DS_POWER_PIN 10 // Spannungsversorgung Platine mit DS1307
#define LED_green 9 // t88: HW-PWM auf den Pins 9 und 10 
//#define SET_TIME // write Compile Time + 1 day to DS1307 -> // am Minutenanfang kompilieren (wg. +10 sec.)
  #ifdef SET_TIME
  #include "SetTime.c" // von 1_SetTime.ino
  #endif
//#define ReadTest_2 // Debug-Ausgabe DS1307 auf Serial
//#define ScanI2C
#ifdef ScanI2C
  #include "I2C_Scan.c"
  // I2C device found at address 0x50 = EEPROM
  // I2C device found at address 0x68 = DS1307
  #endif
#define USE_LCD
  #ifdef USE_LCD
  #include <LiquidCrystal.h>
  const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
  char str[9];
  static uint8_t errcnt = 0;
  #endif

#include "Array_Abfalltermine.h"
  static uint8_t BioM, BioT, BlauM, BlauT, GelbM, GelbT, RestM, RestT = 0; // Monat, Tag
#include "TimeLib.h"
  tmElements_t tm;
#include "DS1307RTC.h"
  #include <Wire.h>
// #include "LowPower.h" // github.com/rocketscream/Low-Power - unterstützt kein tiny88

#ifdef ReadTest_2
void print2digits(int number) {
  if (number >= 0 && number < 10) Serial.write('0');
  Serial.print(number);
}
#endif
void ReadTest() {
#ifdef ReadTest_2
    Serial.print("DS1307: Time = ");
    print2digits(tm.Hour);    Serial.write(':');
    print2digits(tm.Minute);    Serial.write(':');
    print2digits(tm.Second);    Serial.print(" Date = ");
    Serial.print(tm.Day);    Serial.write('.');
    Serial.print(tm.Month);    Serial.write('.');
//    Serial.println(tmYearToCalendar(tm.Year));
    Serial.println(tm.Year - 30);
#endif
}


void read_Arrays() {
  uint8_t i,x;  uint8_t Tag = tm.Day;  bool found = false;

  for(x = 0; x < 2; x++) { if (x) Tag = 1; // Suche über 2 Monate
    for(i=0; i < Bio_Termine; i++ ) {
      volatile uint8_t vBioM = pgm_read_byte_near(&(Bio[i][0])), vBioT = pgm_read_byte_near(&(Bio[i][1]));
      if ((vBioM  == (tm.Month + x)) && (vBioT >= Tag)) { BioM  =  vBioM;  BioT  =  vBioT;  found = true;  break; }
    if (found) break; }
  }  Tag = tm.Day;  found = false;

  for(x = 0; x < 2; x++) { if (x) Tag = 1; // Suche über 2 Monate
    for(i=0; i < Blau_Termine; i++ ) {
      volatile uint8_t vBlauM = pgm_read_byte_near(&(Blau[i][0])), vBlauT = pgm_read_byte_near(&(Blau[i][1]));
      if ((vBlauM == (tm.Month + x)) && (vBlauT >= Tag)) { BlauM = vBlauM;  BlauT = vBlauT;  found = true;  break; }
    if (found) break; }
  }  Tag = tm.Day;  found = false;

  for(x = 0; x < 2; x++) { if (x) Tag = 1; // Suche über 2 Monate
    for(i=0; i < Gelb_Termine; i++ ) {
      volatile uint8_t vGelbM = pgm_read_byte_near(&(Gelb[i][0])), vGelbT = pgm_read_byte_near(&(Gelb[i][1]));
      if ((vGelbM == (tm.Month + x)) && (vGelbT >= Tag)) { GelbM = vGelbM;  GelbT = vGelbT;  found = true;  break; }
    if (found) break;}
  }  Tag = tm.Day;  found = false;

  for(x = 0; x < 2; x++) { if (x) Tag = 1; // Suche über 2 Monate
    for(i=0; i < Schwarz_Termine; i++ ) {
      volatile uint8_t vRestM = pgm_read_byte_near(&(Schwarz[i][0])), vRestT = pgm_read_byte_near(&(Schwarz[i][1]));
      if ((vRestM == (tm.Month + x)) && (vRestT >= Tag)) { RestM = vRestM;  RestT = vRestT;  found = true;  break; }
    if (found) break;}
  }

}

void DS_retry() { // Quarz läuft immer (mit eigenem Akku)
  lcd.setCursor(0, 1);  lcd.print("DS_retry");
  pinMode(DS_POWER_PIN, OUTPUT);
  digitalWrite(DS_POWER_PIN, LOW);  delay(2000);
  digitalWrite(DS_POWER_PIN, HIGH);  delay(1000);
}

void setup(void)
{
  pinMode(DS_POWER_PIN, OUTPUT);  digitalWrite(DS_POWER_PIN, HIGH);

  #ifdef USE_SERIAL
  Serial.begin(115200);  Serial.println("Abfalltermine");
  #endif

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_green, OUTPUT);

  #ifdef USE_LCD
  lcd.begin(8, 2);  // set up the LCD's number of columns and rows:
  lcd.print("Abfall-T");
  digitalWrite(LED_BUILTIN, HIGH);  delay(500);  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_green  , HIGH);  delay(500);  digitalWrite(LED_green  , LOW);
  delay(1000); // für DS ?
  lcd.clear();
  #endif

#ifdef SET_TIME
  write_compile_time_to_DS();
#endif

#ifdef ScanI2C
  I2C_Scan();
#endif
}


void loop(void)
{  uint8_t Anz = 0;
if (!RTC.read(tm)) DS_retry(); // Quarz läuft immer (mit Akku)
if (RTC.read(tm))
{
  ReadTest();
  read_Arrays();

  #ifdef USE_SERIAL
  Serial.print("Bio    "); Serial.print(BioT); Serial.write('.'); Serial.print(BioM); Serial.println(".");
// -> (ohne LCD:) Sketch uses 5522 bytes (34%) of program storage space. Global variables use 606 bytes (59%) of dynamic memory
//    String S_B = "Bio: ";  S_B += BioT;  S_B += "."; S_B += BioM; S_B += ".";  Serial.println(S_B);
// -> (ohne LCD:) Sketch uses 7162 bytes (45%) of program storage space. Global variables use 616 bytes (60%) of dynamic memory -> +1.5 kB !
  Serial.print("Blau   "); Serial.print(BlauT); Serial.write('.'); Serial.print(BlauM); Serial.println(".");
  Serial.print("Gelb   "); Serial.print(GelbT); Serial.write('.'); Serial.print(GelbM); Serial.println(".");
  Serial.print("Schwrz "); Serial.print(RestT); Serial.write('.'); Serial.print(RestM); Serial.println(".");
  Serial.println();
  #endif

  #ifdef USE_LCD
  lcd.setCursor(0, 0);
  if ((BioT  == tm.Day) && (BioM  == tm.Month)) {
    lcd.print("Bio  ");  if (tm.Day<10)lcd.write(' ');  lcd.print(tm.Day);  lcd.write('.');  lcd.setCursor(0, 1);  Anz++; }
  if ((BlauT == tm.Day) && (BlauM == tm.Month)) {
    lcd.print("Blau ");  if (tm.Day<10)lcd.write(' ');  lcd.print(tm.Day);  lcd.write('.');  lcd.setCursor(0, 1);  Anz++; }
  if ((GelbT == tm.Day) && (GelbM == tm.Month)) {
    lcd.print("Gelb ");  if (tm.Day<10)lcd.write(' ');  lcd.print(tm.Day);  lcd.write('.');  lcd.setCursor(0, 1);  Anz++; }
  if ((RestT == tm.Day) && (RestM == tm.Month)) {
    lcd.print("Rest ");  if (tm.Day<10)lcd.write(' ');  lcd.print(tm.Day);  lcd.write('.');                        Anz++; }
  if (!Anz) { // aktuell keine Leerung
    uint16_t naechste[4]; // zur Datumssortierung
    naechste[0] = BioM  * 32 + BioT;
    naechste[1] = BlauM * 32 + BlauT;
    naechste[2] = GelbM * 32 + GelbT;
    naechste[3] = RestM * 32 + RestT;
    uint16_t                  next = naechste[0];  lcd.setCursor(0, 0);  lcd.print("n.: Bio ");
    if (naechste[1] < next) { next = naechste[1];  lcd.setCursor(0, 0);  lcd.print("n.: Blau"); }
    if (naechste[2] < next) { next = naechste[2];  lcd.setCursor(0, 0);  lcd.print("n.: Gelb"); }
    if (naechste[3] < next) { next = naechste[3];  lcd.setCursor(0, 0);  lcd.print("n.: Rest"); }
    lcd.setCursor(0, 1); // 2. Zeile
    delay(60000);  // um evtl. DS_retry noch zu sehen
    if ((next == naechste[1]) && (naechste[1] == naechste[2])) // Blau und Gelb zusammen
      lcd.print("  + Gelb");
    else
      lcd.print("        ");
  }
  #endif

#ifdef HEUTE
  lcd.setCursor(0, 0);
  sprintf(str, "%d", tm.Hour);    if (tm.Hour <10) lcd.write(' ');  lcd.print(str);
  lcd.write(':');
  sprintf(str, "%d", tm.Minute);  if (tm.Minute <10) lcd.write('0');  lcd.print(str);
#else
//  lcd.setCursor(0, 0);
//  lcd.print("morgen:");
//  lcd.setCursor(0, 1);
//  sprintf(str, "%d.%d.%d", tm.Day, tm.Month, tm.Year - 30);  lcd.print(str);
#endif

} else // !RTC.read()
{
  if (RTC.chipPresent()) {
  #ifdef USE_SERIAL
    Serial.println("The DS1307 is stopped. Run 1_SetTime to initialize the time.\n");
  #endif
  } else {
  #ifdef USE_SERIAL
    Serial.println("DS1307 read error! Please check the circuitry.\n");
  #endif
    #ifdef USE_LCD
      errcnt++;
      lcd.setCursor(0, 1);  lcd.print("DSerr");  lcd.print(errcnt);
    #endif
  }
}

//  pinMode(DS_POWER_PIN, INPUT); //  digitalWrite(DS_POWER_PIN, LOW);
if (!Anz) {
  //analogWrite(LED_green,128);  delay(500);
  //analogWrite(LED_green,64);  delay(500);
  analogWrite(LED_green,32);  delay(200);
  analogWrite(LED_green,16);  delay(200);
  analogWrite(LED_green, 8);  delay(200);
  analogWrite(LED_green, 4);  delay(200);
  analogWrite(LED_green, 2);  delay(200);
  analogWrite(LED_green, 1);
} else {
  analogWrite(LED_green,255);  delay(1000);
}
  delay(60000-1050);
//  pinMode(DS_POWER_PIN, OUTPUT);  digitalWrite(DS_POWER_PIN, HIGH);  delay(4900);
  digitalWrite(LED_BUILTIN, HIGH);  delay(50);  digitalWrite(LED_BUILTIN, LOW);
}

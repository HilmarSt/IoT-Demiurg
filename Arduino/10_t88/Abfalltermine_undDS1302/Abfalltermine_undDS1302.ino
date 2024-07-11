// #define SET_TIME // zum Uhrzeitsetzen (DS1302 geht ~1 Minute pro Woche vor)
// SoftSerial: 9600
// Programmer: USBtinyISP FAST
// Pin Mapping: MH Tiny

#define SLEEP_TIME 60000 // 60000 z.Zt. delay()
#define LED_green 9 // t88: HW-PWM auf den Pins 9 und 10 
#include <Arduino.h> // für PINs

#include "Array_Abfalltermine2024.h"
  static uint8_t BioM, BioT, BlauM, BlauT, GelbM, GelbT, RestM, RestT = 0; // Monat, Tag

// #define USE_SERIAL
  #ifdef USE_SERIAL
  #include <SoftwareSerial.h>
  SoftwareSerial SoftSer(16,15); // RX,TX
  #endif

#define USE_LCD
  #ifdef USE_LCD
  #include <LiquidCrystal.h>
  const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
  char str[9];
  static uint8_t errcnt = 0;
  #endif

// DS1302:
#define delay_us 10
#include "libDS1302/Ds1302.cpp"
                  // nicht: A0, A1, A2 -  oder doch: 19, 20, 21
#define PIN_ENA 0 // RST, rote LED
#define PIN_DAT 1
#define PIN_CLK 2

Ds1302 rtc(PIN_ENA, PIN_CLK, PIN_DAT); // DS1302 RTC instance
Ds1302::DateTime tm;

void setup()
{
  #ifdef USE_SERIAL
    SoftSer.begin(9600);  delay(500);
    SoftSer.println(" # Abfalltermine_undDS1302.ino");
  #endif

  #ifdef USE_LCD
    lcd.begin(8, 2);  // set up the LCD's number of columns and rows:
    lcd.print("Abfall-T");
    digitalWrite(LED_BUILTIN, HIGH);  delay(500);  digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_green  , HIGH);  delay(500);  digitalWrite(LED_green  , LOW);
    delay(1000);
    lcd.clear();
  #endif

    // initialize the RTC
    rtc.init();
    rtc.getDateTime(&tm);

  #ifdef USE_SERIAL
    SerPrintTime();
  #endif

    if (rtc.isHalted() || !tm.month) {
    if (!tm.month) {
      #ifdef USE_SERIAL
        SoftSer.println("invalid time");
      #endif
      #ifdef USE_LCD
        lcd.setCursor(0, 0);  lcd.print("RTC NOK");
      #endif
    } else {
      #ifdef USE_SERIAL
        SoftSer.println("RTC is halted: enable SET_TIME");
      #endif
      #ifdef USE_LCD
        lcd.setCursor(0, 0);  lcd.print("RTC halt");
        lcd.setCursor(0, 1);  lcd.print("SET_TIME");
      #endif
      }
        delay(1000);
        while(1); ///////// stop here
    }

  #ifdef SET_TIME
      tm.year   = 24;
      tm.month  = 2;
      tm.dow    = 6; // 1=Mo
      tm.day    = 17 +1;      // heute ist morgen - wg. Hinweis Mülltonne einen Tag vorher
      tm.hour   = 13;
      tm.minute = 14 -1; // geht ~1 Minute pro Woche vor
      tm.second = 20; // Kompilier- und Hochladzeit

    rtc.setDateTime(&tm);

    #ifdef USE_SERIAL
      SoftSer.println("SET_TIME enabled: Time is set.");
    #endif
  #endif // SET_TIME

} // setup()


void SerPrintTime() { // ~ 400 Bytes
  #ifdef USE_SERIAL
        SoftSer.print("20");
        SoftSer.print(tm.year);    // 00-99
        SoftSer.print('-');
        if (tm.month < 10) SoftSer.print('0');
        SoftSer.print(tm.month);   // 01-12
        SoftSer.print('-');
        if (tm.day < 10) SoftSer.print('0');
        SoftSer.print(tm.day);     // 01-31
        SoftSer.print(' ');
        //SoftSer.print(WeekDays[tm.dow - 1]); // 1-7
        SoftSer.print(' ');
        if (tm.hour < 10) SoftSer.print('0');
        SoftSer.print(tm.hour);    // 00-23
        SoftSer.print(':');
        if (tm.minute < 10) SoftSer.print('0');
        SoftSer.print(tm.minute);  // 00-59
        SoftSer.print(':');
        if (tm.second < 10) SoftSer.print('0');
        SoftSer.print(tm.second);  // 00-59
        SoftSer.println();
  #endif
    }

void read_Arrays() {
  uint8_t i,x;  uint8_t Tag = tm.day;  bool found = false;

  for(x = 0; x < 2; x++) { if (x) Tag = 1; // Suche über 2 Monate
    for(i=0; i < Bio_Termine; i++ ) {
      volatile uint8_t vBioM = pgm_read_byte_near(&(Bio[i][0])), vBioT = pgm_read_byte_near(&(Bio[i][1]));
      if ((vBioM  == (tm.month + x)) && (vBioT >= Tag)) { BioM  =  vBioM;  BioT  =  vBioT;  found = true;  break; }
    if (found) break; }
  }  Tag = tm.day;  found = false;

  for(x = 0; x < 2; x++) { if (x) Tag = 1; // Suche über 2 Monate
    for(i=0; i < Blau_Termine; i++ ) {
      volatile uint8_t vBlauM = pgm_read_byte_near(&(Blau[i][0])), vBlauT = pgm_read_byte_near(&(Blau[i][1]));
      if ((vBlauM == (tm.month + x)) && (vBlauT >= Tag)) { BlauM = vBlauM;  BlauT = vBlauT;  found = true;  break; }
    if (found) break; }
  }  Tag = tm.day;  found = false;

  for(x = 0; x < 2; x++) { if (x) Tag = 1; // Suche über 2 Monate
    for(i=0; i < Gelb_Termine; i++ ) {
      volatile uint8_t vGelbM = pgm_read_byte_near(&(Gelb[i][0])), vGelbT = pgm_read_byte_near(&(Gelb[i][1]));
      if ((vGelbM == (tm.month + x)) && (vGelbT >= Tag)) { GelbM = vGelbM;  GelbT = vGelbT;  found = true;  break; }
    if (found) break;}
  }  Tag = tm.day;  found = false;

  for(x = 0; x < 2; x++) { if (x) Tag = 1; // Suche über 2 Monate
    for(i=0; i < Schwarz_Termine; i++ ) {
      volatile uint8_t vRestM = pgm_read_byte_near(&(Schwarz[i][0])), vRestT = pgm_read_byte_near(&(Schwarz[i][1]));
      if ((vRestM == (tm.month + x)) && (vRestT >= Tag)) { RestM = vRestM;  RestT = vRestT;  found = true;  break; }
    if (found) break;}
  }

}


void loop()
{  uint8_t Anz = 0;
  rtc.getDateTime(&tm);
  read_Arrays();

  #ifdef USE_SERIAL
    SoftSer.print("Bio    "); SoftSer.print(BioT); SoftSer.write('.'); SoftSer.print(BioM); SoftSer.println(".");
  //    Sketch uses 5522 bytes (34%) of program storage space. Global variables use 606 bytes (59%) of dynamic memory
  // mit: String S_B = "Bio: ";  S_B += BioT;  S_B += "."; S_B += BioM; S_B += ".";  SoftSer.println(S_B);
  // -> Sketch uses 7162 bytes (45%) of program storage space. Global variables use 616 bytes (60%) of dynamic memory -> +1.5 kB !
    SoftSer.print("Blau   "); SoftSer.print(BlauT); SoftSer.write('.'); SoftSer.print(BlauM); SoftSer.println(".");
    SoftSer.print("Gelb   "); SoftSer.print(GelbT); SoftSer.write('.'); SoftSer.print(GelbM); SoftSer.println(".");
    SoftSer.print("Schwrz "); SoftSer.print(RestT); SoftSer.write('.'); SoftSer.print(RestM); SoftSer.println(".");
    SoftSer.println();
  #endif

  #ifdef USE_LCD
  lcd.setCursor(0, 0);
  if ((BioT  == tm.day) && (BioM  == tm.month)) {
    lcd.print("Bio  ");  if (tm.day<10)lcd.write(' ');  lcd.print(tm.day);  lcd.write('.');  lcd.setCursor(0, 1);  Anz++; }
  if ((BlauT == tm.day) && (BlauM == tm.month)) {
    lcd.print("Blau ");  if (tm.day<10)lcd.write(' ');  lcd.print(tm.day);  lcd.write('.');  lcd.setCursor(0, 1);  Anz++; }
  if ((GelbT == tm.day) && (GelbM == tm.month)) {
    lcd.print("Gelb ");  if (tm.day<10)lcd.write(' ');  lcd.print(tm.day);  lcd.write('.');  lcd.setCursor(0, 1);  Anz++; }
  if ((RestT == tm.day) && (RestM == tm.month)) {
    lcd.print("Rest ");  if (tm.day<10)lcd.write(' ');  lcd.print(tm.day);  lcd.write('.');                        Anz++; }
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
    //delay(60000);  // um evtl. DS_retry noch zu sehen
    if ((next == naechste[1]) && (naechste[1] == naechste[2])) // Blau und Gelb zusammen
      lcd.print("  + Gelb");
    else
      lcd.print("        ");
  }
  #endif

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
    analogWrite(LED_green,128);  delay(1000);
  }
    delay(SLEEP_TIME-1050);
  //  pinMode(DS_POWER_PIN, OUTPUT);  digitalWrite(DS_POWER_PIN, HIGH);  delay(4900);
    digitalWrite(LED_BUILTIN, HIGH);  delay(50);  digitalWrite(LED_BUILTIN, LOW);
}

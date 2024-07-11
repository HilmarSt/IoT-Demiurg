// wetzlar.de : Suche: Abfall -> Abfall- und Umweltkalender 224 -> https://www.wetzlar.de/leben-in-wetzlar/abfall-und-entsorgung/abfuhrtermine-224.php
// ...Naunheim, Bachstr. -> Jahreskalender als iCal: abfallkalender.ics
// 3_m328\Abfalltermine: _2_ical-csv_Konverter.txt
// aus output.csv: "223-1-6T::","Biotonne" [search & replace] (auch Null) -> {1,6},

// Biotonne         LED grün
// Gelbe Tonne      LED gelb
// Altpapiertonne   LED blau
// Restmuelltonne   LED weiß / rot / Farbwechsler?

static const uint8_t Bio[][2] PROGMEM = { // Monat,Tag
{1,6},
{1,19},
{2,2},
{2,16},
  // Test (1 Tag zu heute (17.2.) dazuzählen):
  // {2,18},
{3,1},
{3,15},
{3,28},
{4,12},
{4,26},
{5,11},
{5,25},
{6,7},
{6,21},
{7,5},
{7,19},
{8,2},
{8,16},
{8,3},
{9,13},
{9,27},
{1,11},
{1,25},
{11,8},
{11,22},
{12,6},
{12,2},
};
#define Bio_Termine 26


static const uint8_t Schwarz[][2] PROGMEM = {
{1,12},
{1,26},
{2,9},
{2,23},
{3,8},
{3,22},
{4,6},
{4,19},
{5,4},
{5,17},
{6,1},
{6,14},
{6,28},
{7,12},
{7,26},
{8,9},
{8,23},
{9,6},
{9,2},
{1,5},
{1,18},
{11,1},
{11,15},
{11,29},
{12,13},
{12,28},
};
#define Schwarz_Termine 26


static const uint8_t Blau[][2] PROGMEM = {
{1,18},
{2,15},
{3,14},
{4,11},
{5,1},
{6,6},
{7,4},
{8,1},
{8,29},
{9,26},
{1,24},
{11,21},
{12,19},
};
#define Blau_Termine 13


static const uint8_t Gelb[][2] PROGMEM = {
{1,5},
{1,18},
{2,1},
{2,15},
{2,29},
{3,14},
{3,27},
{4,11},
{4,25},
{5,1},
{5,24},
{6,6},
{6,2},
{7,4},
{7,18},
{8,1},
{8,15},
{8,29},
{9,12},
{9,26},
{1,1},
{1,24},
{11,7},
{11,21},
{12,5},
{12,19},
};
#define Gelb_Termine 26

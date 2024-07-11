// aus output.csv: "2023-01-06T::","Biotonne" [search & replace] -> {1,6},

// Biotonne         LED grün
// Gelbe Tonne      LED gelb
// Altpapiertonne   LED blau
// Restmuelltonne   LED weiß / rot / Farbwechsler?

static const uint8_t Bio[][2] PROGMEM = { // Monat,Tag
{1,6},
{1,20},
{2,3},
{2,17},
{3,3},
{3,17},
{3,31},
{4,15},
{4,28},
{5,12},
{5,26},
{6,10},
  // Test (1 Tag dazuzählen):
  // {6,14},
{6,23},
{7,7},
{7,21},
{8,4},
{8,18},
{9,1},
{9,15},
{9,29},
{10,13},
{10,27},
{11,10},
{11,24},
{12,8},
{12,22}
};
#define Bio_Termine 26 // (Zeile) 34 - (Zeile) 9 +1

static const uint8_t Gelb[][2] PROGMEM = {
{1,5},
{1,19},
{2,2},
{2,16},
{3,2},
{3,16},
{3,30},
{4,14},
{4,27},
{5,11},
{5,25},
{6,9},
{6,22},
{7,6},
{7,20},
{8,3},
{8,17},
{8,31},
{9,14},
{9,28},
{10,12},
{10,26},
{11,9},
{11,23},
{12,7},
{12,21}
};
#define Gelb_Termine 26 // (Zeile) 64 - (Zeile) 39 +1

static const uint8_t Blau[][2] PROGMEM = {
{1,19},
{2,16},
{3,16},
{4,14},
{5,11},
{6,9},
{7,6},
{8,3},
{8,31},
{9,28},
{10,26},
{11,23},
{12,21},
};
#define Blau_Termine 13 // (Zeile) 81 - (Zeile) 69 +1

static const uint8_t Schwarz[][2] PROGMEM = {
{1,13},
{1,27},
{2,10},
{2,24},
{3,10},
{3,24},
{4,6},
{4,21},
{5,6},
{5,20},
{6,3},
{6,16},
{6,30},
{7,14},
{7,28},
{8,11},
{8,25},
{9,8},
{9,22},
{10,7},
{10,20},
{11,3},
{11,17},
{12,1},
{12,15},
{12,30},
};
#define Schwarz_Termine 26 // (Zeile) 111 - (Zeile) 86 +1

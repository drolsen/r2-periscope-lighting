///////////////////////////////////////////////////////////// 
// R-Series Periscope Lighting Kit Sketch (Devin R. Olsen) //
/////////////////////////////////////////////////////////////
// Configurable options
int Brightness = 250;     // Defines the brightness of RGB leds (255 max / 200 recommended)
byte COMTYPE = 0;         // 0 === Legacy three pin ABC communication | 1 === RX/TX serial communication (requires RX/TX pads be soldered on top PCB)
byte MODE = 0;            // 0 == FullCycle | 1 == FullOff | 2 == ObiWanLights | 3 == YodaLights | 4 == SithLights | 5 == SearchLights | 6 == DagobahLights | 7 == SparkelLights | 8 == FullOn

// Mode Speeds
int ObiwanSpeed = 150;    // Speed Obiwan lights mode
int YodaSpeed = 250;      // Speed Yoda lights mode
int SithSpeed = 250;      // Speed Sith lights mode
int SearchSpeed = 45;    // Speed Search lights mode
int DagobahSpeed = 500;  // Speed Degobah lights mode
int SparkelSpeed = 50;    // Speed Sparkel lights mode

// All Colors in RGB format
int RED[3] = {255, 0, 0};
int GREEN[3] = {0, 255, 0};
int BLUE[3] = {0, 0, 255};
int LISBORN[3] = {20, 20, 10};
int VIOLET[3] = {189, 51, 255};
int DARKVIOLET[3] = {35, 5, 45};
int WHITE[3] = {255, 255, 255};
int BLACK[3] = {0, 0, 0};

// Mode's Used Colors
int ObiwanColors[2] =   {BLUE, WHITE};
int YodaColors[2] =     {GREEN, LISBORN};
int SithColors[2] =     {RED, LISBORN};
int SearchColors[2] =   {WHITE, DARKVIOLET};
int DagobahColors[2] =  {VIOLET, WHITE};
int SparkelColors[2] =  {VIOLET, WHITE};

//////////////////////////////////////
// Do not change the settings below //
//////////////////////////////////////
#include <Adafruit_NeoPixel.h>
#define RGBPin 14     // Data pin number for LEDS
#define RGBCount 31   // How many RGB LEDS on Lights
Adafruit_NeoPixel RGBLEDS(RGBCount, RGBPin, NEO_GRB + NEO_KHZ800);

byte RGBCenter[2] = {0, 11};  // Center RGB Pixel Range
byte RGBLeft[2] = {12, 17};   // Left RGB Pixel Range
byte RGBFront[2] = {18, 24};  // Front RGB Pixel Range
byte RGBRight[2] = {25, 30};  // Right RGB Pixel Range

// RED LED PINS
byte REDBack[2] = {11, 13};   // Back RED LED Range
byte REDFront[2] = {2, 7};    // Front RED LED Range

// COM PINS
#define COM0 8
#define COM1 9
#define COM2 10

// Non-delay Wait Timestamp Helpers
unsigned long REDBackWait = millis();
unsigned long REDFrontWait = millis();
unsigned long CenterWait = millis();
unsigned long LeftWait = millis();
unsigned long FrontWait = millis();
unsigned long RightWait = millis();
unsigned long FullCycleWait = millis();

// States (Helpers used as flip-flop or reference current states)
byte REDBackCurrent = REDBack[0];
byte REDFrontCurrent = REDFront[0];
byte CenterLEDSCurrent = RGBCenter[0];
byte LeftLEDSCurrent = RGBLeft[0];
byte FrontTopLEDSCurrent = RGBFront[0];
byte RightLEDSCurrent = RGBRight[0];
byte FullCycleCurrent = 2;

byte CenterDirection = 1;       // 0 = move up / 1 = move down
byte LeftDirection = 1;         // 0 = move up / 1 = move down
byte FrontTopDirection = 1;     // 0 = move up / 1 = move down
byte RightDirection = 1;        // 0 = move up / 1 = move down
byte REDFrontDirection = 1;     // 0 = move left / 1 = move right

void setup() {
  Serial.begin(9600);
  
  // A, B, C three pin communication    
  pinMode(COM0, INPUT_PULLUP);
  pinMode(COM1, INPUT_PULLUP);
  pinMode(COM2, INPUT_PULLUP);

  for (byte i = 2; i <= 7; i++) {
    pinMode(i, OUTPUT);
    // digitalWrite(i, HIGH);
  }

  for (byte i = 11; i <= 13; i++) {
    pinMode(i, OUTPUT);
    // digitalWrite(i, HIGH);
  }  

  RGBLEDS.begin();
  RGBLEDS.show();
  RGBLEDS.setBrightness(Brightness); // Set pixel brightness defined above
}

void loop() {
  COMCheck();
  
  // Full Cycle
  if (MODE == 0) { FullCycle(); }
  
  // Full Off
  if (MODE == 1) { Off(); }

  // Obi Wan Lights
  if (MODE == 2) { ObiWanLights(); }

  // Yoda Lights
  if (MODE == 3) { YodaLights(); }

  // Sith Lights
  if (MODE == 4) { SithLights(); }

   // Search Light
  if (MODE == 5) { SearchLights(); }

  // Dagobah Lights
  if (MODE == 6) { DagobahLights(); } 

  // Lights Sparkel
  if (MODE == 7) { SparkelLights(); }

  // Debug
  if (MODE == 8) { DebugLights(); }
}


////////////////////
// HELPER METHODS //
////////////////////

// Checks COM for different light modes
void COMCheck() {
  ///////////////////////////
  // ABC Pin COMMUNICATION //
  ///////////////////////////
  char COMBOS[8][3] = {
    {HIGH, HIGH, HIGH},   // Full Cycle
    {HIGH, HIGH, LOW},    // Full Off
    {HIGH, LOW, HIGH},    // Obi Wan Lights
    {HIGH, LOW, LOW},     // Yoda Lights
    {LOW, HIGH, HIGH},    // Sith Lights
    {LOW, HIGH, LOW},     // Search Light
    {LOW, LOW, HIGH},     // Dagobah Lights
    {LOW, LOW, LOW}       // Lights Sparkel
  };
  
  if (COMTYPE == 0) {
    // Find a match in our COMBOS array
    for (byte i = 0; i < 7; i++) {
      if (
        COM0 == COMBOS[i][0] 
        && COM1 == COMBOS[i][1] 
        && COM2 == COMBOS[i][2]
      ) {
        MODE = i;
      }
    }
  } else {
    
  //////////////////////////////////////
  // Alternative RX/TX COMMUNICATION  //
  //////////////////////////////////////
    if (Serial.available() > 0) {
      MODE = Serial.parseInt();
    }
  }
}

// Debug LEDs
void DebugLights() {
  // Light up all RGB LEDs as blue
  for (int i = 0; i < RGBCount; i++) {
    RGBLEDS.setPixelColor(i, 0, 0, 255);
  }

  // Show pixel's change
  RGBLEDS.show();
  
  // Turn on all RED back LEDs
  for (byte i = REDBack[0]; i <= REDBack[1]; i++) {
    digitalWrite(i, HIGH);
  }
  
  // Turn on all RED front LEDs
  for (byte i = REDFront[0]; i <= REDFront[1]; i++) {
    digitalWrite(i, HIGH);
  }
}

// Clearing LEDs
void Clear(byte side[2], int bgColor[3], bool isDigital) {
  if (isDigital) {
    for (int i = side[0]; i <= side[1]; i++) {
      digitalWrite(i, LOW);
    }
  } else {
    if (bgColor) {
      RGBLEDS.fill(RGBLEDS.Color(bgColor[0], bgColor[1], bgColor[2]), side[0], ((side[1] - side[0]) + 1));
    } else {
      RGBLEDS.fill(RGBLEDS.Color(0, 0, 0), side[0], ((side[1] - side[0]) + 1));
    }
  }
}

///////////////////////
// ANIMATION METHODS //
///////////////////////

// On / Off
void On(int rgb[3], byte pixelRange[2]){
  for (byte i = pixelRange[0]; i <= pixelRange[1]; i++){
    RGBLEDS.setPixelColor(i, RGBLEDS.Color(rgb[0], rgb[1], rgb[2]));    
  }
  RGBLEDS.show();
}

void Off() {
  RGBLEDS.clear();
  Clear(REDFront, NULL, true);
  Clear(REDBack, NULL, true); 
}

void FrontBottom__On(){
  for (byte i = 2; i <= 7; i++){
    digitalWrite(i, HIGH);   
  }
}

// Sweeps 
void FullSweep(int rgb[3], int bgColor[3], int delay, unsigned long& wait) {
  byte SweepSequence[17][6] = {
    {28},         // left
    {28, 29},
    {27, 26},
    {31, 30},
    {31},
    {3, 25},          // center & front top
    {2, 4, 24},
    {1, 10, 11, 5, 23},
    {1, 9, 12, 5, 22},
    {6, 9, 12, 5, 21},
    {8, 6, 20},
    {7, 19},
    {18},         // right
    {18, 17},
    {14, 13},
    {15, 16},
    {15}
  };

  if (millis() > (wait + delay)) {
    // Change Direction
    if (CenterDirection == 1) {
      CenterLEDSCurrent++;
      if (CenterLEDSCurrent > 15) { CenterDirection = 0; }
    } else {
      CenterLEDSCurrent--; 
      if (CenterLEDSCurrent <= 0) { CenterDirection = 1; }  
    }
    
    // First, lets set all LEDS to BG color
    for (int i = 0; i < RGBCount; i++) {
      RGBLEDS.setPixelColor(i, 
        RGBLEDS.Color(
          bgColor[0], 
          bgColor[1], 
          bgColor[2]
        )
      );
    }
    RGBLEDS.show();
    
    // Second, lets loop over squence array and define set sweep color
    for (int i = 0; i < 5; i++) {
      if (SweepSequence[CenterLEDSCurrent][i]) {
        RGBLEDS.setPixelColor(
          (SweepSequence[CenterLEDSCurrent][i] - 1), 
          RGBLEDS.Color(
            rgb[0], 
            rgb[1], 
            rgb[2]
          )
        );
      }
    }       
    RGBLEDS.show();
    wait = millis();
  }
}

void FrontSweep(int rgb[3], int delay, unsigned long& wait) {
  if (millis() > (wait + delay)) {
    // Change Direction
    if (FrontTopDirection == 1) {
      FrontTopLEDSCurrent++;
      if (FrontTopLEDSCurrent == 24) { FrontTopDirection = 0; }    
    } else {
      FrontTopLEDSCurrent--; 
      if (FrontTopLEDSCurrent == 18) { FrontTopDirection = 1; }    
    }    
    
    // First turn off all front LEDS
    Clear(RGBFront, NULL, false);
    RGBLEDS.setPixelColor(FrontTopLEDSCurrent, RGBLEDS.Color(rgb[0], rgb[1], rgb[2]));
    
    // Reset timestamp checking
    RGBLEDS.show();
    wait = millis();
  }
}

void REDFrontSweep() {
  if (millis() > (REDFrontWait + 500)) {
    // First turn off all front LEDS
    Clear(REDFront, NULL, true);
        
    // Increase current front RED LED
    if (REDFrontDirection == 1) {
      REDFrontCurrent++;
      if (REDFrontCurrent == 7) { REDFrontDirection = 0; }    
    } else {
      REDFrontCurrent--; 
      if (REDFrontCurrent == 2) { REDFrontDirection = 1; }    
    }
    
    digitalWrite(REDFrontCurrent, HIGH);

    // Reset timestamp checking
    REDFrontWait = millis();
  }
}

// Randoms
void Random(int rgb[3], int background[3], byte side[2], int delay, unsigned long& wait) {
  if (millis() > (wait + delay)) {
    Clear(side, background, false);
    
    int randomA = random(side[0], (side[1]+1));
    int randomB = random(side[0], (side[1]+1));
    
    // For random dual color instances
    while (randomB == randomA) {
      randomB = random(side[0], (side[1]+1));
    }

    RGBLEDS.setPixelColor(
      randomA, 
      RGBLEDS.Color(
        rgb[0], 
        rgb[1], 
        rgb[2]
      )
    );
    
    RGBLEDS.setPixelColor(
      randomB, 
      RGBLEDS.Color(
        rgb[0], 
        rgb[1], 
        rgb[2]
      )
    );

    RGBLEDS.show();
    wait = millis();
  }
}

void REDFrontRandom() {
  if (millis() >= (REDFrontWait + 500)) {
    Clear(REDFront, NULL, true);
    REDFrontCurrent = random(1, 6);
    digitalWrite(REDFront[REDFrontCurrent], HIGH);

    REDFrontWait = millis();
  }
}

void REDBackRandom() {
  if (millis() > (REDBackWait + 2000)) {
    //Clear(BackMinMax, NULL, true);
    REDBackCurrent = random(11, 14);
    digitalWrite(REDBackCurrent, HIGH);

    REDBackWait = millis();
  }  
}

// Flashes
void Flash(int rgb[3], byte side[2], unsigned long& speed, byte& direction) {
  if (millis() >= speed) {
    if (direction == 0) {
      for (byte i = side[0]; i < side[1]; i++) {
        RGBLEDS.setPixelColor(
          i, 
          RGBLEDS.Color(
            rgb[0], 
            rgb[1], 
            rgb[2]
          )
        );
      }
      direction = 1;
    } else {
      for (byte i = side[0]; i < side[1]; i++) {
        RGBLEDS.setPixelColor(
          i, 
          RGBLEDS.Color(
            rgb[0], 
            rgb[0], 
            rgb[0]
          )
        );
      }
      direction = 0;
    }

    speed = millis(); // reset delay wait (aka delay speed relative to clock)
    RGBLEDS.show();
  }
}

// Fades
void Fade(int rgb[3], byte side[2], byte& direction, int delay, unsigned long& wait) {
  if (millis() > (wait + delay)) {
    direction = (direction != 1) ? 1 : 0;
    uint8_t r = (RGBLEDS.getPixelColor(side[0]) >> 16);
    uint8_t g = (RGBLEDS.getPixelColor(side[0]) >> 8);
    uint8_t b = RGBLEDS.getPixelColor(side[0]);

    for (int i = 0; i <= 255; i++) {
      if (direction == 0) {
        if (r > 0) { r--; }
        if (g > 0) { g--; }
        if (b > 0) { b--; }
        
        if (r == 0 && g == 0 && b == 0) {i = 255; } 
      }

      if (direction == 1) {
        if (r < rgb[0]) { r++;}
        if (g < rgb[1]) { g++; }
        if (b < rgb[2]) { b++; }

        if (r == rgb[0] && g == rgb[1] && b == rgb[2]) {i = 255; } 
      }

      RGBLEDS.fill(RGBLEDS.Color(r, g, b), side[0], ((side[1] - side[0]) + 1));
      RGBLEDS.show();        
    }
    
    delay = millis();
  }
}

////////////////////
// MODE SEQUENCES //
////////////////////

void FullCycle() {
  if (millis() > (FullCycleWait + 5000)) {
    FullCycleCurrent++;
    
    if (FullCycleCurrent > 6) {
      FullCycleCurrent = 2;
    }
    
    FullCycleWait = millis();
  }

  if (FullCycleCurrent == 2) { YodaLights(); }
  if (FullCycleCurrent == 3) { SithLights(); }
  if (FullCycleCurrent == 4) { SearchLights(); }
  if (FullCycleCurrent == 5) { DagobahLights(); }
  if (FullCycleCurrent == 6) { SparkelLights(); }  
}

void ObiWanLights() {
  Fade(ObiwanColors[0], RGBFront, FrontTopDirection, ObiwanSpeed, FrontWait);
  On(BLUE, RGBRight);
  On(BLUE, RGBLeft);
  Random(ObiwanColors[1], ObiwanColors[0], RGBCenter, ObiwanSpeed, CenterWait);

  REDFrontSweep();
  REDBackRandom();
}

void YodaLights() {
  Random(YodaColors[0], YodaColors[1], RGBCenter, YodaSpeed, CenterWait);
  Fade(YodaColors[0], RGBLeft, LeftDirection, YodaSpeed, LeftWait);
  Fade(YodaColors[0], RGBRight, RightDirection, YodaSpeed, RightWait);
  Fade(YodaColors[0], RGBFront, FrontTopDirection, YodaSpeed, FrontWait);

  REDFrontSweep();
  REDBackRandom();
}

void SithLights() {
  Fade(SithColors[0], RGBFront, FrontTopDirection, SithSpeed, FrontWait);
  Fade(SithColors[0], RGBLeft, LeftDirection, SithSpeed, LeftWait);
  Fade(SithColors[0], RGBRight, RightDirection, SithSpeed, RightWait);
  Random(SithColors[0], SithColors[1], RGBCenter, (SithSpeed * 2), CenterWait);
  
  REDFrontSweep();
  REDBackRandom();
}

void SearchLights() {
  FullSweep(SearchColors[0], SearchColors[1], SearchSpeed, CenterWait);
  REDFrontSweep();
  REDBackRandom();  
}

void DagobahLights() {
  FrontSweep(DagobahColors[0], DagobahSpeed, FrontWait); // Sweeps a violet light across top lights at an interval of 1 second
  On(DagobahColors[1], RGBCenter);  
  On(DagobahColors[1], RGBRight);
  On(DagobahColors[1], RGBLeft);
   
  FrontBottom__On();
  REDBackRandom();
}

void SparkelLights() {
  Random(SparkelColors[1], NULL, RGBCenter, SparkelSpeed, CenterWait);
  Random(SparkelColors[0], NULL, RGBLeft, SparkelSpeed, LeftWait);
  Random(SparkelColors[0], NULL, RGBRight, SparkelSpeed, RightWait);
  Random(SparkelColors[0], NULL, RGBFront, SparkelSpeed, FrontWait);
  
  REDFrontSweep();
  REDBackRandom();
}

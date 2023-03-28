//////////////////////////////////////////////////////
//     R-Series Periscope Lighting Kit Sketch       // 
//          (Devin R. Olsen - 3/28/2023)            //
// https://github.com/drolsen/r2-periscope-lighting //
//////////////////////////////////////////////////////

// Configurable options
int Brightness = 250;     // Defines the brightness of RGB leds (255 max / 200 recommended)
byte COMTYPE = 0;         // 0 === Legacy three pin ABC communication | 1 === RX/TX serial communication (requires RX/TX pads be soldered on top PCB)
byte MODE = 1;            // 1 == FullCycle | 2 == FullOff | 3 == ObiWanLights | 4 == YodaLights | 5 == SithLights | 6 == SearchLights | 7 == DagobahLights | 8 == SparkelLights | 9 == FullOn

// Mode Speeds (smaller numbers is faster)
int ObiwanSpeed = 5;        // Speed Obiwan lights mode
int YodaSpeed = 5;          // Speed Yoda lights mode
int SithSpeed = 5;          // Speed Sith lights mode
int SearchSpeed = 45;       // Speed Search lights mode
int DagobahSpeed = 500;     // Speed Degobah lights mode
int SparkelSpeed = 50;      // Speed Sparkel lights mode

// Durations (smaller numbers is faster)
int FullCycleDuration = 10000;    // Duration between each mode
int REDFrontDuration = 900;       // Duration between each RED front LEDs lighting up
int REDBackDuration = 1000; // Duration between random on/off states for RED back LEDs

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

byte RGBAllSides[2] = {12, 30};
byte RGBCenter[2] = {0, 11};  // Center RGB Pixel Range
byte RGBLeft[2] = {12, 17};   // Left RGB Pixel Range
byte RGBFront[2] = {18, 24};  // Front RGB Pixel Range
byte RGBRight[2] = {25, 30};  // Right RGB Pixel Range

// RED LED PINS
byte REDBack[2] = {11, 13};   // Back RED LED Range
byte REDFront[2] = {2, 7};    // Front RED LED Range

// COM PINS
byte MODETEST;
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
byte FullCycleCurrent = 3;

byte CenterDirection = 1;       // 0 = move up / 1 = move down
byte LeftDirection = 1;         // 0 = move up / 1 = move down
byte FrontTopDirection = 1;     // 0 = move up / 1 = move down
byte RightDirection = 1;        // 0 = move up / 1 = move down
byte REDFrontDirection = 1;     // 0 = move left / 1 = move right

int FrontRGBState[3] = {0, 0, 0};
int LeftRGBState[3] = {0, 0, 0};
int RightRGBState[3] = {0, 0, 0};
int CenterRGBState[3] = {0, 0, 0};

void setup() {
  if (COMTYPE == 1) {
    Serial.begin(9600);
    Serial.setTimeout(50);
  }
  
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
  if (MODE == 1) { FullCycle(); }
  
  // Full Off
  if (MODE == 2) { Off(); }

  // Obi Wan Lights
  if (MODE == 3) { ObiWanLights(); }

  // Yoda Lights
  if (MODE == 4) { YodaLights(); }

  // Sith Lights
  if (MODE == 5) { SithLights(); }

   // Search Light
  if (MODE == 6) { SearchLights(); }

  // Dagobah Lights
  if (MODE == 7) { DagobahLights(); } 

  // Lights Sparkel
  if (MODE == 8) { SparkelLights(); }

  // Debug (all on)
  if (MODE == 9) { DebugLights(); }
}


////////////////////
// HELPER METHODS //
////////////////////
// Checks COM for different light modes
void COMCheck() {
  ///////////////////////////
  // ABC Pin COMMUNICATION //
  ///////////////////////////
  int COMBOS[8][3] = {
    {1, 1, 1},   // Full Cycle
    {1, 1, 0},   // Full Off
    {1, 0, 1},   // Obi Wan Lights
    {1, 0, 0},   // Yoda Lights
    {0, 1, 1},   // Sith Lights
    {0, 1, 0},   // Search Light
    {0, 0, 1},   // Dagobah Lights
    {0, 0, 0}    // Lights Sparkel
  };
  
  if (COMTYPE == 0) {
    // Find a match in our COMBOS array
    for (byte i = 0; i <= 7; i++) {
      if (
        digitalRead(COM0) == COMBOS[i][0] 
        && digitalRead(COM1) == COMBOS[i][1] 
        && digitalRead(COM2) == COMBOS[i][2]
      ) {
        MODE = (i+1); // Cause mode is not zero based
      }
    }
  } else {
    //////////////////////////////////////
    // Alternative RX/TX COMMUNICATION  //
    //////////////////////////////////////
    if (Serial.available()) {
      MODETEST = Serial.parseInt();                    //Read user input and hold it in a variable
      if (MODETEST > 0) {
        MODE = MODETEST;
      }
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
void On(int rgb[3], byte side[2]){
  if (rgb != NULL) {
    for (byte i = side[0]; i <= side[1]; i++){
      RGBLEDS.setPixelColor(i, RGBLEDS.Color(rgb[0], rgb[1], rgb[2]));    
    }
    RGBLEDS.show();
  } else {
    for (byte i = side[0]; i <= side[1]; i++){
      digitalWrite(i, HIGH);    
    }    
  }
}

void Off() {
  for (byte i = 0; i <= RGBCount; i++) {
    RGBLEDS.setPixelColor(i, RGBLEDS.Color(0, 0, 0));   
  }
  RGBLEDS.show();
  
  Clear(REDFront, NULL, true);
  Clear(REDBack, NULL, true); 
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

// Left to right sweeps
void FrontSweepLTR(int rgb[3], int delay, unsigned long& wait) {
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

void REDFrontSweepLTR() {
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

// Left & Right to Center sweeps
void FrontSweepLRTC(int rgb[3], int delay, unsigned long& wait) {
  if (millis() > (wait + delay)) {
    // Change Direction
    byte SweepSequence[4][2] = {
      {18, 24},
      {19, 23},
      {20, 22},
      {21, 21}
    };
    
    if (FrontTopDirection == 1) {
      FrontTopLEDSCurrent++;
      if (FrontTopLEDSCurrent == 3) { FrontTopDirection = 0; }    
    } else {
      FrontTopLEDSCurrent--; 
      if (FrontTopLEDSCurrent == 0) { FrontTopDirection = 1; }    
    }    
    
    // First turn off all front LEDS
    Clear(RGBFront, NULL, false);
    RGBLEDS.setPixelColor(SweepSequence[FrontTopLEDSCurrent][0], RGBLEDS.Color(rgb[0], rgb[1], rgb[2]));
    RGBLEDS.setPixelColor(SweepSequence[FrontTopLEDSCurrent][1], RGBLEDS.Color(rgb[0], rgb[1], rgb[2]));
    
    // Reset timestamp checking
    RGBLEDS.show();
    wait = millis();
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

void REDBackRandom() {
  if (millis() > (REDBackWait + REDBackDuration)) {
    Clear(REDBack, NULL, true);
    REDBackCurrent = random(11, 14);
    digitalWrite(REDBackCurrent, HIGH);

    REDBackWait = millis();
  }  
}

// Fades
void Fade(int rgb[3], int rgbState[3], byte side[2], byte& direction, int delay, unsigned long& wait) {
  if (millis() > (wait + delay)) {
    if (rgbState[0] > rgb[0]) { rgbState[0] = 0; }
    if (rgbState[1] > rgb[1]) { rgbState[1] = 0; }
    if (rgbState[2] > rgb[2]) { rgbState[2] = 0; }

    if (direction == 0) {
      if (rgbState[0] > 0) { rgbState[0]--; }
      if (rgbState[1] > 0) { rgbState[1]--; }
      if (rgbState[2] > 0) { rgbState[2]--; }
  
      if (
        rgbState[0] == 0 
        && rgbState[1] == 0 
        && rgbState[2] == 0
      ) {
        direction = 1;
      }
    }

    if (direction == 1) {
      if (rgbState[0] < rgb[0]) { rgbState[0]++; }
      if (rgbState[1] < rgb[1]) { rgbState[1]++; }
      if (rgbState[2] < rgb[2]) { rgbState[2]++; }
  
      if (
        rgbState[0] == rgb[0] 
        && rgbState[1] == rgb[1] 
        && rgbState[2] == rgb[2]
      ) {
        direction = 0;
      }
    }

    RGBLEDS.fill(RGBLEDS.Color(rgbState[0], rgbState[1], rgbState[2]), side[0], ((side[1] - side[0]) + 1));
    RGBLEDS.show(); 
    wait = millis();
  }
}

////////////////////
// MODE SEQUENCES //
////////////////////
void FullCycle() {
  if (millis() > (FullCycleWait + FullCycleDuration)) {
    FullCycleCurrent++;
    
    if (FullCycleCurrent > 8) { FullCycleCurrent = 3; }
    
    FullCycleWait = millis();
  }

  if (FullCycleCurrent == 3) { ObiWanLights(); }
  if (FullCycleCurrent == 4) { YodaLights(); }
  if (FullCycleCurrent == 5) { SithLights(); }
  if (FullCycleCurrent == 6) { SearchLights(); }
  if (FullCycleCurrent == 7) { DagobahLights(); }
  if (FullCycleCurrent == 8) { SparkelLights(); }
}

void ObiWanLights() {
  On(BLUE, RGBRight);
  On(BLUE, RGBLeft);   
  Fade(ObiwanColors[0], FrontRGBState, RGBFront, FrontTopDirection, ObiwanSpeed, FrontWait);
  REDFrontSweepLTR();
  REDBackRandom();
  Random(ObiwanColors[1], ObiwanColors[0], RGBCenter, (ObiwanSpeed * 25), CenterWait);
}

void YodaLights() {
  Fade(YodaColors[0], LeftRGBState, RGBLeft, LeftDirection, YodaSpeed, LeftWait);
  Fade(YodaColors[0], RightRGBState, RGBRight, RightDirection, YodaSpeed+1, RightWait);
  Fade(YodaColors[0], FrontRGBState, RGBFront, FrontTopDirection, YodaSpeed+2, FrontWait);
  REDFrontSweepLTR();
  REDBackRandom();
  Random(YodaColors[0], YodaColors[1], RGBCenter, (YodaSpeed * 20), CenterWait);
}

void SithLights() {
  Fade(SithColors[0], LeftRGBState, RGBLeft, LeftDirection, SithSpeed, LeftWait);
  Fade(SithColors[0], RightRGBState, RGBRight, RightDirection, SithSpeed+2, RightWait);
  Fade(SithColors[0], FrontRGBState, RGBFront, FrontTopDirection, SithSpeed+3, FrontWait);
  REDFrontSweepLTR();
  REDBackRandom();
  Random(SithColors[0], SithColors[1], RGBCenter, (SithSpeed * 51), CenterWait);
}

void SearchLights() {
  FullSweep(SearchColors[0], SearchColors[1], SearchSpeed, CenterWait);
  REDFrontSweepLTR();
  REDBackRandom();  
}

void DagobahLights() {
  FrontSweepLTR(DagobahColors[0], DagobahSpeed, FrontWait); // Sweeps a violet light across top lights at an interval of 1 second
  On(DagobahColors[1], RGBCenter);  
  On(DagobahColors[1], RGBRight);
  On(DagobahColors[1], RGBLeft);
  On(NULL, REDFront);
  REDBackRandom();
}

void SparkelLights() {
  Random(SparkelColors[1], NULL, RGBCenter, SparkelSpeed, CenterWait);
  Random(SparkelColors[0], NULL, RGBLeft, SparkelSpeed, LeftWait);
  Random(SparkelColors[0], NULL, RGBRight, SparkelSpeed, RightWait);
  Random(SparkelColors[0], NULL, RGBFront, SparkelSpeed, FrontWait);
  REDFrontSweepLTR();
  REDBackRandom();
}

///////////////////////////////////////////////////////////// 
// R-Series Periscope Lighting Kit Sketch (Devin R. Olsen) //
/////////////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>

byte COMTYPE = 0; // 0 === Legacy three pin ABC communication | 1 === RX/TX serial communication (requires RX/TX pads be soldered on top PCB)

// EXTERNAL COMMUNICATION
#define COM0 8
#define COM1 9
#define COM2 10

// FRONT / BACK RED LEDS
#define REDLED1 2
#define REDLED2 3
#define REDLED3 4
#define REDLED4 5
#define REDLED5 6
#define REDLED6 7
#define REDLED7 11
#define REDLED8 12
#define REDLED9 13

// Colors
int RED[3] = {235, 33, 45};
int BLUE[3] = {47, 103, 248};
int GREEN[3] = {46, 249, 36};
int WHITE[3] = {255, 255, 255};
int BLACK[3] = {0, 0, 0};

// MODES
byte MODE = 0;

// BACK
byte BackLEDS[3] = { REDLED7, REDLED8, REDLED9 };
byte BackLEDSCurrent = 1;
byte BackDirection = 1; // 0 = move left / 1 = move right
unsigned long BackCurrentTime = 0;

// FRONT BOTTOM
byte FrontBottomLEDS[6] = { REDLED1, REDLED2, REDLED3, REDLED4, REDLED5, REDLED6 };
byte FrontBottomLEDSCurrent = 1;
byte FrontBottomDirection = 1; // 0 = move left / 1 = move right
unsigned long FrontBottomCurrentTime = 0;

// CENTER
byte CenterLEDS[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
byte CenterLEDSCurrent = CenterLEDS[0];
byte CenterDirection = 1; // 0 = move up / 1 = move down
unsigned long CenterCurrentTime = 0;

// LEFT
byte LeftLEDS[5] = { 10, 11, 12, 13, 14 };
byte LeftLEDSCurrent = LeftLEDS[0];
byte LeftDirection = 1; // 0 = move up / 1 = move down
unsigned long LeftCurrentTime = 0;

// FRONT TOP
byte FrontTopLEDS[7] = { 15, 16, 17, 18, 19, 20, 21 };
byte FrontTopLEDSCurrent = FrontTopLEDS[0];
byte FrontTopDirection = 1; // 0 = move up / 1 = move down
unsigned long FrontTopCurrentTime = 0;

// RIGHT
byte RightLEDS[5] = { 22, 23, 24, 25, 26 };
byte RightLEDSCurrent = RightLEDS[0];
byte RightDirection = 1; // 0 = move up / 1 = move down
unsigned long RightCurrentTime = 0;

// Millis delay variables
int BlinkWait = 1000;
unsigned long RGBLEDCurrentTime = 0;
unsigned long REDFRONTLEDCurrentTime = 0;
unsigned long REDBACKLEDCurrentTime = 0;

// RGB LEDS
Adafruit_NeoPixel RGBLEDS(26, 14, NEO_GRB + NEO_KHZ800);

void setup() {
  if (COMTYPE == 1) { 
    // RX / TX serial communication
    Serial.begin(9600); 
  } else if (COMTYPE == 0) {
    // Legacy A, B, C three pin communication    
    pinMode(COM0, INPUT_PULLUP);
    pinMode(COM1, INPUT_PULLUP);
    pinMode(COM2, INPUT_PULLUP);
  }
  
  for (byte i = 0; i < 5; i++) {
    pinMode(FrontBottomLEDS[i], OUTPUT);
    pinMode(FrontBottomLEDS[i], LOW);
  }

  for (byte i = 0; i < 2; i++) {
    pinMode(BackLEDS[i], OUTPUT);
    pinMode(BackLEDS[i], LOW);
  }  

  RGBLEDS.begin();
}

void loop() {
  COMCheck();

  // Full Cycle
  if (MODE == 0) { FullCycle(); }

  // Full Off
  if (MODE == 1) { FullOff(); }

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
}


////////////////////
// HELPER METHODS //
////////////////////

// Checks COM for different light modes
void COMCheck() {
  //////////////////////////////////////////
  // LEGACY COMMUNICATION (3 PIN A, B, C) //
  //////////////////////////////////////////
  if (COMTYPE == 0) {
    // Full Cycle
    if (COM0 == 1 && COM1 == 1 && COM2 == 1) { MODE = 0; }
  
    // Full Off
    else if (COM0 == 1 && COM1 == 1 && COM2 == 0) { MODE = 1; }
  
    // Obi Wan Lights
    else if (COM0 == 1 && COM1 == 0 && COM2 == 1) { MODE = 2; }
  
    // Yoda Lights
    else if (COM0 == 1 && COM1 == 0 && COM2 == 0) { MODE = 3; }
  
    // Sith Lights
    else if (COM0 == 0 && COM1 == 1 && COM2 == 1) { MODE = 4; }
  
     // Search Light
    else if (COM0 == 0 && COM1 == 1 && COM2 == 0) { MODE = 5; }
  
    // Dagobah Lights
    else if (COM0 == 0 && COM1 == 0 && COM2 == 1) { MODE = 6; } 
  
    // Lights Sparkel
    else if (COM0 == 0 && COM1 == 0 && COM2 == 0) { MODE = 7; }
  
    // Fallback mode
    else { MODE = 0; }
    
  } else {
    
  //////////////////////////////////////
  // Alternative RX/TX COMMUNICATION  //
  //////////////////////////////////////
    if (Serial.available() > 0) {
      byte COMValue = Serial.read();
      // Full Cycle
      if (COMValue == 0) { MODE = 0; }
    
      // Full Off
      else if (COMValue == 1) { MODE = 1; }
    
      // Obi Wan Lights
      else if (COMValue == 2) { MODE = 2; }
    
      // Yoda Lights
      else if (COMValue == 3) { MODE = 3; }
    
      // Sith Lights
      else if (COMValue == 4) { MODE = 4; }
    
       // Search Light
      else if (COMValue == 5) { MODE = 5; }
    
      // Dagobah Lights
      else if (COMValue == 6) { MODE = 6; } 
    
      // Lights Sparkel
      else if (COMValue == 7) { MODE = 7; }
    
      // Fallback mode
      else { MODE = 0; }
    }
  }
}

// Clearing LEDs
void Center__Clear() {
  for (byte i = 1; i < 8; i++) {
    RGBLEDS.setPixelColor(CenterLEDS[i], RGBLEDS.Color(BLACK[0], BLACK[1], BLACK[2]));
  }
}

void Left__Clear() {
  for (byte i = 1; i < 5; i++) {
    RGBLEDS.setPixelColor(LeftLEDS[i], RGBLEDS.Color(BLACK[0], BLACK[1], BLACK[2]));
  }
}

void Right__Clear() {
  for (byte i = i; i < 5; i++) {
    RGBLEDS.setPixelColor(RightLEDS[i], RGBLEDS.Color(BLACK[0], BLACK[1], BLACK[2]));
  }
}

void FrontTop__Clear() {
  for (byte i = i; i < 7; i++) {
    RGBLEDS.setPixelColor(FrontTopLEDS[i], RGBLEDS.Color(BLACK[0], BLACK[1], BLACK[2]));
  }
}

void FrontBottom__Clear() {
  for (byte i = 0; i < 5; i++){
    digitalWrite(FrontBottomLEDS[i], LOW);   
  }  
}

void BackClear() {
  for (byte i = 0; i < 2; i++){
    digitalWrite(BackLEDS[i], LOW);   
  }   
}

///////////////////////
// ANIMATION METHODS //
///////////////////////

// Full On
void Center__On(int red = 255, int green = 255, int blue = 255){
  for (byte i = 0; i < 8; i++){
    RGBLEDS.setPixelColor(CenterLEDS[i], RGBLEDS.Color(red, green, blue));    
  }
}

void Left__On(int red = 255, int green = 255, int blue = 255){
  for (byte i = 0; i < 4; i++){
    RGBLEDS.setPixelColor(LeftLEDS[i], RGBLEDS.Color(red, green, blue));    
  }
}

void Right__On(int red = 255, int green = 255, int blue = 255){
  for (byte i = 0; i < 4; i++){
    RGBLEDS.setPixelColor(RightLEDS[i], RGBLEDS.Color(red, green, blue));    
  }
}

void FrontTop__On(int red = 255, int green = 255, int blue = 255){
  for (byte i = 0; i < 6; i++){
    RGBLEDS.setPixelColor(FrontTopLEDS[i], RGBLEDS.Color(red, green, blue));    
  }
}

void FrontBottom__On(){
  for (byte i = 0; i < 6; i++){
    digitalWrite(FrontBottomLEDS[i], HIGH);   
  }
}

// Sweeps 
void Center__Sweep(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= CenterCurrentTime + BlinkWait) {
    // Change Direction
    if (CenterDirection == 1) {
      CenterLEDSCurrent++;
      if (CenterLEDSCurrent == 5) { CenterDirection = 0; }    
    } else {
      CenterLEDSCurrent--; 
      if (CenterLEDSCurrent == 1) { CenterDirection = 1; }    
    }    
    
    // First turn off all front LEDS
    
    // Next turn on current LED
    if (CenterLEDSCurrent == 1) { 
      RGBLEDS.setPixelColor(CenterLEDS[0], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(CenterLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[4], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[5], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[6], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[7], RGBLEDS.Color(0, 0, 0));   
      RGBLEDS.setPixelColor(CenterLEDS[8], RGBLEDS.Color(0, 0, 0));
    }
    
    if (CenterLEDSCurrent == 2) { 
      RGBLEDS.setPixelColor(CenterLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[1], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(CenterLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[4], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[5], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[6], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[7], RGBLEDS.Color(red, green, blue));   
      RGBLEDS.setPixelColor(CenterLEDS[8], RGBLEDS.Color(0, 0, 0)); 
     }
    
    if (CenterLEDSCurrent == 3) { 
      RGBLEDS.setPixelColor(CenterLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[2], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(CenterLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[4], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[5], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[6], RGBLEDS.Color(red, green, blue)); 
      RGBLEDS.setPixelColor(CenterLEDS[7], RGBLEDS.Color(0, 0, 0));   
      RGBLEDS.setPixelColor(CenterLEDS[8], RGBLEDS.Color(red, green, blue)); 
    }
    
    if (CenterLEDSCurrent == 4) { 
      RGBLEDS.setPixelColor(CenterLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[3], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(CenterLEDS[4], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[5], RGBLEDS.Color(red, green, blue)); 
      RGBLEDS.setPixelColor(CenterLEDS[6], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[7], RGBLEDS.Color(0, 0, 0));   
      RGBLEDS.setPixelColor(CenterLEDS[8], RGBLEDS.Color(0, 0, 0));    
    }

    if (CenterLEDSCurrent == 5) { 
      RGBLEDS.setPixelColor(CenterLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(CenterLEDS[4], RGBLEDS.Color(red, green, blue)); 
      RGBLEDS.setPixelColor(CenterLEDS[5], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[6], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(CenterLEDS[7], RGBLEDS.Color(0, 0, 0));   
      RGBLEDS.setPixelColor(CenterLEDS[8], RGBLEDS.Color(0, 0, 0));    
    }    

    // Reset timestamp checking
    CenterCurrentTime = millis();
  }
}

void Left__Sweep(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= LeftCurrentTime + BlinkWait) {
    // Change Direction
    if (LeftDirection == 1) {
      LeftLEDSCurrent++;
      if (LeftLEDSCurrent == 3) { LeftDirection = 0; }    
    } else {
      LeftLEDSCurrent--; 
      if (LeftLEDSCurrent == 1) { LeftDirection = 1; }    
    }    
    
    // First turn off all front LEDS
    
    // Next turn on current LED
    if (LeftLEDSCurrent == 1) { 
      RGBLEDS.setPixelColor(LeftLEDS[0], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(LeftLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(LeftLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(LeftLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(LeftLEDS[4], RGBLEDS.Color(0, 0, 0)); 
    }
    
    if (LeftLEDSCurrent == 2) { 
      RGBLEDS.setPixelColor(LeftLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(LeftLEDS[1], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(LeftLEDS[2], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(LeftLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(LeftLEDS[4], RGBLEDS.Color(0, 0, 0)); 
     }
    
    if (LeftLEDSCurrent == 3) { 
      RGBLEDS.setPixelColor(LeftLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(LeftLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(LeftLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(LeftLEDS[3], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(LeftLEDS[4], RGBLEDS.Color(red, green, blue));  
    }    

    // Reset timestamp checking
    LeftCurrentTime = millis();
  }
}

void Right__Sweep(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= RightCurrentTime + BlinkWait) {
    // Change Direction
    if (RightDirection == 1) {
      RightLEDSCurrent++;
      if (RightLEDSCurrent == 3) { RightDirection = 0; }    
    } else {
      LeftLEDSCurrent--; 
      if (RightLEDSCurrent == 1) { RightDirection = 1; }    
    }    
    
    // First turn off all front LEDS
    
    // Next turn on current LED
    if (RightLEDSCurrent == 1) { 
      RGBLEDS.setPixelColor(RightLEDS[0], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(RightLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(RightLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(RightLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(RightLEDS[4], RGBLEDS.Color(0, 0, 0)); 
    }
    
    if (RightLEDSCurrent == 2) { 
      RGBLEDS.setPixelColor(RightLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(RightLEDS[1], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(RightLEDS[2], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(RightLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(RightLEDS[4], RGBLEDS.Color(0, 0, 0)); 
     }
    
    if (RightLEDSCurrent == 3) { 
      RGBLEDS.setPixelColor(RightLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(RightLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(RightLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(RightLEDS[3], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(RightLEDS[4], RGBLEDS.Color(red, green, blue));  
    }    

    // Reset timestamp checking
    RightCurrentTime = millis();
  }
}

void FrontTop__Sweep(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= FrontTopCurrentTime + BlinkWait) {
    // Change Direction
    if (FrontTopDirection == 1) {
      FrontTopLEDSCurrent++;
      if (FrontTopLEDSCurrent == 5) { FrontTopDirection = 0; }    
    } else {
      FrontTopLEDSCurrent--; 
      if (FrontTopLEDSCurrent == 1) { FrontTopDirection = 1; }    
    }    
    
    // First turn off all front LEDS
    
    // Next turn on current LED
    if (FrontTopLEDSCurrent == 1) { 
      RGBLEDS.setPixelColor(FrontTopLEDS[0], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(FrontTopLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[4], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(FrontTopLEDS[5], RGBLEDS.Color(0, 0, 0)); 
    }
    
    if (FrontTopLEDSCurrent == 2) { 
      RGBLEDS.setPixelColor(FrontTopLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[1], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(FrontTopLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[4], RGBLEDS.Color(0, 0, 0)); 
      RGBLEDS.setPixelColor(FrontTopLEDS[5], RGBLEDS.Color(0, 0, 0)); 
     }
    
    if (FrontTopLEDSCurrent == 3) { 
      RGBLEDS.setPixelColor(FrontTopLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[2], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(FrontTopLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[4], RGBLEDS.Color(0, 0, 0));  
      RGBLEDS.setPixelColor(FrontTopLEDS[5], RGBLEDS.Color(0, 0, 0)); 
    }

    if (FrontTopLEDSCurrent == 4) { 
      RGBLEDS.setPixelColor(FrontTopLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[3], RGBLEDS.Color(red, green, blue));
      RGBLEDS.setPixelColor(FrontTopLEDS[4], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[5], RGBLEDS.Color(0, 0, 0));   
    }

    if (FrontTopLEDSCurrent == 5) { 
      RGBLEDS.setPixelColor(FrontTopLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[4], RGBLEDS.Color(red, green, blue)); 
      RGBLEDS.setPixelColor(FrontTopLEDS[5], RGBLEDS.Color(0, 0, 0));  
    }

    if (FrontTopLEDSCurrent == 6) { 
      RGBLEDS.setPixelColor(FrontTopLEDS[0], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[1], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[2], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[3], RGBLEDS.Color(0, 0, 0));
      RGBLEDS.setPixelColor(FrontTopLEDS[4], RGBLEDS.Color(0, 0, 0));  
      RGBLEDS.setPixelColor(FrontTopLEDS[5], RGBLEDS.Color(red, green, blue));  
    }                

    // Reset timestamp checking
    FrontTopCurrentTime = millis();
  }
}

void FrontBottom__Sweep() {
  if (millis() >= FrontBottomCurrentTime + BlinkWait) {
    // Increase current front RED LED
    if (FrontBottomDirection == 1) {
      FrontBottomLEDSCurrent++;
      if (FrontBottomLEDSCurrent == 6) { FrontBottomDirection = 0; }    
    }

    if (FrontBottomDirection == 0) {
      FrontBottomLEDSCurrent--; 
      if (FrontBottomLEDSCurrent == 1) { FrontBottomDirection = 1; }    
    }    
    
    // First turn off all front LEDS
    FrontBottom__Clear();

    // Next turn on current LED
    if (FrontBottomLEDSCurrent == 1) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 2) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 3) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 4) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 5) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 6) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }            

    // Reset timestamp checking
    FrontBottomCurrentTime = millis();
  }
}

void Back__Sweep() {
  if (millis() >= BackCurrentTime + BlinkWait) {
    // Increase current front RED LED
    if (BackDirection == 1) {
      BackLEDSCurrent++;
      if (BackLEDSCurrent == 3) { BackDirection = 0; }    
    }

    if (BackDirection == 0) {
      BackLEDSCurrent--; 
      if (BackLEDSCurrent == 1) { BackDirection = 1; }    
    }    
    
    // First turn off all front LEDS
    BackClear();

    // Next turn on current LED
    if (BackLEDSCurrent == 1) { digitalWrite(BackLEDS[BackLEDSCurrent], HIGH); }
    if (BackLEDSCurrent == 2) { digitalWrite(BackLEDS[BackLEDSCurrent], HIGH); }
    if (BackLEDSCurrent == 3) { digitalWrite(BackLEDS[BackLEDSCurrent], HIGH); }

    // Reset timestamp checking
    BackCurrentTime = millis();
  }
}


// Random Blink
void Center__Random(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= CenterCurrentTime + (BlinkWait / 4)) {
    Center__Clear();
    CenterLEDSCurrent = random(0, 9);
    RGBLEDS.setPixelColor(CenterLEDS[CenterLEDSCurrent], RGBLEDS.Color(red, green, blue));

    CenterCurrentTime = millis();
  }
}

void Left__Random(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= LeftCurrentTime + (BlinkWait / 4)) {
    Left__Clear();
    LeftLEDSCurrent = random(0, 5);
    RGBLEDS.setPixelColor(LeftLEDS[LeftLEDSCurrent], RGBLEDS.Color(red, green, blue));

    LeftCurrentTime = millis();
  }
}

void Right__Random(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= RightCurrentTime + (BlinkWait / 4)) {
    Right__Clear();
    RightLEDSCurrent = random(0, 5);
    RGBLEDS.setPixelColor(RightLEDS[RightLEDSCurrent], RGBLEDS.Color(red, green, blue));

    RightCurrentTime = millis();
  }
}

void FrontTop__Random(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= FrontTopCurrentTime + (BlinkWait / 4)) {
    FrontTop__Clear();
    FrontTopLEDSCurrent = random(0, 7);
    RGBLEDS.setPixelColor(FrontTopLEDS[FrontTopLEDSCurrent], RGBLEDS.Color(red, green, blue));

    RightCurrentTime = millis();
  }
}

void FrontBottom__Random() {
  if (millis() >= (FrontBottomCurrentTime + BlinkWait)) {
    FrontBottom__Clear();
    FrontBottomLEDSCurrent = random(1, 7);
    if (FrontBottomLEDSCurrent == 1) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 2) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 3) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 4) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 5) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }
    if (FrontBottomLEDSCurrent == 6) { digitalWrite(FrontBottomLEDS[FrontBottomLEDSCurrent], HIGH); }

    FrontBottomCurrentTime = millis();
  }
}

void Back__Random() {
  if (millis() >= BackCurrentTime + (BlinkWait / 2)) {
    BackClear();
    BackLEDSCurrent = random(1, 4);
    
    if (BackLEDSCurrent == 1) { digitalWrite(BackLEDS[BackLEDSCurrent], HIGH); }
    if (BackLEDSCurrent == 2) { digitalWrite(BackLEDS[BackLEDSCurrent], HIGH); }
    if (BackLEDSCurrent == 3) { digitalWrite(BackLEDS[BackLEDSCurrent], HIGH); }

    BackCurrentTime = millis();
  }  
}

// Cycle

// Flash
void Center__Flash(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= CenterCurrentTime + BlinkWait) {
    if (CenterDirection == 0) {
      for (byte i = 0; i < 8; i++) {
        RGBLEDS.setPixelColor(CenterLEDS[i], RGBLEDS.Color(red, green, blue));
      }
      CenterDirection = 1;
    } else {
      for (byte i = 0; i < 8; i++) {
        RGBLEDS.setPixelColor(CenterLEDS[i], RGBLEDS.Color(BLACK[0], BLACK[0], BLACK[0]));
      }
      CenterDirection = 0;
    }

    CenterCurrentTime = millis();
  }
}

void Left__Flash(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= LeftCurrentTime + BlinkWait) {
    if (LeftDirection == 0) {
      for (byte i = 0; i < 4; i++) {
        RGBLEDS.setPixelColor(LeftLEDS[i], RGBLEDS.Color(red, green, blue));
      }
      LeftDirection = 1;
    } else {
      for (byte i = 0; i < 4; i++) {
        RGBLEDS.setPixelColor(LeftLEDS[i], RGBLEDS.Color(BLACK[0], BLACK[0], BLACK[0]));
      }
      LeftDirection = 0;
    }

    LeftDirection = millis();
  }
}

void Right__Flash(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= RightCurrentTime + BlinkWait) {
    if (RightDirection == 0) {
      for (byte i = 0; i < 4; i++) {
        RGBLEDS.setPixelColor(RightLEDS[i], RGBLEDS.Color(red, green, blue));
      }
      RightDirection = 1;
    } else {
      for (byte i = 0; i < 4; i++) {
        RGBLEDS.setPixelColor(RightLEDS[i], RGBLEDS.Color(BLACK[0], BLACK[0], BLACK[0]));
      }
      RightDirection = 0;
    }

    RightDirection = millis();
  }  
}

void FrontTop__Flash(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= FrontTopCurrentTime + BlinkWait) {
    if (FrontTopDirection == 0) {
      for (byte i = 0; i < 6; i++) {
        RGBLEDS.setPixelColor(FrontTopLEDS[i], RGBLEDS.Color(red, green, blue));
      }
      FrontTopDirection = 1;
    } else {
      for (byte i = 0; i < 6; i++) {
        RGBLEDS.setPixelColor(FrontTopLEDS[i], RGBLEDS.Color(BLACK[0], BLACK[0], BLACK[0]));
      }
      FrontTopDirection = 0;
    }

    FrontTopDirection = millis();
  }  
}

void FrontBottom__Flash(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= FrontBottomCurrentTime + (BlinkWait / 2)) {
    if (FrontBottomDirection == 0) {
      FrontBottom__On();
      FrontTopDirection = 1;
    } else {
      FrontBottom__Clear();
      FrontTopDirection = 0;
    }

    FrontTopDirection = millis();
  }  
}

// Fades
void Center__Fade(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= CenterCurrentTime + BlinkWait) {
    if (CenterDirection == 0) {
      CenterDirection = 1;
    } else {
      CenterDirection = 0;
    }

    CenterCurrentTime = millis();
  }
  
  for (byte i = 0; i < 8; i++) {
    int r = (RGBLEDS.getPixelColor(CenterLEDS[i]) >> 16);
    int g = (RGBLEDS.getPixelColor(CenterLEDS[i]) >> 8);
    int b = RGBLEDS.getPixelColor(CenterLEDS[i]);
        
    if (CenterDirection == 1) {      
      if (r < red) { r++; }
      if (g < green) { g++; }
      if (b < blue) { b++; }
    } else {
      if (r > 0) { r--; }
      if (g > 0) { g--; }
      if (b > 0) { b--; }
    }  

    RGBLEDS.setPixelColor(CenterLEDS[i], RGBLEDS.Color(r, g, b));
  }
}

void Left__Fade(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= LeftCurrentTime + BlinkWait) {
    if (LeftDirection == 0) {
      LeftDirection = 1;
    } else {
      LeftDirection = 0;
    }

    LeftCurrentTime = millis();
  }
  
  for (byte i = 0; i < 4; i++) {
    int r = (RGBLEDS.getPixelColor(LeftLEDS[i]) >> 16);
    int g = (RGBLEDS.getPixelColor(LeftLEDS[i]) >> 8);
    int b = RGBLEDS.getPixelColor(LeftLEDS[i]);
        
    if (LeftDirection == 1) {      
      if (r < red) { r++; }
      if (g < green) { g++; }
      if (b < blue) { b++; }
    } else {
      if (r > 0) { r--; }
      if (g > 0) { g--; }
      if (b > 0) { b--; }
    }
    
    RGBLEDS.setPixelColor(LeftLEDS[i], RGBLEDS.Color(r, g, b));
  }
}

void Right__Fade(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= RightCurrentTime + BlinkWait) {
    if (RightDirection == 0) {
      RightDirection = 1;
    } else {
      RightDirection = 0;
    }

    RightCurrentTime = millis();
  }
  
  for (byte i = 0; i < 4; i++) {
    int r = (RGBLEDS.getPixelColor(RightLEDS[i]) >> 16);
    int g = (RGBLEDS.getPixelColor(RightLEDS[i]) >> 8);
    int b = RGBLEDS.getPixelColor(RightLEDS[i]); 
       
    if (RightDirection == 1) {      
      if (r < red) { r++; }
      if (g < green) { g++; }
      if (b < blue) { b++; }
    } else {
      if (r > 0) { r--; }
      if (g > 0) { g--; }
      if (b > 0) { b--; }
    }
    
    RGBLEDS.setPixelColor(RightLEDS[i], RGBLEDS.Color(r, g, b));
  }
}

void FrontTop__Fade(int red = 255, int green = 255, int blue = 255) {
  if (millis() >= FrontTopCurrentTime + BlinkWait) {
    if (FrontTopDirection == 0) {
      FrontTopDirection = 1;
    } else {
      FrontTopDirection = 0;
    }

    FrontTopCurrentTime = millis();
  }

  for (byte i = 0; i < 4; i++) {
    int r = (RGBLEDS.getPixelColor(FrontTopLEDS[i]) >> 16);
    int g = (RGBLEDS.getPixelColor(FrontTopLEDS[i]) >> 8);
    int b = RGBLEDS.getPixelColor(FrontTopLEDS[i]);
    
    if (FrontTopDirection == 1) {      
      if (r < red) { r++; }
      if (g < green) { g++; }
      if (b < blue) { b++; }
    } else {
      if (r > 0) { r--; }
      if (g > 0) { g--; }
      if (b > 0) { b--; }
    }
    
    RGBLEDS.setPixelColor(FrontTopLEDS[i], RGBLEDS.Color(r, g, b));
  }
}

////////////////////
// MODE SEQUENCES //
////////////////////

void FullOff() {
  RGBLEDS.clear();
  digitalWrite(REDLED1, LOW);
  digitalWrite(REDLED2, LOW);
  digitalWrite(REDLED3, LOW);
  digitalWrite(REDLED4, LOW);
  digitalWrite(REDLED5, LOW);
  digitalWrite(REDLED6, LOW);
  digitalWrite(REDLED7, LOW);
  digitalWrite(REDLED8, LOW);
  digitalWrite(REDLED9, LOW);  
}

void FullCycle() {
  byte RandomColor = random(0, 4);
  int red = 255;
  int green = 255;
  int blue = 255;
  
  if (RandomColor == 0) { red = RED[0]; green = RED[1]; blue = RED[2]; }
  if (RandomColor == 1) { red = GREEN[0]; green = GREEN[1]; blue = GREEN[2]; }
  if (RandomColor == 2) { red = BLUE[0]; green = BLUE[1]; blue = BLUE[2]; }
  if (RandomColor == 2) { red = WHITE[0]; green = WHITE[1]; blue = WHITE[2]; }

  // RGB LEDS
  Center__Random(red, green, blue);
  Left__Random(red, green, blue);
  FrontTop__Random(red, green, blue);
  Right__Random(red, green, blue);

  // Fixed color LEDS
  FrontBottom__Random();
  Back__Random();
}

void ObiWanLights() {
  FrontTop__Flash(BLUE[0], BLUE[1], BLUE[2]);
  Left__On(BLUE[0], BLUE[1], BLUE[2]);
  Right__On(BLUE[0], BLUE[1], BLUE[2]);
  Center__Random(WHITE[0], WHITE[1], WHITE[2]);
  
  RGBLEDS.setBrightness(210);
  RGBLEDS.show();
}

void YodaLights() {
  FrontTop__Fade(GREEN[0], GREEN[1], GREEN[2]);
  Left__Fade(GREEN[0], GREEN[1], GREEN[2]);
  Right__Fade(GREEN[0], GREEN[1], GREEN[2]);
  Center__Random(GREEN[0], GREEN[1], GREEN[2]);
  FrontBottom__Sweep();
  Back__Random();
}

void SithLights() {
  FrontTop__Fade(RED[0], RED[1], RED[2]);
  Left__Flash(RED[0], RED[1], RED[2]);
  Right__Flash(RED[0], RED[1], RED[2]);
  Center__Fade(RED[0], RED[1], RED[2]);
  FrontBottom__Sweep();
  Back__Random();
}

void SearchLights() {
  Center__On();
  Left__On();
  Right__On();
  Center__On();
  FrontBottom__Sweep();
  Back__Random();  
}

void DagobahLights() {
  Center__On();  
  Left__On();
  Right__On();
  FrontTop__Sweep(RED[0], RED[1], RED[2]);  
  FrontBottom__On();
  Back__Random();
}

void SparkelLights() {
  Center__Random();
  Left__Random();
  Right__Random();
  FrontTop__Sweep(BLUE[0], BLUE[1], BLUE[2]);
  FrontBottom__Sweep();
  Back__Random();
}

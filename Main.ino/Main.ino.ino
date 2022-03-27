byte COM0 = 7;
byte COM1 = 8;
byte COM2 = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(COM0, INPUT_PULLUP);
  pinMode(COM1, INPUT_PULLUP);
  pinMode(COM2, INPUT_PULLUP);

}

void loop() {
  // Full Cycle
  if (COM0 == 1 && COM1 == 1 && COM2 == 1) {}

  // Lights Sparkel
  if (COM0 == 0 && COM1 == 0 && COM2 == 0) {}
    
  // Lights Off
  if (COM0 == 1 && COM1 == 1 && COM2 == 0) {}

  // Obi Wan
  if (COM0 == 1 && COM1 == 0 && COM2 == 1) {}

  // Sith Lights
  if (COM0 == 0 && COM1 == 1 && COM2 == 1) {}

   // Search Light
  if (COM0 == 0 && COM1 == 1 && COM2 == 0) {}

  // Yoda Lights
  if (COM0 == 1 && COM1 == 0 && COM2 == 0) {}

  // Dagobah Lights
  if (COM0 == 0 && COM1 == 0 && COM2 == 1) {}          
}

#include <RP2040Version.h>

/* NOTES
*
* Have certain commmands for moving forward and backward that have array's with bytes 
* that structure the individual commands to move in eight motions. Keywords are going to be 
* sent over SPI to pass the data, but Leon is still figuring that out. 
*
* Need to have WASD, and strafe controls. Either full off or full on. probably need a margin of error for both.
*
*
* Idea for certain commands:
*
* Create if statements for storing information to variable that could send data. The if statements will be 
* in a certain parameter that will trigger the response of certain functions, like Ethan's encoders.
*
*/

// Define the pins for the joystick
const int joystickX = 28; // ADC2__X-axis analog pin
const int joystickY = 27; // ADC1__Y-axis analog pin
const int btnA = 22; // joystick button
const int btnB = 21; // green button
const int btnC = 20; // black button
const int btnD = 19; // blue button


// Variables for store values
int valX = 0;
int valY = 0; 

//////////////////////////////////////////////////////////////////////////////////////////
// MOTOR DIRECTION FUNCTIONS

struct Buttons {         
  bool A; // inverted button logic
  bool B; 
  bool C;
  bool D;
}; // end buttonState structure

// Assign Button Pin Locations
Buttons buttonState = { false, false, false, false }; // inverted button logic

bool stop(void) {              // need to use bool here because structure in T/F return 
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);

  const int centX = 512;
  const int centY = 512;
  const int dev = 10;

  const bool zeroRange = (abs(valX - centX) <= dev) && (abs(valY - centY) <= dev);
  return(zeroRange);
} // end bool stop
/////////////////////////////////////////////////////////////////////////////////////////////
// Standard Movements
bool forward(void) {
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);
  
  const int minY = 257;
  const int maxY = 767;
  const int minX = 523;
  const int maxX = 1023;

  const bool forwardRange = (valX >= minX && valX <= maxX) && (valY >= minY && valY <= maxY);
  return(forwardRange);
  } // end bool forward

bool right(void) {
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);
  
  const int minY = 523;
  const int maxY = 1023;
  const int minX = 257;
  const int maxX = 767;

  const bool rightRange = (valX >= minX && valX <= maxX) && (valY >= minY && valY <= maxY);
  return(rightRange);
  } // end bool forward

bool backward(void) {
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);
  
  const int minY = 257;
  const int maxY = 767;
  const int minX = 0;
  const int maxX = 501;

  const bool backwardRange = (valX >= minX && valX <= maxX) && (valY >= minY && valY <= maxY);
  return(backwardRange);
  } // end bool forward

bool left(void) {
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);
  
  const int minY = 0;
  const int maxY = 501;
  const int minX = 257;
  const int maxX = 767;

  const bool leftRange = (valX >= minX && valX <= maxX) && (valY >= minY && valY <= maxY);
  return(leftRange);
  } // end bool forward
/////////////////////////////////////////////////////////////////////////////////////////////
// Strafe Movements
bool strafe_FL(void) {
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);
  
  const int minY = 0;
  const int maxY = 256;
  const int minX = 768;
  const int maxX = 1023;

  const bool strafe_FLRange = (valX >= minX && valX <= maxX) && (valY >= minY && valY <= maxY);
  return(strafe_FLRange);
  } // end bool strafe_fl

bool strafe_BL(void) {
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);
  
  const int minY = 0;
  const int maxY = 256;
  const int minX = 0;
  const int maxX = 257;

  const bool strafe_BLRange = (valX >= minX && valX <= maxX) && (valY >= minY && valY <= maxY);
  return(strafe_BLRange);
  } // end bool strafe_bl

bool strafe_FR(void) {
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);
  
  const int minY = 768;
  const int maxY = 1023;
  const int minX = 768;
  const int maxX = 1023;

  const bool strafe_FRRange = (valX >= minX && valX <= maxX) && (valY >= minY && valY <= maxY);
  return(strafe_FRRange);
  } // end bool forward

bool strafe_BR(void) {
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);
  
  const int minY = 768;
  const int maxY = 1023;
  const int minX = 0;
  const int maxX = 256;

  const bool strafe_BRRange = (valX >= minX && valX <= maxX) && (valY >= minY && valY <= maxY);
  return(strafe_BRRange);
  } // end bool forward
/////////////////////////////////////////////////////////////////////////////////////////////
// Main Setup & Loop
void setup() {
  Serial.begin(115200);

  // Set the button pin as an input 
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(btnC, INPUT_PULLUP);
  pinMode(btnD, INPUT_PULLUP);

} // end setup

void loop() {

  //Movement
  Serial.print("Movement: ");
  // Movement State Checks
  if(stop() == true) {
    Serial.print("Stop      |");
  } // end check stop

  if(forward() == true) {
    Serial.print("Forward   |");
  } // end forward check

  if(right() == true) {
    Serial.print("Right     |");
  } // end right check  

  if(backward() == true) {
    Serial.print("Backward  |");
  } // end backward check  

  if(left() == true) {
    Serial.print("Left      |");
  } // end left check 

  if(strafe_FL() == true) {
    Serial.print("Strafe_FL |");
  } // end Strafe_FL check 

  if(strafe_BL() == true) {
    Serial.print("Strafe_BL |");
  } // end Strafe_BL check 

  if(strafe_FR() == true) {
    Serial.print("Strafe_FR |");
  } // end Strafe_FL check 

  if(strafe_BR() == true) {
    Serial.print("Strafe_BR |");
  } // end Strafe_BL check 

////////////////////////////////////////////////////////////////////////////////////////
  // Button Checks
  buttonState.A = (digitalRead(btnA) == LOW);
  Serial.print(" A: ");
  if(buttonState.A) {
    Serial.print(" Y |");
  } // end btn.A read
  else {
    Serial.print(" N |");
  } // end btn.A else

  buttonState.B = (digitalRead(btnB) == LOW);
  Serial.print(" B: ");
  if(buttonState.B) {
    Serial.print(" Y |");
  } // end btn.B read
  else {
    Serial.print(" N |");
  } // end btn.B else

  buttonState.C = (digitalRead(btnC) == LOW);
  Serial.print(" C: ");
  if(buttonState.C) {
    Serial.print(" Y |");
  } // end btn.C read
  else {
    Serial.print(" N |");
  } // end btn.C else

  buttonState.D = (digitalRead(btnD) == LOW);
  Serial.print(" D: ");
  if(buttonState.D) {
    Serial.println(" Y |");
  } // end btn.A read
  else {
    Serial.println(" N |");
  } // end btn.A else


// delay to not bog terminal
delay(100);

} // end loop

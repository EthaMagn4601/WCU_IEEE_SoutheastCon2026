// #include <RP2040Version.h>
// #include <Wire.h>

/*

have certain commmands for moving forward and backward that have array's with bytes 
that structure the individual commands to move in eight motions. Keywords are going to be 
sent over SPI to pass the data, but Leon is still figuring that out. 

need to have WASD, and strafe controls. Either full off or full on. probably need a margin of error for both.


Idea for certain commands:

Create if statements for storing information to variable that could send data. The if statements will be 
in a certain parameter that will trigger the response of certain functions, like Ethan's encoders.

*/
// Define the pins for the joystick
const int joystickX = A0; // X-axis analog pin
const int joystickY = A1; // Y-axis analog pin
const int btn1 = 13; // joystick button
const int btn2 = 12; // green button
const int btn3 = 11; // black button
const int btn4 = 10; // blue button

// Variables for store values
int valX = 0;
int valY = 0; 
bool buttonState = false;

void setup() {
  Serial.begin(115200);

  // Set the button pin as an input 
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);

} // end setup

void loop() {

  // Analog Read
  valX = analogRead(joystickX);
  valY= analogRead(joystickY);

  // Button Check
  buttonState = digitalRead(btn1);
  buttonState = digitalRead(btn2);
  buttonState = digitalRead(btn3);
  buttonState = digitalRead(btn4);

  // Joystick Print
  Serial.print("X: ");
  Serial.print(valX);
  Serial.print(" | Y: ");
  Serial.print(valY);
  Serial.print(" | Button: ");
  
  // Button Print
  if (buttonState) {
    Serial.println(" Pressed");
  } //end if button pressed
  else {
    Serial.println(" Released");
  } //end else not pressed


// delay to not bog terminal
delay(100);

}

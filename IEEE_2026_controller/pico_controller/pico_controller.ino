  #include <RP2040Version.h>

  /* NOTES
  *
  * Have certain commmands for moving forward and backward that have array's with bytes 
  * that structure the individual commands to move in eight motions. Keywords are going to be 
  * sent over SPI to pass the data, but Leon is still figuring that out. 
  *
  * Idea for certain commands:
  *
  * Create if statements for storing information to variable that could send data. The if statements will be 
  * in a certain parameter that will trigger the response of certain functions, like Ethan's encoders.
  *
  */

  // Joystick Pin Definition
  #define joystickX 28 // ADC2__X-axis analog pin
  #define joystickY 27 // ADC1__Y-axis analog pin
  #define btnA 22 // joystick button
  #define btnB 21 // green button
  #define btnC 20 // black button
  #define btnD 19 // blue button
  #define btnE 17 // tiny switch 1
  #define btnF 16 // tiny switch 2

  const uint16_t maxAVal = 1023; // max values for x and y axis of joystick respectively.
  const uint8_t mapDivisions = 11;

  bool turnMode = 0;

  // Variables for store values
  int valX = 0;
  int valY = 0; 
  bool ser_ena = true; // disable if you don't want serial output to terminal (does not mean serial output to other devices)

  //////////////////////////////////////////////////////////////////////////////////////////
  // MOTOR DIRECTION FUNCTIONS
  struct Buttons {         
    bool A; // inverted button logic
    bool B; 
    bool C;
    bool D;
    bool E;
    bool F;
  }; // end buttonState structure

  // Assign Button Pin Locations
  Buttons buttonState = {false, false, false, false, false, false}; 

  /////////////////////////////////////////////////////////////////////////////////////////////
  // Main Setup & Loop
  void setup() {
    Serial.begin(115200);

    // Set the button pin as an input 
    pinMode(btnA, INPUT_PULLUP);
    pinMode(btnB, INPUT_PULLUP);
    pinMode(btnC, INPUT_PULLUP);
    pinMode(btnD, INPUT_PULLUP);
    pinMode(btnE, INPUT_PULLUP);
    pinMode(btnF, INPUT_PULLUP);

  } // end setup

  void loop() {

    while(!digitalRead(btnA)){
      delay(200);
      while(!digitalRead(btnA)){
        delay(1);
      }
      turnMode = !turnMode;
    }

    valY = map(analogRead(joystickY), 0, maxAVal, 0, mapDivisions);
    
    // Debug
    // Serial.println(analogRead(joystickX));
    // Serial.println(analogRead(joystickY));

    switch (map(analogRead(joystickX), 0, maxAVal, 0, mapDivisions)){
      case 0: // We're pushing joystick left
        if(turnMode) {if(ser_ena) Serial.println("Movement: CCW Rotate");} 
        else {if(ser_ena) Serial.println("Movement: Left");}
        break;
      case 1: case 2: case 3: // We're pushing joystick left and (up or down)
        if(valY <= 4) {if(ser_ena){ Serial.println("Movement: Strafe BL");}}
        else {if(ser_ena) Serial.println("Movement: Strafe FL");}
        break;
      case 4: case 5: case 6: // We're pushing joystick straight (up or down) or !pushing
        if(valY <= 3) {if(ser_ena) Serial.println("Movement: Reverse");}
        else if(valY > 6) {if(ser_ena) Serial.println("Movement: Forward");}
        else{if(ser_ena) Serial.println("Movement: Stop");}
        break;
      case 7: case 8: case 9: // We're pushing joystick right and (up or down)
        if(valY <= 4) {if(ser_ena) Serial.println("Movement: Strafe BR");}
        else{if(ser_ena) Serial.println("Movement: FR");}
        break;
      case 10: // We're pushing joystick right
        if(turnMode) {if(ser_ena) Serial.println("Movement: CW Rotate");}
        else {if(ser_ena) Serial.println("Movement: ");}
        break;
      default:
        if(ser_ena) Serial.println("Movement: Stop");
        break;
    }

  // delay to not bog terminal
  delay(10);

  } // end loop

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
  const uint8_t mapDivisions = 31;
  const uint8_t upXThresh = 26;
  const uint8_t downXThresh = 11;

  bool turnMode = 0;

  // Variables for store values
  int valX = 0;

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

  /////////////////////
  // Encoding Scheme //
  /////////////////////

  uint8_t m_state = 0x0;

  uint8_t m_move[11] = {
    0x0, // Stop
    0x1, // Strafe FL
    0x2, // Strafe BR
    0x3, // Strafe BL
    0x4, // Strafe FR
    0x5, // Left
    0x6, // Right
    0x7, // Forward
    0x8, // Backward
    0x9, // CCW Rotate
    0xA // CW Rotate
  };

  // Assign Button Pin Locations
  Buttons buttonState = {false, false, false, false, false, false}; 

  /////////////////////////////////////////////////////////////////////////////////////////////
  // Main Setup & Loop
  
  bool ser_ena = true; // disable if you don't want serial output to terminal (does not mean serial output to other devices)
  
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

    valX = map(analogRead(joystickX), 0, maxAVal, 0, mapDivisions);
    
    // Debug
    if(ser_ena){
      Serial.print("Analog ValX: ");
      Serial.println(analogRead(joystickX));
      Serial.print("Map ValX: ");
      Serial.println(valX);
      Serial.print("Analog ValY: ");
      Serial.println(analogRead(joystickY));
      Serial.print("Map ValY: ");
      Serial.println(map(analogRead(joystickY), 0, maxAVal, 0, mapDivisions));
    }

    switch (map(analogRead(joystickY), 0, maxAVal, 0, mapDivisions)){
      case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: 
        if(valX >= upXThresh){m_state = m_move[1]; if(ser_ena) Serial.println("Movement: Strafe FL");}
        else if(valX <= downXThresh){m_state = m_move[3]; if(ser_ena) Serial.println("Movement: Strafe BL");}
        else{if(turnMode){m_state = m_move[9]; if(ser_ena) Serial.println("Movement: CCW Rotate");} else{m_state = m_move[5]; if(ser_ena) Serial.println("Movement: Left");}}
      break;
      case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: 
      case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23:
        if(valX >= upXThresh){m_state = m_move[7]; if(ser_ena) Serial.println("Movement: Forward");}
        else if(valX <= downXThresh){m_state = m_move[8]; if(ser_ena) Serial.println("Movement: Backward");}
        else{m_state = m_move[0]; if(ser_ena) Serial.println("Movement: Stop");}
      break;
      case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31: 
        if(valX >= upXThresh){m_state = m_move[4]; if(ser_ena) Serial.println("Movement: Strafe FR");}
        else if(valX <= downXThresh){m_state = m_move[2]; if(ser_ena) Serial.println("Movement: Strafe BR");}
        else{if(turnMode){m_state = m_move[10]; if(ser_ena) Serial.println("Movement: CW Rotate");} else{m_state = m_move[6]; if(ser_ena) Serial.println("Movement: Right");}}
      break;
      default:
        m_state = m_move[0]; if(ser_ena) Serial.println("Movement: Stop");
      break;
    }
  
  // Debug
  if(ser_ena){
    Serial.print("m_state: ");
    Serial.println(m_state);
    // delay to not bog terminal
    delay(200);
  }

} // end loop

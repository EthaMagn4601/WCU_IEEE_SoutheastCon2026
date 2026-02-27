// IEEE team coders 2026: Leon Mueller; Jordan Fox; Ethan Magnante


//change pwm to just be a global speed control and use a map function
//
//


#include <EEPROM.h>

// ================= Encoder pins =================
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

// For an encoder count of 629 positions, the robot moved 13.25 in.
#define ENC_POS_PER_INCH 47.4716981132f  

// ================= PWM Helpers =================
#define PWM_FREQ 20000      // 20 kHz
#define PWM_WRAP 1000       // Resolution

// Motor 1 (Front Right)
#define MOTOR_IN1 7
#define MOTOR_IN2 8

// Motor 2 (Back Right)
#define MOTOR_IN3 12
#define MOTOR_IN4 13

// Motor 3 (Front Left)
#define MOTOR_IN5 16
#define MOTOR_IN6 17

// Motor 4 (Rear Left)
#define MOTOR_IN7 18
#define MOTOR_IN8 19

#define MOTOR_PWM 6   // Shared PWM enable

#define m_stop         0b11111111
#define m_forward      0b10101010
#define m_backward     0b01010101
#define m_strafe_fl    0b10011001
#define m_strafe_br    0b01100110
#define m_strafe_bl    0b01101001
#define m_strafe_fr    0b10010110
#define m_left         0b01100110
#define m_right        0b10011001
#define m_ccw_rotate   0b01011010
#define m_cw_rotate    0b10100101

#define eeprom_size 1024 //1024 bytes we are using 3 bytes per position, so about 340 position changes

volatile int32_t encoder_pos = 0;

bool remote_flag = 0; //serial communication flag
uint8_t remote_input = 0; //remote dataword

uint8_t memory_movement[350] = {}; //reserving ram by defining expected size
uint16_t memory_time[350] = {};
uint16_t memory_changes = 0;
uint16_t total_mem_changes = 0;
uint16_t stop_stamp = 0;

bool repeat_flag = 0;

bool serial_enable = 1;

uint8_t motor_command[11] = {
    m_stop, m_strafe_fl, m_strafe_br, m_strafe_bl, m_strafe_fr,
    m_left, m_right, m_cw_rotate, m_ccw_rotate, m_backward, m_forward
  };

uint8_t motor_enable[8] = {
    MOTOR_IN1, MOTOR_IN2,
    MOTOR_IN3, MOTOR_IN4,
    MOTOR_IN5, MOTOR_IN6,
    MOTOR_IN7, MOTOR_IN8
  };

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  //while (!Serial) delay(10);  // wait for serial port to open

  // ----- Encoder -----
  pinMode(ENCODER_PIN_A, INPUT_PULLDOWN);
  pinMode(ENCODER_PIN_B, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderISR, RISING);
  //noInterrupts();
  //interrupts();
  
  // ----- Motor direction pins -----
  for (int i = 0; i < 8; i++) {
    pinMode(motor_enable[i], OUTPUT);
    digitalWrite(motor_enable[i], LOW);
  }

  // ----- PWM -----
  pinMode(MOTOR_PWM, OUTPUT);
  analogWriteFreq(PWM_FREQ);
  analogWriteRange(PWM_WRAP);
  analogWrite(MOTOR_PWM, 0);

  uint8_t motor_speed = 20;  // percent
  pwm_set_duty(MOTOR_PWM, motor_speed);

  EEPROM.begin(eeprom_size);
}

void loop() {
  if(Serial1.available()) {uint8_t received = Serial1.read(); uint8_t movement_index = received >> 4; motorControl(motor_command[movement_index]);}
  //motorControl(motor_command[parsed_bit]);
  // if (remote_flag) {
  //   remote_flag = 0;
  //   if(serial_enable) Serial.println(remote_input);

  //   motorControl(motor_command[remote_input >> 4]); //1 of 16 different movement calls, defined within motor_command array

  //   switch (remote_input & 0b1111) { // 16 postions for special functions
  //     case 0: // regular movement saving
  //       memory_movement[memory_changes] = remote_input;
  //       memory_time[memory_changes] = customTime();
  //       if(serial_enable) Serial.print("regular operation, movement change number: ");
  //       if(serial_enable) Serial.println(memory_changes);
  //       memory_changes++;
  //       break;
  //     case 1: //setting repeat flag
  //       repeat_flag = 1;
  //       stop_stamp = customTime();
  //       total_mem_changes = memory_changes;
  //       memory_changes = 0;
  //       break;
  //     case 2: //setting reference location
  //       // statements
  //       break;
  //     case 3: //stop time counter
  //       // statements
  //       break;
  //     case 4: //function call
  //       // statements
  //       break;
  //     case 5: //moving up in menu
  //       // statements
  //       break;
  //     case 6: //moving down in menu
  //       // statements
  //       break;
  //     case 7: //selecting in menu
  //       // statements
  //       break;
  //     default:
  //       break;
  //   } 
  // }

  // if (repeat_flag){
  //   if (memory_time[memory_changes] < customTime() - stop_stamp){
  //     memory_changes++;
  //     motorControl(motor_command[memory_movement[memory_changes] >> 4]);
  //   }
  // }
}


void serialEvent() {
  while (Serial1.available()) {
    //will read one incoming byte
    remote_input = Serial1.read();
    if(serial_enable) Serial.println("we have received serial");
    if(serial_enable) Serial.println(remote_input, BIN);
    if(!repeat_flag) remote_flag = 1; //don't accept remote when vehicle is repeating operation
  }
}

void showError(){
  //some code, some LED, that shows errors
}

void pwm_set_duty(uint pin, uint duty_percent) {
  if (duty_percent > 100) duty_percent = 100;
  uint16_t level = (duty_percent * PWM_WRAP) / 100;
  analogWrite(pin, level);
}

// ================= Encoder Interrupt =================
void encoderISR() {
  if (digitalRead(ENCODER_PIN_B))
    encoder_pos++;
  else
    encoder_pos--;
}

void motorControl (uint8_t motor_action) { // 0b00000000
  for (byte z = 0; z < 8; z++){
    digitalWrite(motor_enable[z], motor_action >> z & 1);
  }
  //if(serial_enable) Serial.print("now executing movement: ");
  //if(serial_enable) Serial.println(motor_action);
}

uint16_t customTime(){ //divide a second into 32 steps to shrink storage space
  return uint16_t(millis() / 31.25);
}

bool commitEEPROM(){
  EEPROM.write(0, uint8_t(total_mem_changes >> 8)); //stores the first eight
  EEPROM.write(1, uint8_t(total_mem_changes & 0b11111111)); //stores the last eight
  
  for(uint16_t z = 0; z < total_mem_changes; z++){
    unsigned int e = (z * 3) + 10; //reserve spaces from 0 to 9 for other variables.
    EEPROM.write(e, uint8_t(memory_time[z] >> 8)); //stores the first eight
    EEPROM.write(e+1, uint8_t(memory_time[z] & 0b11111111)); //stores the last eight
    EEPROM.write(e+2, memory_movement[z]);
  }
  return EEPROM.commit(); //this will write to the EEPROM permanantly. return value shows successful writing
}
void loadEEPROM(){
  total_mem_changes = EEPROM.read(0) << 8; //stores the first eight
  total_mem_changes = total_mem_changes | EEPROM.read(1); //stores the last eight
  
  for(uint16_t z = 0; z < total_mem_changes; z++){
    unsigned int e = (z * 3) + 10; //reserve spaces from 0 to 9 for other variables.
    memory_time[z] = EEPROM.read(e) << 8; //stores the first eight
    memory_time[z] = total_mem_changes | EEPROM.read(e+1); //stores the last eight
    memory_movement[z] = EEPROM.read(e+2);
  }
}
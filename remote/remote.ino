// IEEE team coders 2026: Leon Mueller; Jordan Fox; Ethan Magnante


#include <EEPROM.h>


// Motor 1 (Front Right)
#define MOTOR_IN1 4
#define MOTOR_IN2 5

// Motor 2 (Back Right)
#define MOTOR_IN3 12
#define MOTOR_IN4 13

// Motor 3 (Front Left)
#define MOTOR_IN5 16
#define MOTOR_IN6 17

// Motor 4 (Rear Left)
#define MOTOR_IN7 18
#define MOTOR_IN8 19

#define motor_forward 0b10100101
#define motor_reverse 0b01011010
#define motor_r_strave 0b01100110
#define motor_l_strave 0b10011001
#define motor_cw_rotate 0b10101010
#define motor_ccw_rotate 0b01010101
#define motor_coast 0b00000000
#define motor_stop 0b11111111

#define eeprom_size 1024 //1024 bytes we are using 3 bytes per position, so about 340 position changes

bool remote_flag = 0; //serial communication flag
uint8_t remote_input = 0; //remote dataword

uint8_t memory_movement[350] = {};
uint16_t memory_time[350] = {};
uint16_t memory_changes = 0;
uint16_t total_mem_changes = 0;
uint16_t stop_stamp = 0;

bool repeat_flag = 0;

uint8_t motor_command[8] = {
    motor_forward, motor_reverse, motor_r_strave, motor_l_strave,
    motor_cw_rotate, motor_ccw_rotate, motor_coast, motor_stop
  };

uint8_t motor_enable[8] = {
    MOTOR_IN1, MOTOR_IN2,
    MOTOR_IN3, MOTOR_IN4,
    MOTOR_IN5, MOTOR_IN6,
    MOTOR_IN7, MOTOR_IN8
  };


void motorControl (uint8_t motor_action) { // 0b00000000
  for (byte z = 0; z < 8; z++){
    digitalWrite(motor_enable[z], motor_action >> z & 1);
  }
}


void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);  // wait for serial port to open

  EEPROM.begin(eeprom_size);

}

void loop() {

 if (remote_flag) {
    remote_flag = 0;
    motorControl(motor_command[remote_input >> 4]);
    //other functions using the last 4 bits
    
    memory_movement[memory_changes] = remote_input;
    memory_time[memory_changes] = millis();
    memory_changes++;


    repeat_flag = remote_input & 1; //uses last bit to control repeat action.

    if((repeat_flag)){
      
      stop_stamp = millis();
      total_mem_changes = memory_changes;
      memory_changes = 0;
    }

  }

  if (repeat_flag){
    if (memory_time[memory_changes] < millis() - stop_stamp){
      memory_changes++;
      motorControl(motor_command[memory_movement[memory_changes] >> 4]);
    }

  }

  // for (byte z = 0; z < 8; z++){
  //   Motorcontrol (motor_command[z]);
  //   Serial.print("now executing movement: ");
  //   Serial.println(z);
  //   delay(2000);
  // }
  // delay(5000);
}

void serialEvent() {
  while (Serial.available()) {
    //will read one incoming byte
    remote_input = Serial.read();
    if(!repeat_flag) remote_flag = 1; //don't accept remote when vehicle is repeating operation
  }
}

void showError(){
  //some code, some LED, that shows errors
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
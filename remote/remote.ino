



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

#define motor_foward 0b10100101
#define motor_reverse 0b01011010
#define motor_rstrave 0b01100110
#define motor_lstrave 0b10011001
#define motor_cwrotate 0b10101010
#define motor_ccwrotate 0b01010101
#define motor_coast 0b00000000
#define motor_stop 0b11111111

bool remote_flag = 0; //serial communication flag
uint8_t remote_input = 0; //remote dataword

uint8_t memory_movement[] = {};
unsigned int memory_time[] = {};
unsigned int memory_changes = 0;
unsigned int total_mem_changes = 0;
unsigned int stop_stamp = 0;

bool repeat_flag = 0;

uint8_t motor_command[8] = {
    motor_foward, motor_reverse, motor_rstrave, motor_lstrave,
    motor_cwrotate, motor_ccwrotate, motor_coast, motor_stop
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
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) delay(10);  // wait for serial port to open

}

void loop() {

 if (remote_flag) {
    remote_flag = 0;
    //remote_input
    motorControl(motor_command[remote_input >> 4]);
    millis();
    memory_movement[memory_changes] = {};
    memory_time[memory_changes] = {};
    memory_changes++;

    if(("repeat flag is set")){
      repeat_flag = 1;
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
    bool remote_flag = 1;
  }
}

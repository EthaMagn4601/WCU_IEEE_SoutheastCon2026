// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// DO NOT USE PINS 7 OR 8 — GPIO DRIVE ISSUES ON PICO !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// ================= Encoder pins =================
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3

// ================= Motor control pins =================
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

// Shared PWM enable
#define MOTOR_PWM 6   // PWM pin

unsigned long lastPrintMs = 0;
const unsigned long PRINT_INTERVAL_MS = 200;  // 5 Hz print rate

volatile int32_t encoder_pos = 0;

// For an encoder count of 629 positions, the robot moved 13.25 in.

#define ENC_POS_PER_INCH 47.4716981132f  

// ================= Encoder Interrupt =================
void encoderISR() {
  bool state_b = digitalRead(ENCODER_PIN_B);
  if (state_b)
    encoder_pos--;
  else
    encoder_pos++;
}

// ================= PWM Helpers =================
const uint32_t PWM_FREQ = 20000;      // 20 kHz
const uint16_t PWM_WRAP = 1000;       // Resolution

void pwm_init_motor(uint pin) {
  pinMode(pin, OUTPUT);
  analogWriteFreq(PWM_FREQ);
  analogWriteRange(PWM_WRAP);
  analogWrite(pin, 0);
}

void pwm_set_duty(uint pin, uint duty_percent) {
  if (duty_percent > 100) duty_percent = 100;
  uint16_t level = (duty_percent * PWM_WRAP) / 100;
  analogWrite(pin, level);
}

// ================= Motor Helpers =================
void motors_forward(void) {
  digitalWrite(MOTOR_IN1, HIGH); digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, HIGH); digitalWrite(MOTOR_IN4, LOW);
  digitalWrite(MOTOR_IN5, LOW);  digitalWrite(MOTOR_IN6, HIGH);
  digitalWrite(MOTOR_IN7, LOW);  digitalWrite(MOTOR_IN8, HIGH);
}

void motors_reverse(void) {
  digitalWrite(MOTOR_IN1, LOW);  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, LOW);  digitalWrite(MOTOR_IN4, HIGH);
  digitalWrite(MOTOR_IN5, HIGH); digitalWrite(MOTOR_IN6, LOW);
  digitalWrite(MOTOR_IN7, HIGH); digitalWrite(MOTOR_IN8, LOW);
}

void motors_strafe_left(void) {
  digitalWrite(MOTOR_IN1, HIGH); digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);  digitalWrite(MOTOR_IN4, HIGH);
  digitalWrite(MOTOR_IN5, HIGH); digitalWrite(MOTOR_IN6, LOW);
  digitalWrite(MOTOR_IN7, LOW);  digitalWrite(MOTOR_IN8, HIGH);
}

void motors_strafe_right(void) {
  digitalWrite(MOTOR_IN1, LOW);  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, HIGH); digitalWrite(MOTOR_IN4, LOW);
  digitalWrite(MOTOR_IN5, LOW);  digitalWrite(MOTOR_IN6, HIGH);
  digitalWrite(MOTOR_IN7, HIGH); digitalWrite(MOTOR_IN8, LOW);
}

void motors_rotate_cw(void) {
  digitalWrite(MOTOR_IN1, HIGH); digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, HIGH); digitalWrite(MOTOR_IN4, LOW);
  digitalWrite(MOTOR_IN5, HIGH); digitalWrite(MOTOR_IN6, LOW);
  digitalWrite(MOTOR_IN7, HIGH); digitalWrite(MOTOR_IN8, LOW);
}

void motors_rotate_ccw(void) {
  digitalWrite(MOTOR_IN1, LOW);  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, LOW);  digitalWrite(MOTOR_IN4, HIGH);
  digitalWrite(MOTOR_IN5, LOW);  digitalWrite(MOTOR_IN6, HIGH);
  digitalWrite(MOTOR_IN7, LOW);  digitalWrite(MOTOR_IN8, HIGH);
}

void motors_stop(void) {
  digitalWrite(MOTOR_IN1, HIGH); digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, HIGH); digitalWrite(MOTOR_IN4, HIGH);
  digitalWrite(MOTOR_IN5, HIGH); digitalWrite(MOTOR_IN6, HIGH);
  digitalWrite(MOTOR_IN7, HIGH); digitalWrite(MOTOR_IN8, HIGH);
}

// ================= Setup =================
void setup() {
  Serial.begin(115200);

  // ----- Encoder -----
  pinMode(ENCODER_PIN_A, INPUT_PULLDOWN);
  pinMode(ENCODER_PIN_B, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderISR, RISING);

  // ----- Motor direction pins -----
  uint8_t motor_pins[] = {
    MOTOR_IN1, MOTOR_IN2,
    MOTOR_IN3, MOTOR_IN4,
    MOTOR_IN5, MOTOR_IN6,
    MOTOR_IN7, MOTOR_IN8
  };

  for (int i = 0; i < 8; i++) {
    pinMode(motor_pins[i], OUTPUT);
    digitalWrite(motor_pins[i], LOW);
  }

  // ----- PWM -----
  pwm_init_motor(MOTOR_PWM);
}

// ================= Loop =================
void loop() {
  
//   delay(5000);

  uint8_t motor_speed = 20;  // percent
  pwm_set_duty(MOTOR_PWM, motor_speed);

//   motors_forward();
//   delay(1000);
//   motors_stop();
//   while (true){
//       // ----- Encoder printout -----
//     if (millis() - lastPrintMs >= PRINT_INTERVAL_MS) {
//       lastPrintMs = millis();

//       noInterrupts();                 // atomic read
//       int32_t enc = encoder_pos;
//       interrupts();

//       Serial.print("Encoder count: ");
//       Serial.println(enc);
//   }
// }
// I'm changing this to do things
  motors_strafe_right();
  delay(2000);

  motors_stop();
  delay(1000);

  motors_reverse();
  delay(1000);
  motors_strafe_right();
  delay(250);
  motors_reverse();
  delay(1000);
  motors_strafe_right();
  delay(250);
  motors_reverse();
  delay(1000);

  motors_stop();
  delay(10000);
}

#include <RP2040Version.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>

///////////////////////////////////////////////////////////////////////////////////////////
// Declare the Two Sensors
Adafruit_VL53L0X loxA = Adafruit_VL53L0X(); // Leftmost sensor
Adafruit_VL53L0X loxB = Adafruit_VL53L0X(); // Second leftmost sensor
Adafruit_VL53L0X loxC = Adafruit_VL53L0X(); // Second rightmost sensor
Adafruit_VL53L0X loxD = Adafruit_VL53L0X(); // Rightmost sensor

// XSHUT Pins
const uint8_t XSHUT_A = 2;
const uint8_t XSHUT_B = 3;
const uint8_t XSHUT_C = 4;
const uint8_t XSHUT_D = 5;

// New I2C Addresses (7-bit)
// NOTE THAT THE ORIGINAL I2C ADDRESS IS 0X29
const uint8_t ADDR_A = 0x30;
const uint8_t ADDR_B = 0x31;
const uint8_t ADDR_C = 0x32;
const uint8_t ADDR_D = 0x33;

/////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // I2C pins
  Wire.setSDA(12);
  Wire.setSCL(13);
  Wire.begin();

  // Configue XSHUT Pins
  pinMode(XSHUT_A, OUTPUT);
  pinMode(XSHUT_B, OUTPUT);
  pinMode(XSHUT_C, OUTPUT);
  pinMode(XSHUT_D, OUTPUT);
  delay(10);

  // Turn both off sensors
  Serial.println("Initializing VL53L0X sensors A,B,C,D");

  // Bring up Sensor A
  digitalWrite(XSHUT_A, HIGH);
  delay(10); // allow boot time
  
  if (!loxA.begin(ADDR_A)) { //Set address to sensor A
    Serial.println("Failed to boot Sensor A");
    while(1) delay (10);
  } // end boot A
  Serial.println("Sensor A online at 0x30");

  // Bring up Sensor B
  digitalWrite(XSHUT_B, HIGH);
  delay(10);

  if(!loxB.begin(ADDR_B)) {
    Serial.println("Failed to boot Sensor B");
    while(1) delay(10);
  } // end boot B
  Serial.println("Sensor B online at 0x31");

  // Bring up Sensor C
  digitalWrite(XSHUT_C, HIGH);
  delay(10); // allow boot time
  
  if (!loxC.begin(ADDR_C)) { //Set address to sensor C
    Serial.println("Failed to boot Sensor C");
    while(1) delay (10);
  } // end boot C
  Serial.println("Sensor C online at 0x32");

  // Bring up Sensor D
  digitalWrite(XSHUT_D, HIGH);
  delay(10); // allow boot time
  
  if (!loxD.begin(ADDR_D)) { //Set address to sensor D
    Serial.println("Failed to boot Sensor D");
    while(1) delay (10);
  } // end boot D
  Serial.println("Sensor D online at 0x33");

  // Serial.println("Both Sensors initialized");
  Serial.println("All Sensors Online");

  // Set timing to increase the accuracy by lowering standard deviation of measurements
  loxA.setMeasurementTimingBudgetMicroSeconds(200000);
  loxB.setMeasurementTimingBudgetMicroSeconds(200000);
  loxC.setMeasurementTimingBudgetMicroSeconds(200000);
  loxD.setMeasurementTimingBudgetMicroSeconds(200000);

} // end void setup
/////////////////////////////////////////////////////////////////////////////////////
void loop() {

  VL53L0X_RangingMeasurementData_t measureA;
  VL53L0X_RangingMeasurementData_t measureB;
  VL53L0X_RangingMeasurementData_t measureC;
  VL53L0X_RangingMeasurementData_t measureD;

  // Begin measuring
  loxA.rangingTest(&measureA, false); // false = boolean debug
  delay(20);
  loxB.rangingTest(&measureB, false);
  delay(20);
  loxC.rangingTest(&measureC, false);
  delay(20);
  loxD.rangingTest(&measureD, false);
  delay(20);

  Serial.print(" | A: ");
  if (measureA.RangeStatus != 4) {
    float mmA = measureB.RangeMilliMeter;
    Serial.print(mmA);
    Serial.print(" mm");
  } else {
    Serial.print("O.O.R");
  }
  // B Measurement & Serial Print
  Serial.print(" | B: ");
  if (measureB.RangeStatus != 4) {
    float mmB = measureB.RangeMilliMeter;
    Serial.print(mmB);
    Serial.print(" mm");
  } else {
    Serial.print("O.O.R");
  }
    // C Measurement & Serial Print
  Serial.print(" | C: ");
  if (measureC.RangeStatus != 4) {
    float mmC = measureB.RangeMilliMeter;
    Serial.print(mmC);
    Serial.print(" mm");
  } else {
    Serial.print("O.O.R");
  }
    // D Measurement & Serial Print
  Serial.print(" | D: ");
  if (measureD.RangeStatus != 4) {
    float mmD = measureB.RangeMilliMeter;
    Serial.print(mmD);
    Serial.print(" mm");
  } else {
    Serial.print("O.O.R");
  }
  Serial.println();
  delay(100);
} // end void loop
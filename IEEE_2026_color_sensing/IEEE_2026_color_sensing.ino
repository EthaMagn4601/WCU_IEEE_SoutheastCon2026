#include <Wire.h>
#include <U8g2lib.h>

// Note, need to drive with 5v

// LED SDA and SCL designators, crucial for screen to work
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

const int S0 = 4;
const int S1 = 5;
const int S2 = 6;
const int S3 = 7;
const int OUT_PIN = 8;

// for '-' pin module, common cathode is typical
const bool COMMON_ANODE = false;

// Calibration storage
unsigned long whiteR = 0, whiteG = 0, whiteB = 0;
unsigned long blackR = 0, blackG = 0, blackB = 0;
bool hasWhite = false, hasBlack = false;

// Helpers
int max3(int a, int b, int c) {return max(a, max(b,c)); }
int min3(int a, int b, int c) {return min(a, min(b,c)); }

void setRgbLed(int r, int g, int b) {
  r = constrain(r, 0, 255);
  g = constrain(r, 0, 255);
  b = constrain(r, 0, 255);

  // if(COMMON_ANODE) {
  //   analogWrite(LED_R, 255 - r);
  //   analogWrite(LED_G, 255 - g);
  //   analogWrite(LED_B, 255 - b);
  // } else {
  //   analogWrite(LED_R, r);
  //   analogWrite(LED_G, g);
  //   analogWrite(LED_B, b);
  // }
} // end setRgbLed 


void showColorOnLed(const String &label) {
  if (label == "WHITE") setRgbLed(255,255,255);
  else if (label == "BLACK") setRgbLed(0,0,0);

  else if (label == "RED") setRgbLed(255,0,0);
  else if (label == "GREEN") setRgbLed(0,255,0);
  else if (label == "BLUE") setRgbLed(0,0,255);

  else if (label == "YELLOW") setRgbLed(255,180,0);
  else if (label == "ORANGE") setRgbLed(255,60,0);

  else if (label == "PURPLE") setRgbLed(180,0,255);

  else setRgbLed(0,0,0); // UNKOWN => off

} // end showColorOnLed 

//========================================
// Sensor Reading

unsigned long readDuration(bool s2, bool s3) {
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  delay(50);

  unsigned long d = pulseIn(OUT_PIN, LOW, 100000);
  if (d == 0) d = 100000;
  return d;

} // end read duration

void readRaw(unsigned long &r, unsigned long &g, unsigned long &b) {
  r = readDuration(LOW, HIGH);  // READ
  g = readDuration(HIGH, HIGH); // GREEN
  b = readDuration(LOW, HIGH);  // BLUE
} // end readRaw RGB values

int normalize(unsigned long val, unsigned long blackVal, unsigned long whiteVal) {
  if (whiteVal == blackVal) return 0;

  long x = (long)val;
  long b = (long)blackVal;
  long w = (long)whiteVal;

  if(x < w) x = w;
  if(x > b) x = b;

  long out = (b - x) * 255L / (b - w);
  if (out < 0) out = 0;
  if (out > 255) out = 255;
  return (int)out;
}  // end normalize values

//=======================================
// Color Classification

String detectColor(int R, int G, int B) {
  int mx = max3(R, G, B);
  int mn = min3(R, G, B);
  int spread = mx - mn;
  int sum = R + G + B;

  if(sum < 140) return "BLACK/DARK";
  if(sum > 620 && spread < 35) return "WHITE";
  if(spread < 25) return "GRAY";

  const int dom = 30;
  const int rgClose = 35;

  //Yellow/Orange First
  if (B + 35 < min(R, G)) {
    if (abs(R - G) <= rgClose) return "YELLOW";
    if (R > G + 20) return "ORANGE";
    return "YELLOW";
  } // end if yellow/orange

  if(R > G + 35 && B > G + 35) return "PURPLE";
  if(R > G + dom && R > B + dom) return "RED";
  if(G > R + dom && G > B + dom) return "GREEN";
  if(B > R + dom && B > G + dom) return "BLUE";

  return "UNKNOWN";
} // end String detectColor


void setup() {
  Serial.begin(9600);
  // u8g2.begin();

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT_PIN, OUTPUT);
  // 20% Scaling
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.println("TSC3200 Hard Coded Colors");
  Serial.println("Send 'w' for WHITE, 'k' for BLACK calibration");

} // end setup

void loop() {

  //Calibration Keys
  if(Serial.available()) {
    char c = Serial.read();

    if(c == 'w') {
      readRaw(whiteR, whiteG, whiteB);
      hasWhite = true;
      Serial.print("WHITE captured (raw):  ");
      Serial.print(whiteR);
      Serial.print(", ");
      Serial.print(whiteG);
      Serial.print(", ");
      Serial.print(whiteB);
    }// end w check

    if(c == 'k') {
    readRaw(blackR, blackG, blackG);
    hasWhite = true;
    Serial.print("BLACK captured (raw):  ");
    Serial.print(blackR);
    Serial.print(", ");
    Serial.print(blackG);
    Serial.print(", ");
    Serial.print(blackB);
    }// end w check
  }// end black or white check

  //Read Sensor
  unsigned long rawR, rawG, rawB;
  readRaw(rawR, rawG, rawB);

  if(hasWhite && hasBlack) {
    int R = normalize(rawR, blackR, whiteR);
    int G = normalize(rawG, blackG, whiteG);
    int B = normalize(rawB, blackB, whiteB);

    String label = detectColor(R, G, B);

    showColorOnLed(label);

    Serial.print("RGB: ");
    Serial.print(R);
    Serial.print(", ");
    Serial.print(G);
    Serial.print(", ");
    Serial.print(B);
    Serial.print("  ->");
    Serial.println(label);
  } else {

    setRgbLed(0, 0, 0);
    Serial.print("RAW (calibrate first): ");
    Serial.print(rawR);
    Serial.print(", ");
    Serial.print(rawG);
    Serial.print(", ");
    Serial.println(rawB);
    Serial.println("Send 'w' for WHITE and 'k' for BLACK.");

  } // end else

  delay(150);



  // u8g2.setFontMode(1);
  // u8g2.setBitmapMode(1);
  // u8g2.drawFrame(1, 0, 126, 62);

  // u8g2.setFont(u8g2_font_timR18_tr);
  // u8g2.drawStr(3, 45, "LED 1: ");

  // u8g2.setFont(u8g2_font_t0_17b_tr);
  // u8g2.drawStr(6, 14, "GO CATAMOUNTS");

  // u8g2.drawXBM(3, 45, 116, 5, image_Layer_4_bits);

  // u8g2.sendBuffer();



}





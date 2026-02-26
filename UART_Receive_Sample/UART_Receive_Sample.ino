void setup() {
  Serial.begin(9600);   // USB monitor
  Serial1.begin(9600);  // UART pins
}

void loop() {
  if(Serial1.available()) {Serial.println(Serial1.read());}
}
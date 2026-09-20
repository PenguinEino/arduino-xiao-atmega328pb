void setup() {
  Serial.begin(115200);  // USB FT232 UART0
  Serial1.begin(9600);   // D6=TX, D7=RX
}
void loop() {
  if (Serial.available()) Serial1.write(Serial.read());
  if (Serial1.available()) Serial.write(Serial1.read());
}

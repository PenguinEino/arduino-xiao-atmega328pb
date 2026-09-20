// The fitted user LED is active-low on PD5.
void setup() { pinMode(LED_BUILTIN, OUTPUT); }
void loop() {
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ACTIVE); delay(500);
  digitalWrite(LED_BUILTIN, !LED_BUILTIN_ACTIVE); delay(500);
}

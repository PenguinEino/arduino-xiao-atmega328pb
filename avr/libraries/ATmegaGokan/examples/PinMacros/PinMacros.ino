#include <Wire.h>
#include <SPI.h>
static_assert(PIN_WIRE0_SDA == D4 && PIN_WIRE0_SCL == D5, "I2C pin map");
static_assert(PIN_SERIAL1_TX == D6 && PIN_SERIAL1_RX == D7, "UART pin map");
static_assert(PIN_SPI_SCK == D8 && PIN_SPI_MISO == D9 && PIN_SPI_MOSI == D10, "SPI pin map");
void setup() {
  Serial.begin(115200);
  Wire.begin(); // I2C0: D4/D5
  SPI.begin();  // SPI1: D8/D9/D10; D0 becomes hardware SS output
  Serial.println(PIN_WIRE0_SDA);
  Serial.println(analogRead(A9)); // D9 / PC0 / ADC0
}
void loop() {}

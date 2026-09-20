#include <Arduino.h>
#include <Wire.h>
#include <Wire1.h>
#include <SPI.h>
#include <SPI1.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

static_assert(D0 == 0 && D10 == 10, "Header numbering");
static_assert(PIN_PE2 == D0 && PIN_PD2 == D1 && PIN_PD3 == D2 && PIN_PB1 == D3, "D0-D3");
static_assert(PIN_PC4 == D4 && PIN_PC5 == D5 && PIN_PB3 == D6 && PIN_PB4 == D7, "D4-D7");
static_assert(PIN_PC1 == D8 && PIN_PC0 == D9 && PIN_PE3 == D10, "D8-D10");
static_assert(PIN_WIRE0_SDA == D4 && PIN_WIRE_SDA0 == D4 && SDA == D4, "SDA");
static_assert(PIN_WIRE0_SCL == D5 && PIN_WIRE_SCL0 == D5 && SCL == D5, "SCL");
static_assert(PIN_SERIAL1_TX == D6 && PIN_SERIAL1_RX == D7, "UART1");
static_assert(PIN_SERIAL0_RX == PIN_PD0 && PIN_SERIAL0_TX == PIN_PD1, "UART0");
static_assert(SS == D0 && MOSI == D10 && MISO == D9 && SCK == D8, "Default SPI1");
static_assert(SS1 == SS && MOSI1 == MOSI && MISO1 == MISO && SCK1 == SCK, "SPI1 aliases");
static_assert(PIN_SPI0_MOSI == D6 && PIN_SPI0_MISO == D7, "SPI0 shared with UART1");
static_assert(digitalPinToInterrupt(D1) == 0 && digitalPinToInterrupt(D2) == 1, "EXTINT");
static_assert(digitalPinToInterrupt(D0) == NOT_AN_INTERRUPT, "No false EXTINT");
static_assert(analogPinToChannel(A0) == 6 && analogPinToChannel(A4) == 4 && analogPinToChannel(A5) == 5, "ADC map");
static_assert(analogPinToChannel(A8) == 1 && analogPinToChannel(A9) == 0 && analogPinToChannel(A10) == 7, "ADC map");
static_assert(analogPinToChannel(D1) == -1 && analogPinToChannel(D2) == -1 && analogPinToChannel(D3) == -1, "No fictitious ADC");
static_assert(LED_BUILTIN == PIN_PD5 && LED_BUILTIN_ACTIVE == LOW, "User LED");
#define ADC_ROUNDTRIP(n) static_assert(analogPinToChannel(analogInputToDigitalPin(n)) == n, "ADC round trip")
ADC_ROUNDTRIP(0); ADC_ROUNDTRIP(1); ADC_ROUNDTRIP(2); ADC_ROUNDTRIP(3);
ADC_ROUNDTRIP(4); ADC_ROUNDTRIP(5); ADC_ROUNDTRIP(6); ADC_ROUNDTRIP(7);
static_assert(digitalPinHasPWM(D1) && digitalPinHasPWM(D2) && digitalPinHasPWM(D3) && digitalPinHasPWM(D6), "Header PWM");
static_assert(!digitalPinHasPWM(D0) && !digitalPinHasPWM(D4) && !digitalPinHasPWM(D5) && !digitalPinHasPWM(D7) && !digitalPinHasPWM(D8) && !digitalPinHasPWM(D9) && !digitalPinHasPWM(D10), "Non-PWM header");

SoftwareSerial soft(D1, D2);
volatile uint8_t observed;
void irq() { observed++; }
void setup() {
  Serial.begin(115200); Serial1.begin(9600); soft.begin(9600);
  Wire.begin(); Wire1.begin(); SPI.begin();
  SPIClass* bus = &SPI;
  SPI1Class* sameBus = &SPI1;
  observed = bus == sameBus;
  bus->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  observed = bus->transfer(0x55);
  bus->endTransaction();
  sameBus->beginTransaction(SPI1Settings(1000000, MSBFIRST, SPI_MODE0));
  sameBus->endTransaction();
  attachInterrupt(digitalPinToInterrupt(D1), irq, CHANGE);
  detachInterrupt(digitalPinToInterrupt(D1));
  EEPROM.update(0, EEPROM.read(0));
  for (uint8_t p = 0; p < NUM_DIGITAL_PINS; p++) {
    observed = digitalPinToPort(p);
    observed = digitalPinToBitMask(p);
    observed = digitalPinToTimer(p);
    observed = digitalPinToPCICRbit(p);
    observed = digitalPinToPCMSKbit(p);
  }
  analogWrite(D3, 127); observed = analogRead(A0);
}
void loop() { observed = Serial1.available(); }

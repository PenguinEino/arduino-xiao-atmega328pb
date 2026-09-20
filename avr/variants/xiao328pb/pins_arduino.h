// SPDX-License-Identifier: LGPL-2.1-or-later
// Based on Arduino/MiniCore pin definitions: David A. Mellis (2007), MCUdude (2018).
// XIAO numbering for the ATmega Gokan QFN board. 2026 PenguinEino.
#ifndef Pins_Arduino_h
#define Pins_Arduino_h
#include <avr/pgmspace.h>
#if !defined(__AVR_ATmega328PB__)
#error "This variant requires ATmega328PB"
#endif
#define MINICORE
#define MCUDUDE_MINICORE
#define ATMEGA_GOKAN_XIAO
#define NUM_DIGITAL_PINS 27
#define NUM_ANALOG_INPUTS 8
#define PINS_COUNT NUM_DIGITAL_PINS
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define PIN_PE2 0
#define PIN_PD2 1
#define PIN_PD3 2
#define PIN_PB1 3
#define PIN_PC4 4
#define PIN_PC5 5
#define PIN_PB3 6
#define PIN_PB4 7
#define PIN_PC1 8
#define PIN_PC0 9
#define PIN_PE3 10
#define PIN_PD0 11
#define PIN_PD1 12
#define PIN_PD5 13
#define PIN_PC2 14
#define PIN_PC3 15
#define PIN_PD4 16
#define PIN_PD6 17
#define PIN_PD7 18
#define PIN_PB0 19
#define PIN_PB2 20
#define PIN_PB5 21
#define PIN_PE0 22
#define PIN_PE1 23
#define PIN_PB6 24
#define PIN_PB7 25
#define PIN_PC6 26

#define LED_BUILTIN PIN_PD5
#define LED_BUILTIN_ACTIVE LOW
static const uint8_t LED = LED_BUILTIN;

// Only XIAO positions with physical ADC capability get Ax aliases.
#define PIN_A0 D0
#define PIN_A4 D4
#define PIN_A5 D5
#define PIN_A8 D8
#define PIN_A9 D9
#define PIN_A10 D10
static const uint8_t A0 = PIN_A0;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A8 = PIN_A8;
static const uint8_t A9 = PIN_A9;
static const uint8_t A10 = PIN_A10;
// ADC channel aliases also expose the two internal/test-point channels.
#define PIN_ADC0 PIN_PC0
#define PIN_ADC1 PIN_PC1
#define PIN_ADC2 PIN_PC2
#define PIN_ADC3 PIN_PC3
#define PIN_ADC4 PIN_PC4
#define PIN_ADC5 PIN_PC5
#define PIN_ADC6 PIN_PE2
#define PIN_ADC7 PIN_PE3
#define analogInputToDigitalPin(p) ((p)==0 ? PIN_PC0 : (p)==1 ? PIN_PC1 : (p)==2 ? PIN_PC2 : (p)==3 ? PIN_PC3 : (p)==4 ? PIN_PC4 : (p)==5 ? PIN_PC5 : (p)==6 ? PIN_PE2 : (p)==7 ? PIN_PE3 : -1)
// analogRead accepts board pin numbers, including the Ax / PIN_ADCx aliases.
#define analogPinToChannel(p) ((p)==PIN_PC0 ? 0 : (p)==PIN_PC1 ? 1 : (p)==PIN_PC2 ? 2 : (p)==PIN_PC3 ? 3 : (p)==PIN_PC4 ? 4 : (p)==PIN_PC5 ? 5 : (p)==PIN_PE2 ? 6 : (p)==PIN_PE3 ? 7 : -1)
#define digitalPinHasPWM(p) ((p)==PIN_PD0 || (p)==PIN_PD1 || (p)==PIN_PD2 || (p)==PIN_PD3 || (p)==PIN_PD5 || (p)==PIN_PD6 || (p)==PIN_PB1 || (p)==PIN_PB2 || (p)==PIN_PB3)

#define WIRE_INTERFACES_COUNT 2
#define PIN_WIRE0_SDA PIN_PC4
#define PIN_WIRE0_SCL PIN_PC5
#define PIN_WIRE1_SDA PIN_PE0
#define PIN_WIRE1_SCL PIN_PE1
#define PIN_WIRE_SDA PIN_WIRE0_SDA
#define PIN_WIRE_SCL PIN_WIRE0_SCL
#define PIN_WIRE_SDA0 PIN_WIRE0_SDA
#define PIN_WIRE_SCL0 PIN_WIRE0_SCL
#define PIN_WIRE_SDA1 PIN_WIRE1_SDA
#define PIN_WIRE_SCL1 PIN_WIRE1_SCL
static const uint8_t SDA = PIN_WIRE_SDA, SCL = PIN_WIRE_SCL;
static const uint8_t SDA0 = PIN_WIRE0_SDA, SCL0 = PIN_WIRE0_SCL;
static const uint8_t SDA1 = PIN_WIRE1_SDA, SCL1 = PIN_WIRE1_SCL;

#define SPI_INTERFACES_COUNT 2
#define PIN_SPI0_SS PIN_PB2
#define PIN_SPI0_MOSI PIN_PB3
#define PIN_SPI0_MISO PIN_PB4
#define PIN_SPI0_SCK PIN_PB5
#define PIN_SPI1_SS PIN_PE2
#define PIN_SPI1_MOSI PIN_PE3
#define PIN_SPI1_MISO PIN_PC0
#define PIN_SPI1_SCK PIN_PC1
#define PIN_SPI_SS PIN_SPI1_SS
#define PIN_SPI_MOSI PIN_SPI1_MOSI
#define PIN_SPI_MISO PIN_SPI1_MISO
#define PIN_SPI_SCK PIN_SPI1_SCK
#define PIN_SPI_SS0 PIN_SPI0_SS
#define PIN_SPI_MOSI0 PIN_SPI0_MOSI
#define PIN_SPI_MISO0 PIN_SPI0_MISO
#define PIN_SPI_SCK0 PIN_SPI0_SCK
#define PIN_SPI_SS1 PIN_SPI1_SS
#define PIN_SPI_MOSI1 PIN_SPI1_MOSI
#define PIN_SPI_MISO1 PIN_SPI1_MISO
#define PIN_SPI_SCK1 PIN_SPI1_SCK
static const uint8_t SS = PIN_SPI_SS, MOSI = PIN_SPI_MOSI, MISO = PIN_SPI_MISO, SCK = PIN_SPI_SCK;
static const uint8_t SS0 = PIN_SPI0_SS, MOSI0 = PIN_SPI0_MOSI, MISO0 = PIN_SPI0_MISO, SCK0 = PIN_SPI0_SCK;
static const uint8_t SS1 = PIN_SPI1_SS, MOSI1 = PIN_SPI1_MOSI, MISO1 = PIN_SPI1_MISO, SCK1 = PIN_SPI1_SCK;

#define PIN_SERIAL0_RX PIN_PD0
#define PIN_SERIAL0_TX PIN_PD1
#define PIN_SERIAL1_RX PIN_PB4
#define PIN_SERIAL1_TX PIN_PB3
#define PIN_SERIAL_RX PIN_SERIAL1_RX
#define PIN_SERIAL_TX PIN_SERIAL1_TX
#define PIN_SERIAL_RX0 PIN_SERIAL0_RX
#define PIN_SERIAL_TX0 PIN_SERIAL0_TX
#define PIN_SERIAL_RX1 PIN_SERIAL1_RX
#define PIN_SERIAL_TX1 PIN_SERIAL1_TX
#define SERIAL_PORT_MONITOR Serial
#define SERIAL_PORT_HARDWARE Serial
#define SERIAL_PORT_HARDWARE_OPEN Serial1
#define SERIAL_PORT_HARDWARE1 Serial1
#define SERIAL_PORT_HARDWARE_OPEN1 Serial1

#define EXTERNAL_NUM_INTERRUPTS 2
#define digitalPinToInterrupt(p) ((p)==PIN_PD2 ? 0 : (p)==PIN_PD3 ? 1 : NOT_AN_INTERRUPT)
// PCINT group and bit are derived from the remapped port/mask tables.
#define digitalPinToPCICR(p) ((unsigned)(p)<NUM_DIGITAL_PINS ? &PCICR : (uint8_t*)0)
#define digitalPinToPCICRbit(p) (digitalPinToPort(p)-2)
#define digitalPinToPCMSK(p) ((unsigned)(p)>=NUM_DIGITAL_PINS ? (uint8_t*)0 : digitalPinToPort(p)==2 ? &PCMSK0 : digitalPinToPort(p)==3 ? &PCMSK1 : digitalPinToPort(p)==4 ? &PCMSK2 : &PCMSK3)
#define digitalPinToPCMSKbit(p) (__builtin_ctz((unsigned)digitalPinToBitMask(p)))

#ifdef ARDUINO_MAIN
const uint16_t PROGMEM port_to_mode_PGM[] = {NOT_A_PORT, NOT_A_PORT, (uint16_t)&DDRB, (uint16_t)&DDRC, (uint16_t)&DDRD, (uint16_t)&DDRE};
const uint16_t PROGMEM port_to_output_PGM[] = {NOT_A_PORT, NOT_A_PORT, (uint16_t)&PORTB, (uint16_t)&PORTC, (uint16_t)&PORTD, (uint16_t)&PORTE};
const uint16_t PROGMEM port_to_input_PGM[] = {NOT_A_PORT, NOT_A_PORT, (uint16_t)&PINB, (uint16_t)&PINC, (uint16_t)&PIND, (uint16_t)&PINE};
const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
  PE, // 0: PE2
  PD, // 1: PD2
  PD, // 2: PD3
  PB, // 3: PB1
  PC, // 4: PC4
  PC, // 5: PC5
  PB, // 6: PB3
  PB, // 7: PB4
  PC, // 8: PC1
  PC, // 9: PC0
  PE, // 10: PE3
  PD, // 11: PD0
  PD, // 12: PD1
  PD, // 13: PD5
  PC, // 14: PC2
  PC, // 15: PC3
  PD, // 16: PD4
  PD, // 17: PD6
  PD, // 18: PD7
  PB, // 19: PB0
  PB, // 20: PB2
  PB, // 21: PB5
  PE, // 22: PE0
  PE, // 23: PE1
  PB, // 24: PB6
  PB, // 25: PB7
  PC, // 26: PC6
};
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(2), // 0: PE2
  _BV(2), // 1: PD2
  _BV(3), // 2: PD3
  _BV(1), // 3: PB1
  _BV(4), // 4: PC4
  _BV(5), // 5: PC5
  _BV(3), // 6: PB3
  _BV(4), // 7: PB4
  _BV(1), // 8: PC1
  _BV(0), // 9: PC0
  _BV(3), // 10: PE3
  _BV(0), // 11: PD0
  _BV(1), // 12: PD1
  _BV(5), // 13: PD5
  _BV(2), // 14: PC2
  _BV(3), // 15: PC3
  _BV(4), // 16: PD4
  _BV(6), // 17: PD6
  _BV(7), // 18: PD7
  _BV(0), // 19: PB0
  _BV(2), // 20: PB2
  _BV(5), // 21: PB5
  _BV(0), // 22: PE0
  _BV(1), // 23: PE1
  _BV(6), // 24: PB6
  _BV(7), // 25: PB7
  _BV(6), // 26: PC6
};
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  NOT_ON_TIMER, // 0: PE2
  TIMER4B, // 1: PD2
  TIMER2B, // 2: PD3
  TIMER1A, // 3: PB1
  NOT_ON_TIMER, // 4: PC4
  NOT_ON_TIMER, // 5: PC5
  TIMER2A, // 6: PB3
  NOT_ON_TIMER, // 7: PB4
  NOT_ON_TIMER, // 8: PC1
  NOT_ON_TIMER, // 9: PC0
  NOT_ON_TIMER, // 10: PE3
  TIMER3A, // 11: PD0
  TIMER4A, // 12: PD1
  TIMER0B, // 13: PD5
  NOT_ON_TIMER, // 14: PC2
  NOT_ON_TIMER, // 15: PC3
  NOT_ON_TIMER, // 16: PD4
  TIMER0A, // 17: PD6
  NOT_ON_TIMER, // 18: PD7
  NOT_ON_TIMER, // 19: PB0
  TIMER1B, // 20: PB2
  NOT_ON_TIMER, // 21: PB5
  NOT_ON_TIMER, // 22: PE0
  NOT_ON_TIMER, // 23: PE1
  NOT_ON_TIMER, // 24: PB6
  NOT_ON_TIMER, // 25: PB7
  NOT_ON_TIMER, // 26: PC6
};
#endif
// MiniCore register compatibility. SPI.h adapter uses SPI1 explicitly;
// legacy register names remain SPI0, as in upstream MiniCore.
#define SPCR SPCR0
#define SPSR SPSR0
#define SPDR SPDR0
#define SPI_STC_vect SPI0_STC_vect
#define SPI_STC_vect_num SPI0_STC_vect_num
#define TWBR TWBR0
#define TWSR TWSR0
#define TWAR TWAR0
#define TWDR TWDR0
#define TWCR TWCR0
#define TWAMR TWAMR0
#define TWI_vect TWI0_vect
#define TWI_vect_num TWI0_vect_num
#define USART_RX_vect USART0_RX_vect
#define USART_RX_vect_num USART0_RX_vect_num
#define USART_UDRE_vect USART0_UDRE_vect
#define USART_UDRE_vect_num USART0_UDRE_vect_num
#define USART_TX_vect USART0_TX_vect
#define USART_TX_vect_num USART0_TX_vect_num
#endif

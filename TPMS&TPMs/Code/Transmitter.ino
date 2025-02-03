#include <TPMS_T_config.h>
#include <TPMS_structures.h>


RF24 radio(RF_CE, RF_CSN);

volatile bool hasToInit = true;
volatile uint8_t INTtrigger = 0, prevINTtrigger = 0;

struct {
  char code;
} ack;

void setupInterrupts() {
  cli();
  MCUCR |= (1 << ISC01); // Enable INT0 on falling edge.
  GIMSK |= (1 << INT0) | (1 << PCIE0); // Enable INT0 and pin change interrupts.

  PCMSK0 |= (1 << PCINT7); // Enable pin change interrupt on PA7.
  sei();
}

ISR(INT0_vect) {
  INTtrigger++;
}

ISR(PCINT0_vect) {
  hasToInit = true;
}

void setupMCU() {
  MCUCR |= (1 << SM1); // When "sleep" is executed the MCU goes in power down mode.

  ADMUX |= (1 << REFS1) | (1 << MUX5) | (1 << MUX1); // Enable the temperature sensor.
  ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0); // Enable the ADC with on 125kHz.

  // Configure pins.
  DDRA |= (1 << DDA3) | (1 << DDA2); // Configure CS/A and CS/P as outputs.
  PORTA |= (1 << PORTA3) | (1 << PORTA2); // Set CS on both I2C devices to 1.

  // Configure the INT0 as output.
  DDRB &= ~(1 << DDB2);
  PORTB |= (1 << PORTB2); // Enable internal pull-up resistor.

  setupInterrupts();
}

void sleepMCU() {
  MCUCR |= (1 << SE);
  __asm__ __volatile__("sleep");
  MCUCR &= ~(1 << SE);
}

void setupRF() {
  if (!radio.begin())
    PORTB |= (1 << PORTB0);

  radio.setChannel(CHANNEL);
  radio.setRadiation(RF24_PA_MIN, RF24_250KBPS);

  radio.setRetries(15, 15);
  radio.getDynamicPayloadSize();
  radio.enableAckPayload();
  // radio.setAutoAck(false);

  radio.openWritingPipe(identity.address);
  radio.stopListening();
}

void setupPressure() {
  writeRegister(PRESSURE_ADDR, 0x20, (1 << 6)); // Enable full scale mode (4060 hPa).
  writeRegister(PRESSURE_ADDR, 0x10, (1 << 4)); // Configure the pressure sensor with AVG = 4 and ODR = 4 Hz.
  writeRegister(PRESSURE_ADDR, 0x5F, 0x00); // Disable AH1/QVAR1 and AH2/QVAR2.
}

void setupACC() {
  writeRegister(ACC_ADDR, 0x20, (1 << 5) | (1 << 3) | (1 << 1) | (1 << 0)); // Configure the accelerometer to work on Low-power mode (10 Hz) and enable it to work on Y and X axes.
  writeRegister(ACC_ADDR, 0x22, (1 << 6)); // Route interrupt to INT1 pin.
  writeRegister(ACC_ADDR, 0x30, (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)); // Configure INT1 to generate interrupt on movement on both axises.
  writeRegister(ACC_ADDR, 0x32, (1 << 2) | (1 << 1)); // Configure the sensitivity 96 mg @ ±2g.
  writeRegister(ACC_ADDR, 0x33, (1 << 0)); // Configure a 100 ms duration so the noise is ignored.
  writeRegister(ACC_ADDR, 0x26, (1 << 1)); // Configure INT polarity to active low.
}

void writeRegister(const byte address, const byte reg, const byte value) {
  Wire.begin();

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();

  Wire.end();
}

byte readRegister(const byte address, const byte reg) {
  Wire.begin();

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false); // "False" keeps the connection open.
  
  Wire.requestFrom(address, 1);
  byte result = 0x00;
  if (Wire.available()) 
    result = Wire.read();
  
  Wire.end();
  return result;
}

void getPressure() {
  uint16_t pressureSum = 0;
  
  for (uint8_t measCnt = 0; measCnt != NUMBER_OF_MEAS; ++measCnt) {
    // Use the given formula in the datasheet for pressure calculation.
    uint32_t pressure = (readRegister(PRESSURE_ADDR, 0x2A) << 16) | (readRegister(PRESSURE_ADDR, 0x29) << 8) | readRegister(PRESSURE_ADDR, 0x28);
    pressure /= 2048;
    pressure /= 10; // Convert to kPa.

    pressureSum += pressure;
  }

  payload.pressure = pressureSum / NUMBER_OF_MEAS >> 1; // >> 1 so it fits in 1 byte (406 kPa is the largest value).
}

void getTemp() {
  int16_t tempSum = 0;
  
  for (uint8_t measCnt = 0; measCnt != NUMBER_OF_MEAS; ++measCnt) {
    ADCSRA |= (1 << ADSC); // Begin measurement.
    while (ADCSRA & (1 << ADSC)); // Wait for measure (the ADCL bit returns to 0 ones the conversion is completed).

    tempSum += ((ADCH << 8) | ADCL);
  }

  payload.temp = tempSum / NUMBER_OF_MEAS - 44; // Using a temperature offset.
}

void gatherPayload() {
  getTemp();
  getPressure();
}

bool changeInPressureOrTemp() {
  static int8_t prevTemp = 0;
  static uint16_t prevPressure = 0;

  if (abs((payload.pressure << 1) - prevPressure) >= prevPressure * 0.05
      || abs(payload.temp - prevTemp) >= prevTemp * 0.05)
  {
    prevPressure = (payload.pressure << 1);
    prevTemp = payload.temp;
    
    return true;
  }
  
  return false;
}

void OVFcorrection(uint32_t *current_ms, uint32_t *keepAlive_ms, uint32_t *prevTemp_ms, uint32_t *prevPressure_ms, uint32_t *prevINT_ms) {
  if (*current_ms >= *keepAlive_ms)
    return;
    
  *current_ms = *keepAlive_ms = *prevTemp_ms = *prevPressure_ms = *prevINT_ms = 0;
}

void interruptCheck(uint32_t current_ms, uint32_t *prevINT_ms) {
  if (INTtrigger <= prevINTtrigger) 
    return;
  
  prevINTtrigger++;
  *prevINT_ms = current_ms;
}

void setup() {
  // Setup debug pin.
  DDRB |= (1 << DDB0);
  PORTB &= ~(1 << PORTB0);

  setupMCU(); 
  setupRF();
  setupPressure();
  setupACC();
  
  sleepMCU();
}

void loop() {
  static uint32_t current_ms = 0, keepAlive_ms = 0, prevTemp_ms = 0, prevPressure_ms = 0, prevINT_ms = 0;
  current_ms = millis();

  OVFcorrection(&current_ms, &keepAlive_ms, &prevTemp_ms, &prevPressure_ms, &prevINT_ms);
  interruptCheck(current_ms, &prevINT_ms);

  if (current_ms - prevINT_ms >= PREV_INT0_MS)
    sleepMCU();

  if (hasToInit)
    sendIdentity();

  if (current_ms - prevTemp_ms >= TEMP_MEAS_MS) {
    prevTemp_ms = current_ms;
    getTemp();

    if (changeInPressureOrTemp()) {
      keepAlive_ms = current_ms;
      sendPayload();
    }
  }

  if (current_ms - prevPressure_ms >= PRESSURE_MEAS_MS) {
    prevPressure_ms = current_ms;
    getPressure();

    if (changeInPressureOrTemp()) {
      keepAlive_ms = current_ms;
      sendPayload();
    }
  }

  if (current_ms - keepAlive_ms >= KEEP_ALIVE_MS) {
    keepAlive_ms = current_ms;

    gatherPayload();
    sendPayload();
  }
}

void sendIdentity() {
  if (!radio.write(&identity, sizeof(identity))
      || !radio.isAckPayloadAvailable()) 
    return;
  
  if (radio.getDynamicPayloadSize() == 1) {
    sleepMCU();
    return;
  }

  radio.read(&identity, radio.getDynamicPayloadSize());
  
  radio.setRetries(identity.retransmitDelay, 15);
  radio.openWritingPipe(identity.address);

  hasToInit = false;
  return;
}

void sendPayload() {
  if (!radio.write(&payload, sizeof(payload))
      || !radio.isAckPayloadAvailable()) 
    return;

  radio.read(&ack, radio.getDynamicPayloadSize());

  return;
}
#ifndef TPMS_T_CONFIG_h
#define TPMS_T_CONFIG_h

#include <RF24.h>
#include <Wire.h>

#define ADDRESS_LEN       5
#define CHANNEL           69
#define NUMBER_OF_MEAS    10

#define PRESSURE_MEAS_MS  5000
#define TEMP_MEAS_MS      60000
#define KEEP_ALIVE_MS     10000
#define PREV_INT0_MS      3000

#define PRESSURE_ADDR     0b1011100
#define ACC_ADDR          0b0011001

#define RF_CE             9
#define RF_CSN            1

#endif

#ifndef TPMS_R_CONFIG_H
#define TPMS_R_CONFIG_H

#include <RF24.h>
#include <EEPROM.h>

#define SUMMER_MARKER_ADDR  0
#define SUMMER_TIRES_ADDR   (SUMMER_MARKER_ADDR + 1)
#define SUMMER_MARKER       's'

#define WINTER_MARKER_ADDR  sizeof(tires_t) + 2
#define WINTER_TIRES_ADDR   (WINTER_MARKER_ADDR + 1)
#define WINTER_MARKER       'w'

#define ADDRESS_LEN         5
#define CHANNEL             69

#define TIRES               1
#define TRANSMIT_OFFSET     1
#define TPMS_SIGNAL_MS      10500

#define RF_CE               9
#define RF_CSN              10

#endif

#ifndef TPMS_S_H
#define TPMS_S_H


struct __attribute__((packed)) {
  int8_t temp;
  uint8_t pressure; // This variable should be << 1 so we return it to the real value [hPa].
} payload;

struct __attribute__((packed)) {
  byte address[ADDRESS_LEN];
  uint8_t retransmitDelay;
} identity = { .address = { 'B', 'R', 'O', 'A', 'D' } };

#endif

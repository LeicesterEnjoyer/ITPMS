#ifndef TIRES_H
#define TIRES_H

#include <TPMS_R_config.h>


typedef struct {
  uint8_t position; // Reference: 0 - FL; 1 - FR; 2 - RR; 3 - RL.
  byte address[ADDRESS_LEN];
  uint8_t retransmitDelay;
} tire_t;

typedef struct {
  tire_t tire[TIRES];
  char complect; // Reference: s - summer; w - winter. The summer tires include the all-season tires.
} tires_t;

bool checkTirePosition(tires_t tires, uint8_t position, const char *address);

void updateTirePosition(tires_t *tires, uint8_t position, const char *address);

// Initializes the tire with unique address and retransmission delay, and the correct position.
void initTier(tires_t *tires, uint8_t position);

void generateUniqueAddress(tires_t *tires, uint8_t position);

void generateUniqueRetransmitDelay(tires_t *tires, uint8_t position);

/*
  Select the correct tire complect if there is previously saved one.
  Return 'true' if there is a saved complect, otherwise return 'false'.
*/
bool selectTires(tires_t *tires, char tireComplect);

char getTireComplect(tires_t tires);

void updateRetransmitDelay(tires_t *tires, uint8_t position, uint8_t retransmitDelay);

#endif

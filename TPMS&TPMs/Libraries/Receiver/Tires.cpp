#include <Tires.h>


bool checkTirePosition(tires_t tires, uint8_t position, const char *address) {
  return memcmp(tires.tire[position].address, address, ADDRESS_LEN) == 0;
}

void updateTirePosition(tires_t *tires, uint8_t position, const char *address) {
  memcpy(tires->tire[position].address, address, ADDRESS_LEN);
}

// Initializes the tire with unique address and retransmission delay, and the correct position.
void initTier(tires_t *tires, uint8_t position) {
  generateUniqueAddress(tires, position);
  generateUniqueRetransmitDelay(tires, position);
}

void generateUniqueAddress(tires_t *tires, uint8_t position) {
  bool unique;

  do {
    unique = true;

    // Generate random address.
    for (uint8_t i = 0; i < ADDRESS_LEN; ++i)
      tires->tire[position].address[i] = random(0, 256);
    
    // Check if the generated address is unique.
    for (uint8_t i = 0; i < TIRES; ++i)
      if (i != position && memcmp(tires->tire[i].address, tires->tire[position].address, ADDRESS_LEN) == 0) {
        unique = false;
        break;
      }
  } while (!unique);
}

void generateUniqueRetransmitDelay(tires_t *tires, uint8_t position) {
  if (position == 0)
    tires->tire[position].retransmitDelay = 0; // Default retransmission time -> 250 us.
  else
    tires->tire[position].retransmitDelay = tires->tire[position - 1].retransmitDelay + TRANSMIT_OFFSET;
}

/*
  Select the correct tire complect if there is previously saved one.
  Return 'true' if there is a saved complect, otherwise return 'false'.
*/
bool selectTires(tires_t *tires, char tireComplect) {
  if (tireComplect == 's') {
    if (EEPROM.read(SUMMER_MARKER_ADDR) != SUMMER_MARKER)
      return false;
    
    EEPROM.get(SUMMER_TIRES_ADDR, *tires);
    return true;
  }

  if (EEPROM.read(WINTER_MARKER_ADDR) != WINTER_MARKER)
    return false;
    
  EEPROM.get(WINTER_TIRES_ADDR, *tires);
  return true;
}

char getTireComplect(tires_t tires) {
  return tires.complect;
}

void updateRetransmitDelay(tires_t *tires, uint8_t position, uint8_t retransmitDelay) {
  tires->tire[position].retransmitDelay = retransmitDelay;
}
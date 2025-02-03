#include <Tires.h>
#include <TPMS_structures.h>


RF24 radio(RF_CE, RF_CSN);


void clearEEPROM() {
  for (uint16_t i = 0; i < EEPROM.length(); ++i) {
    if (EEPROM.read(i) == 0xFF)
      break;
    EEPROM.write(i, 0xFF);
  }
}

void removeGarbage() {
  byte garbage[32];
  radio.read(&garbage, radio.getDynamicPayloadSize());
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  randomSeed(analogRead(A0)); // Seed randomness with floating analog pin.

  // Begin communication with the RF module.
  if (!radio.begin())
    Serial.println("Couldn't establish connection with the RF module.");
  
  radio.setChannel(CHANNEL);
  radio.setRadiation(RF24_PA_MIN, RF24_250KBPS);

  radio.getDynamicPayloadSize();
  radio.enableAckPayload();
  // radio.setAutoAck(false);

  const byte broadAddress[ADDRESS_LEN] = {'B', 'R', 'O', 'A', 'D'};

  radio.openReadingPipe(0, broadAddress);
  radio.startListening();

  // For debugging purposes only.
  // clearEEPROM(); 

  Serial.println("Connection established!");
}

void loop() {
  static tires_t tires;
  static char tireComplect = '0';
  static const char* positions[] = {"front left", "front right", "rear right", "rear left"};
  uint8_t pipe;
  
  static uint32_t current_ms = 0, lastReceived_ms[TIRES] = { 0 };
  static bool isInitializing = true, reinitialize = false, isMoving = true, synchronized = true;
  
  // isMoving comes from the automobile itself and checks if we are moving so we can synchronize the time between recieves.
  if (!isMoving)
    synchronized = false;

  if (!synchronized && isMoving) {
    uint32_t syncTime = millis();

    for (uint8_t i = 0; i < TIRES; ++i) 
      lastReceived_ms[i] = syncTime;
    synchronized = true;
  }

  current_ms = millis();

  // Check for a message in the RX fifo.
  if (radio.available(&pipe)) {
    if (!isInitializing) {
      if (radio.getDynamicPayloadSize() != sizeof(payload)) {
        removeGarbage();
        return;
      }
      
      lastReceived_ms[pipe - 1] = current_ms;

      static uint16_t prevPressure = payload.pressure << 1;
      static int8_t prevTemp = payload.temp;

      // Receive payload from TPMs (a tire).
      memset(&payload, 0, sizeof(payload));
      radio.read(&payload, sizeof(payload));

      Serial.print("Rceived payload from ");
      Serial.print(positions[pipe - 1]);
      Serial.println(":");
      Serial.print("Temperature: ");
      Serial.println(payload.temp);
      Serial.print("Pressure: ");
      Serial.println((payload.pressure << 1) / 1000); // To convert back to kPa -> y = (x << 1) and than to bar -> (y / 100).

      if (abs(payload.pressure - prevPressure) >= prevPressure * 0.1
          || abs(payload.temp - prevTemp) >= abs(prevTemp) * 0.1)
      {
        Serial.println("<Warning on the infotainment>");
        Serial.print("Unusual temperature or pressure changes detected on the ");
        Serial.print(positions[pipe]);
        Serial.println(" tire!");
        Serial.println("Check if everything is fine, before continuing to drive!");
        Serial.println("<Waiting for driver to close the warning from the infotainment>");

        prevPressure = payload.pressure;
        prevTemp = payload.temp;
      }
    }

    // The initialization is enabled by the driver from the infotainment.
    else {
      static uint8_t position = 0;

      // Receive identity from TPMs (a tire).
      memset(&identity, 0, sizeof(identity));
      radio.read(&identity, sizeof(identity));

      // For debugging purposes only.
      Serial.println("Received identity: ");
      Serial.print("Address: ");
      Serial.println((char *)identity.address);
      Serial.print("Retransmit Delay: ");
      Serial.println((char *)identity.retransmitDelay);

      // Driver must choose the tire complect. Valid options: 's' - summer; 'w' - winter.
      if (tireComplect == '0') {
        Serial.println("<Warning on the infotainment>");
        Serial.println("Choose tire complect!");
        Serial.println("<Waiting for driver's decision from the infotainment>");
      }
       
    
      // Select tire complect if possible (if has been used before). If 'selectTires()' returns false save the corresponding marker.  
      if (!selectTires(&tires, tireComplect))
        if (tireComplect == 's')
          EEPROM.write(SUMMER_MARKER_ADDR, SUMMER_MARKER);
        else
          EEPROM.write(WINTER_MARKER_ADDR, WINTER_MARKER);

      // Check if the current tire is already in that position.
      if (!checkTirePosition(tires, position, identity.address)) {
        // Check if the tire address is "BROAD". If it is than the tire is being initialized for the first time.
        if (memcmp(identity.address, "BROAD", sizeof(identity.address)) == 0) {
          initTier(&tires, position); // Initialize the tire with its position and unique address.

          // Load the initialized values in the identity.
          memset(&identity, 0, sizeof(identity));
          memcpy(identity.address, tires.tire[position].address, ADDRESS_LEN);
          identity.retransmitDelay = tires.tire[position].retransmitDelay;

          radio.writeAckPayload(0, &identity, sizeof(identity)); // Return the ACK with the initialized identity.
          radio.openReadingPipe(position + 1, tires.tire[position].address); // Open a new pipe for secure communication with the tire.
        }
        // If the address isn's "BROAD" the tire has been initialized before.
        else {
          // Wait for the driver's response.
          Serial.println("<Warning on the infotainment>");
          Serial.println("The tire that is currently initializing isn't on it's original position!");
          Serial.print("Would you like to fix this or to reinitialize it on it's new position?");
          Serial.println(" If you choose to reinitialize, this decision will also be valid for the other tires!");
          Serial.println("<Waiting for driver's decision from the infotainment>");

          // If he has selected to reinitialize the tires, all tires will be moved to their current positions (that are different from the their previous ones).
          if (reinitialize) {
            updateTirePosition(&tires, position, identity.address); // Move tire in correct position.
            
            radio.writeAckPayload(0, &identity, sizeof(identity)); // Return the ACK with the initialized identity.
            radio.openReadingPipe(position + 1, tires.tire[position].address); // Open a new pipe for secure communication with the tire.
          }
          // The TPMs sensor goes to sleep and waits for driver to initialize it again by the init pen.
          else {
            radio.writeAckPayload(0, 'S', sizeof('S'));
            return;
          }
        }
      }
      // Check if all tires are initialized.
      if (++position == TIRES) {
        isInitializing = false;
        reinitialize = false;

        Serial.println("All tires have been initialized!");

        // Save the tires in the right place.
        if (tireComplect == 's')
          EEPROM.put(SUMMER_TIRES_ADDR, tires);
        else 
          EEPROM.put(WINTER_TIRES_ADDR, tires);
      }
    }
  }

  if (isMoving)
    for (uint8_t i = 0; i < TIRES; ++i) 
      if (lastReceived_ms[i] && current_ms - lastReceived_ms[i] >= TPMS_SIGNAL_MS) {
        Serial.println("<Warning on the infotainment>");
        Serial.print("The connection between the ");
        Serial.print(positions[i]);
        Serial.println(" is lost!");
      }
}
// Define SPI pins and other constants
#define MOSI 11
#define MISO 12
#define SCK  13
#define SS   7   // Slave select pin
#define BUSY 3   // BUSY pin to check if the slave is ready
#define RESET A0

void setup() {
  confSPI();
  confGPIO();
  
  Serial.begin(9600);

  resetLR1110();
  
  Serial.println("Press enter in the terminal to continue...");
}

void confSPI() {
  // Set SPI pins as OUTPUT
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(MISO, INPUT);
  
  // Set SPI default states
  digitalWrite(SS, HIGH); // Ensure SS starts HIGH
  digitalWrite(MOSI, LOW);
  digitalWrite(SCK, LOW);
}

void confGPIO() {
  // Set GPIO pins as OUTPUT or INPUT
  pinMode(BUSY, INPUT);
  pinMode(RESET, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    if (Serial.read() == 0xA) {
      Serial.println("Starting...");
      getVersion();
      getTemp();
      Serial.println("Done");
    }
  }
}

void getVersion() {
  Serial.println("Getting LR1110 version...");

  uint8_t commandSPI[] = {0x01, 0x01};
  uint8_t* respondSPI = sendSPICommandToSPI(commandSPI, sizeof(commandSPI) / sizeof(commandSPI[0]), 5);

  if ((respondSPI[0] & 0b00001110) >> 1 == 0) {
    Serial.println("ERROR: CMD_FAIL");
    return;
  }

  Serial.println("LR1110 version:");
  Serial.print("- HW version: ");
  Serial.println(respondSPI[1]);
  switch (respondSPI[2]) {
    case 1:
      Serial.println("- Use Case: LR1110");
      break;
    case 2:
      Serial.println("- Use Case: LR1120");
      break;
    case 3:
      Serial.println("- Use Case: LR1121");
      break;
    default:
      Serial.println("ERROR: Can not find use case");
      break;
  }
  Serial.print("- FW major: ");
  Serial.println(respondSPI[3]);
  Serial.print("- FW minor: ");
  Serial.println(respondSPI[4]);

  // Free dynamically allocated memory
  free(respondSPI);
}

void getTemp() {
  Serial.println("Getting LR1110 temperature...");

  //remember to set Tcxo mode before getting temeprature
  SetTcxoMode();

  uint8_t commandSPI[] = {0x01, 0x1A};
  uint8_t* respondSPI = sendSPICommandToSPI(commandSPI, sizeof(commandSPI) / sizeof(commandSPI[0]), 3);

  if ((respondSPI[0] & 0b00001110) >> 1 == 0) {
    Serial.println("ERROR: CMD_FAIL");
    return;
  }

  // Extracting bits [15:8] and [7:0] and then [10:0]
  int rawTemperature_10_0 = ((respondSPI[1] << 8) | respondSPI[2]) & 0x7FF;

  float temperature = 25 + (1000/(-1.7)) * ((rawTemperature_10_0/2047.0) * 1.35 - 0.7295);
  Serial.print("LR1110 temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Free dynamically allocated memory
  free(respondSPI);
}

uint8_t* sendSPICommandToSPI(uint8_t commands[], int commandLength, int responseLength) {
  waitForBusyToGet(LOW);

  // Allocate memory for response array
  uint8_t* respondSPI = (uint8_t*)malloc(responseLength * sizeof(uint8_t));

  // Start the command sequence
  digitalWrite(SS, LOW); // Assert SS to indicate the start of communication
  for (int i = 0; i < commandLength; i++) {
    spiTransfer(commands[i]);
  }
  digitalWrite(SS, HIGH); // De-assert SS, end of command sequence

  waitForBusyToGet(LOW);

  digitalWrite(SS, LOW);

  waitForBusyToGet(HIGH);

  for (int i = 0; i < responseLength; i++) {
    respondSPI[i] = spiTransfer(0x00);
  }

  digitalWrite(SS, HIGH); // End communication

  waitForBusyToGet(LOW);

  return respondSPI;
}

void waitForBusyToGet(bool state) {
  while (!digitalRead(BUSY) == state) {
    delay(1);
  }
}

void SetTcxoMode() {
  uint8_t commandSPI[] = {0x01, 0x17, 0x02, 0x00, 0x00, 0xA3};
  sendSPICommandToSPI(commandSPI, sizeof(commandSPI) / sizeof(commandSPI[0]), 6);
  delay(100);
}

void resetLR1110() {
  digitalWrite(RESET, LOW);
  delayMicroseconds(200);
  digitalWrite(RESET, HIGH);
}

uint8_t spiTransfer(uint8_t data) {
  uint8_t receivedByte = 0;
  for (int i = 7; i >= 0; i--) {
    digitalWrite(MOSI, (data >> i) & 0x01); // Send the next bit to MOSI pin
    digitalWrite(SCK, HIGH); // Set clock to HIGH to read MISO pin
    receivedByte = (receivedByte << 1) | digitalRead(MISO); // Read MISO pin and append the bit to the received byte
    digitalWrite(SCK, LOW); // Set clock to LOW for the next bit
  }
  return receivedByte;
}

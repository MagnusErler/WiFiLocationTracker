#include <SPI.h>

// Define SPI pins and other constants
#define MOSI 11
#define MISO 12
#define SCK  13
#define SS   7   // Slave select pin
#define BUSY 3   // BUSY pin to check if the slave is ready
#define BUTTON 2 // Button pin
#define RESET A0

void setup() {
  // Initialize SPI
  SPI.begin();

  // Configure SPI settings
  SPISettings settings(10000, MSBFIRST, SPI_MODE0); // Set baud rate, data order, and SPI mode

  // Start SPI transaction with defined settings
  SPI.beginTransaction(settings);
  
  // Set SPI pins as OUTPUT or INPUT
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(BUSY, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(RESET, OUTPUT);
  
  // Initialize serial communication
  Serial.begin(9600);

  // Set SS pin high (deasserted) initially
  digitalWrite(SS, HIGH); // Ensure SS starts HIGH

  //reset LR1110
  digitalWrite(RESET, LOW);
  delayMicroseconds(200);
  digitalWrite(RESET, HIGH);
}

void loop() {
  if (Serial.available() > 0) {
    if (Serial.read() == 0xA) {
      Serial.println("Starting...");
      SetTcxoMode();
      delay(100);
      getTemp();
      Serial.println("Done");
    }
  }
}

void SetTcxoMode() {
  uint8_t commandSPI[] = {0x01, 0x17, 0x02, 0x00, 0x00, 0xA3};
  sendSPICommandToSPI(commandSPI, sizeof(commandSPI) / sizeof(commandSPI[0]), 6);
}

void getTemp() {
  uint8_t commandSPI[] = {0x01, 0x1A};
  uint8_t* respondSPI = sendSPICommandToSPI(commandSPI, sizeof(commandSPI) / sizeof(commandSPI[0]), 3);

  // Extracting bits [15:8] and [7:0] and then [10:0]
  int rawTemperature_10_0 = ((respondSPI[1] << 8) | respondSPI[2]) & 0x7FF;

  float temperature = 25 + (1000/(-1.7)) * ((rawTemperature_10_0/2047.0) * 1.35 - 0.7295);
  Serial.print("LR1110 temperature: ");
  Serial.println(temperature);

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
    SPI.transfer(commands[i]);
  }
  digitalWrite(SS, HIGH); // De-assert SS, end of command sequence

  waitForBusyToGet(LOW);

  digitalWrite(SS, LOW);

  waitForBusyToGet(HIGH);

  for (int i = 0; i < responseLength; i++) {
    respondSPI[i] = SPI.transfer(0x00);
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

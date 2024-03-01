const int inputPin = 2;  // Input pin for detecting interrupts
volatile bool stateChanged = false;  // Flag to indicate state change

void setup() {
  pinMode(inputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(inputPin), pinChange, CHANGE);
  Serial.begin(9600);
}

void loop() {
  if (stateChanged) {
    Serial.println("State changed!");
    stateChanged = false;  // Reset the flag
  }
}

void pinChange() {
  stateChanged = true;  // Set the flag to indicate state change
}
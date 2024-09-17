const int ledPins[5] = {3, 5, 6, 9, 10};  // Pins for the 5 LEDs
const int numLEDs = 5;
int ledStates[numLEDs] = {0}; // Default state (off)
int previousBrightness = 0; // Default brightness (off)

// Flag to ensure LEDs only light up when commanded
bool updateLEDs = false;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    analogWrite(ledPins[i], 0); // Initialize with off state
  }
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    
    // Check for brightness command
    if (command.startsWith("BRIGHTNESS:")) {
      int brightness = command.substring(11).toInt();
      brightness = constrain(brightness, 0, 255);
      previousBrightness = brightness; // Update global brightness value
      Serial.print("Brightness Set: ");
      Serial.println(brightness);
      updateLEDs = true; // Enable LED update
    } 
    
    // Check for LED state command
    else if (command.startsWith("LED_STATE:")) {
      String states = command.substring(10);
      int ledStates[numLEDs];
      int i = 0;
      int value = 0;
      for (char c : states) {
        if (c == ',') {
          ledStates[i++] = value;
          value = 0;
        } else {
          value = value * 10 + (c - '0');
        }
      }
      ledStates[i] = value; // Last value

      // Update LED states based on received command
      for (int j = 0; j < numLEDs; j++) {
        if (ledStates[j] > 0) {
          analogWrite(ledPins[j], previousBrightness); // Set brightness based on previous value
        } else {
          analogWrite(ledPins[j], 0); // Turn off LED
        }
      }

      Serial.print("LED States: ");
      for (int j = 0; j < numLEDs; j++) {
        Serial.print(ledStates[j]);
        if (j < numLEDs - 1) Serial.print(",");
      }
      Serial.println();
      updateLEDs = false; // Disable further LED updates until a new command
    }
  }
}



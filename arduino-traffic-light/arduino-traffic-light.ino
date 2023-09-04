// ************ Description ************


// Name:        Elena Justo
// SID:         24429298
// Subject:     41099 Introduction to Mechatronics Engineering
// Assessment:  Assessment 1


// ************ Variables ************


// Component Variables
int inputButtonLeft = 2;
int inputButtonRight = 3;

int pedRedLed = 4;
int pedGreenLed = 5;

int trafRedLed = 6;
int trafYellowLed = 7;
int trafGreenLed = 8;

// Control Variables
bool sequenceStatus = false;
int stateNum = 1;
String buttonInput = "No button";  // Default to "No button"

int letterSecondA = 3;
int letterSecondB = 5;
int letterSecondC = 5;
int letterSecondD = 3;
int letterSecondE = 10;
int letterSecondF = 3;
int letterSecondG = 10;
int letterSecondH = 9;

String trafLedState = "GREEN"; // Starting state of traffic lights
String pedLedState = "RED";    // Starting state of pedestrian lights

unsigned long debounceDelay = 200; // Debounce delay in milliseconds

// Delay counter variables
unsigned long lastButtonPressTimeLeft = 0;
unsigned long lastButtonPressTimeRight = 0;

unsigned long previousPrintStatusTime = 0; 

unsigned long sequenceStartTime = 0;  // to keep track of when the sequence started
int sequenceStep = 0;  // to keep track of where we are in the sequence

const unsigned long FLASH_INTERVAL = 500; // 500 milliseconds = 0.5 seconds
unsigned long lastPedFlashTime = 0;  // To track the last time the pedestrian light was toggled
bool pedRedLedState = false; // False means off, true means on

// ************ Program Functions ************


void buttonListener(int buttonPin, const char* message, unsigned long &lastButtonPressTime) {
    if (digitalRead(buttonPin) == HIGH && millis() - lastButtonPressTime >= debounceDelay) {
        if (buttonPin == inputButtonLeft) {
            buttonInput = "Left button";
        } else if (buttonPin == inputButtonRight) {
            buttonInput = "Right button";
        }

        // Only initiate sequence if sequenceStatus is false
        if (!sequenceStatus) {
            activateSequence();
        }

        printStatus();  // Print status immediately when a button is pressed
        
        lastButtonPressTime = millis();
    }
}

void printStatus() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousPrintStatusTime >= 2000 || buttonInput != "No button") {  // Checking if a button was pressed or it's time to print
        Serial.print("Sequence active: "); Serial.print(sequenceStatus ? "Yes" : "No");
        Serial.print(" | Input: "); Serial.print(buttonInput);
        Serial.print(" | Current state number: "); Serial.print(stateNum);
        Serial.print(" | Traffic light status: "); Serial.print(trafLedState);
        Serial.print(" | Pedestrian light status: "); Serial.println(pedLedState);

        buttonInput = "No button";  // Reset the button input after printing
        previousPrintStatusTime = currentMillis;
    }
}

void executeState(int stateCurrent, int trafConfig, int pedConfig, int letterSeconds) {
  
  stateNum = stateCurrent; // Update state number

  trafficLightStateChanger(trafConfig); // Change traffic light

  pedLightStateChanger(pedConfig); // Change pedestrian light

  delay(letterSeconds); // Delay according to specified seconds
}


void trafficLightStateChanger(int lightToBeOn){
  if (lightToBeOn == trafRedLed){

    // Everything but red traffic LED is OFF
    digitalWrite(trafRedLed, HIGH);
    digitalWrite(trafYellowLed, LOW);
    digitalWrite(trafGreenLed, LOW);
    trafLedState = "RED";

  } else if (lightToBeOn == trafYellowLed) {

    // Everything but yellow traffic LED is OFF
    digitalWrite(trafRedLed, LOW);
    digitalWrite(trafYellowLed, HIGH);
    digitalWrite(trafGreenLed, LOW);
    trafLedState = "YELLOW";

  } else {

    // Everything but green traffic LED is OFF
    digitalWrite(trafRedLed, LOW);
    digitalWrite(trafYellowLed, LOW);
    digitalWrite(trafGreenLed, HIGH);
    trafLedState = "GREEN";

  };
}

void pedLightStateChanger(int lightToBeOn) {
  if (lightToBeOn == pedRedLed) {

    // Everything but red ped LED is OFF
    digitalWrite(pedRedLed, HIGH);
    digitalWrite(pedGreenLed, LOW);
    pedLedState = "RED";

  } else {

    // Everything but green ped LED is OFF
    digitalWrite(pedRedLed, LOW);
    digitalWrite(pedGreenLed, HIGH);
    pedLedState = "GREEN";

  };
}

void activateSequence() {
    sequenceStatus = true; // Set sequence as ongoing
    sequenceStartTime = millis();  // mark the start time of the sequence
    sequenceStep = 0;  // start with the first step
}

void checkAndAdvanceSequence() {
    if (sequenceStatus) {
        unsigned long elapsedTime = millis() - sequenceStartTime;

        switch (sequenceStep) {
            case 0:
                executeState(1, trafGreenLed, pedRedLed, 0);
                if (elapsedTime >= letterSecondA * 1000) {
                    sequenceStartTime = millis();
                    sequenceStep++;
                }
                break;

            case 1:
                executeState(2, trafYellowLed, pedRedLed, 0);
                if (elapsedTime >= letterSecondB * 1000) {
                    sequenceStartTime = millis();
                    sequenceStep++;
                }
                break;

            case 2:
                executeState(3, trafRedLed, pedRedLed, 0);
                if (elapsedTime >= letterSecondC * 1000) {
                    sequenceStartTime = millis();
                    sequenceStep++;
                }
                break;

            case 3:
                executeState(4, trafRedLed, pedGreenLed, 0);
                if (elapsedTime >= letterSecondD * 1000) {
                    sequenceStartTime = millis();
                    sequenceStep++;
                }
                break;

            case 4:
               digitalWrite(pedGreenLed, LOW);  

                if (elapsedTime < letterSecondE * 1000) {  // While we're in the duration of letterSecondE
                    if (millis() - lastPedFlashTime >= FLASH_INTERVAL) {  // Check if it's time to toggle the LED
                        if (pedRedLedState) { 
                            digitalWrite(pedRedLed, LOW);  // Turn off
                            pedRedLedState = false;
                            pedLedState = "FLASHING";  // Debugssss
                        } else {
                            digitalWrite(pedRedLed, HIGH); // Turn on
                            pedRedLedState = true;
                            pedLedState = "FLASHING";  // Debugssss
                        }
                        lastPedFlashTime = millis();  // Update the last toggle time
                    }
                } else {
                    sequenceStartTime = millis();
                    sequenceStep++;
                }
                break;

            case 5:
                executeState(6, trafRedLed, pedRedLed, 0);
                if (elapsedTime >= letterSecondG * 1000) {
                    sequenceStartTime = millis();
                    sequenceStep++;
                }
                break;

            case 6:
                executeState(7, trafGreenLed, pedRedLed, 0);
                if (elapsedTime >= letterSecondH * 1000) {
                    sequenceStatus = false;  // end the sequence
                }
                break;
        }
    }
}


// ************ Program Loops ************


void setup() {
  Serial.begin(9600);

  // Setting button pins to read
  pinMode(inputButtonLeft, INPUT);
  pinMode(inputButtonRight, INPUT);

  // Setting LED pins to output
  pinMode(pedRedLed, OUTPUT);
  pinMode(pedGreenLed, OUTPUT);

  pinMode(trafRedLed, OUTPUT);
  pinMode(trafYellowLed, OUTPUT);
  pinMode(trafGreenLed, OUTPUT);

  // Starting LED states
  trafficLightStateChanger(trafGreenLed);
  pedLightStateChanger(pedRedLed);
}

void loop() {
  // Listening for button presses
  buttonListener(inputButtonLeft, "Left button pressed!", lastButtonPressTimeLeft);
  buttonListener(inputButtonRight, "Right button pressed!", lastButtonPressTimeRight);

  checkAndAdvanceSequence();  // check if we need to advance in our sequence
  printStatus();  // Regular status print every 2 seconds
}


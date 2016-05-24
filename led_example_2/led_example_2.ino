const int buttonPin = 2;
const int ledPin =  13;

int buttonState = 0;
boolean buttonPressed = false;
boolean ledLit = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    buttonPressed = true;
  } 
  else if (buttonPressed) {
    if (ledLit) {
      digitalWrite(ledPin, LOW);
      ledLit = false;
    }
    else {
      digitalWrite(ledPin, HIGH);
      ledLit = true;
    }
    buttonPressed = false;
  }
}


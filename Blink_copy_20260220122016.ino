const int ledPin = 8;
const int buttonPin = 9;   // ATmega pin 15

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(ledPin, LOW);
}

void loop() {

  while (digitalRead(buttonPin) == HIGH) {
    digitalWrite(ledPin, LOW);   // keep LED off
  }

  while (digitalRead(buttonPin) == LOW) {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
  }

}

//Ben Trudgen


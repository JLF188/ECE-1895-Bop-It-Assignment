const int buttonPin = 9;   

const int led1 = A3;       
const int led2 = A4;       
const int led3 = A5;      

void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() 
{

  while (digitalRead(buttonPin) == HIGH) 
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }

  while (digitalRead(buttonPin) == LOW) 
  {

    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);

    if (digitalRead(buttonPin) == HIGH) break;

    digitalWrite(led2, HIGH);
    delay(1000);
    digitalWrite(led2, LOW);

    if (digitalRead(buttonPin) == HIGH) break;

    digitalWrite(led3, HIGH);
    delay(1000);
    digitalWrite(led3, LOW);
  }
}
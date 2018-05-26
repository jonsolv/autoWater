int pwmPin = 10;


void setup() {
  // put your setup code here, to run once:
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
analogWrite(pwmPin, 170);
delay(4000);
analogWrite(pwmPin, 190);
delay(4000);
analogWrite(pwmPin, 210);
delay(4000);
}

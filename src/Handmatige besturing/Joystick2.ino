#define VRY_PIN A2

int pwmA = 3;
int dirA = 12;

int yValue = 0;

void setup() {
  // put your setup code here, to run once:
  TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  leesJoystick();
  handmatigBewegen();
  Serial.println(yValue);
}

void leesJoystick() {
  yValue = analogRead(VRY_PIN);
}

void handmatigBewegen() {
  if (yValue > 700) {
    analogWrite(pwmA, 127);
    digitalWrite(dirA, LOW);
  }
  
  else if (yValue < 300) {
    analogWrite(pwmA, 127);
    digitalWrite(dirA, HIGH);
  }
  
  else {
    analogWrite(pwmA, LOW);
  }
}
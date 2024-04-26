#define VRX_PIN  A2 // Arduino pin connected to VRX pin
#define VRY_PIN  A3 // Arduino pin connected to VRY pin

int pwmA = 3;
int dirA = 12;

int pwmB = 11;
int dirB = 13;

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis

String richting = "";

void setup() {
  TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(dirB, OUTPUT);
  Serial.begin(9600) ;
}

void loop() {
  // read analog X and Y analog values
  leesJoystick();
  geefRichting();
  handmatigBewegen();
}

void leesJoystick() {
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
}

void geefRichting() {
  richting = "";
  if (xValue > 700) {
    richting += "1";
  }

  if (xValue < 300) {
    richting += "2";
  }

  if (yValue > 700) {
    richting += "3";
  }

  if (yValue < 300) {
    richting += "4";
  }

  if (richting == "") {
    richting += "0";
  }

  Serial.println(richting);
}

void handmatigBewegen() {
  int var = richting.toInt();
  switch (var) {
    case 2:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, LOW);
      analogWrite(pwmB, 0);
      break;

    case 1:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, HIGH);
      digitalWrite(pwmB, 0);
      break;
    
    case 4:
      analogWrite(pwmB, 255);
      digitalWrite(dirB, LOW);
      analogWrite(pwmA, 0);
      break;
    
    case 3:
      analogWrite(pwmB, 127);
      digitalWrite(dirB, HIGH);
      analogWrite(pwmA, 0);
      break;
    
    case 14:
      analogWrite(pwmA, 127);
      analogWrite(pwmB, 255);
      digitalWrite(dirA, HIGH);
      digitalWrite(dirB, LOW);
      break;

    case 24:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, LOW);
      analogWrite(pwmB, 255);
      digitalWrite(dirB, LOW);
      break;
  
    case 23:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, LOW);  
      analogWrite(pwmB, 127);
      digitalWrite(dirB, HIGH);
      break;

    case 13:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, HIGH);
      analogWrite(pwmB, 127);
      digitalWrite(dirB, HIGH);
      break;

    default:
      analogWrite(pwmA, 0);
      analogWrite(pwmB, 0);
  }
}

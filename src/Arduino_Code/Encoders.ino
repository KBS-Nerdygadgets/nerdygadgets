#define VRX_PIN  A2 // Arduino pin connected to VRX pin
#define VRY_PIN  A3 // Arduino pin connected to VRY pin

const int snelheid = 255;

const int pwmA = 3;
const int dirA = 12;

const int pwmB = 11;
const int dirB = 13;

const int XencoderPin = 2;
const int XrichtingPin = 4;

int Xencoder = 0;

const int xValue = 0; // To store value of the X axis
const int yValue = 0; // To store value of the Y axis

String richting = "";

//753 pulsen per coordinaat
const int coordinaten[5][2] = {
  {155, 0},   //5:1
  {911, 0},   //5:2
  {1664, 0},  //5:3
  {2417, 0},  //5:4
  {3170, 0}   //5:5
}

void setup() {
  TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(dirB, OUTPUT);

  pinMode(XencoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(XencoderPin), leesEncoder, RISING);
  Serial.begin(9600);
}

void loop() {
  // handmatigeStatus();
  Serial.println(Xencoder);
}

void gaNaarCoordinaat(int coordinaatIndex){
  coordinaat = coordinaten[coordinaatIndex]
  //beweeg naar links als coordinaat zich links bevind
  if(Xencoder > coordinaat[0]){
    analogWrite(pwmA, snelheid);
    digitalWrite(dirA, LOW);
    analogWrite(pwmB, 0);
  }
  //beweeg naar rechts als coordinaat zich rechts bevind
  else if(Xencoder < coordinaat[0]){
    analogWrite(pwmA, snelheid);
    digitalWrite(dirA, HIGH);
    analogWrite(pwmB, 0);
  }
  //stop
  else{
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);
  }
}

void leesEncoder() {
  if (digitalRead(XrichtingPin) == 1) {
    Xencoder++;
  } else {
    Xencoder--;
  }
}

void handmatigeStatus(){
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

  // Serial.println(richting);
}

void handmatigBewegen() {
  int var = richting.toInt();
  switch (var) {
    //X naar Rechts
    case 1:
      analogWrite(pwmA, snelheid/2);
      digitalWrite(dirA, HIGH);
      analogWrite(pwmB, 0);
      break;

    //X naar Links
    case 2:
      analogWrite(pwmA, snelheid/2);
      digitalWrite(dirA, LOW);
      analogWrite(pwmB, 0);
      break;

    //Y naar Beneden
    case 3:
      analogWrite(pwmB, snelheid/2);
      digitalWrite(dirB, HIGH);
      analogWrite(pwmA, 0);
      break;

    //Y naar Boven
    case 4:
      analogWrite(pwmB, snelheid/2);
      digitalWrite(dirB, LOW);
      analogWrite(pwmA, 0);
      break;
      
    //rechts omhoog
    case 14:
      analogWrite(pwmA, snelheid/2);
      digitalWrite(dirA, HIGH);
      analogWrite(pwmB, snelheid/2);
      digitalWrite(dirB, LOW);
      break;

    //links omhoog
    case 24:
      analogWrite(pwmA, snelheid/2);
      digitalWrite(dirA, LOW);
      analogWrite(pwmB, snelheid/2);
      digitalWrite(dirB, LOW);
      break;

    //links omlaag
    case 23:
      analogWrite(pwmA, snelheid/2);
      digitalWrite(dirA, LOW);  
      analogWrite(pwmB, snelheid/2);
      digitalWrite(dirB, HIGH);
      break;

    //rechts omlaag
    case 13:
      analogWrite(pwmA, snelheid/2);
      digitalWrite(dirA, HIGH);
      analogWrite(pwmB, snelheid/2);
      digitalWrite(dirB, HIGH);
      break;

    default:
      analogWrite(pwmA, 0);
      analogWrite(pwmB, 0);
  }
}
#include <SoftwareSerial.h>

#define VRX_PIN  A2 // Arduino pin connected to VRX pin
#define VRY_PIN  A3 // Arduino pin connected to VRY pin

const int snelheid = 255;
const int snelheidHandmatig = snelheid/2;

const int pwmA = 3; //Xas
const int dirA = 12;
const int XencoderPin = 2;
const int XrichtingPin = 4;
int Xencoder = 0;
int xValue = 0; // To store value of the X axis

const int pwmB = 11; //Yas
const int dirB = 13;
const int brakeB = 8;
int yValue = 0; // To store value of the Y axis

const int resetKnopEncoder = 5;
const int knopSwitchStatus = 6;

bool status = true;

String richting = "";

//753 pulsen per coordinaat
const int coordinaten[5][2] = {
  {155, 0},   //5:1
  {911, 0},   //5:2
  {1664, 0},  //5:3
  {2417, 0},  //5:4
  {3170, 0}   //5:5
};

void setup() {
  delay(1200);
  TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

  //Pins voor motoren
  //X
  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(XencoderPin, INPUT_PULLUP);
  //Y
  pinMode(pwmB, OUTPUT);
  pinMode(dirB, OUTPUT);

  //Pins voor knoppen
  pinMode(resetKnopEncoder, INPUT_PULLUP);
  pinMode(knopSwitchStatus, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(XencoderPin), leesEncoder, RISING);
  Serial.begin(9600);
}

void loop() {
  isKnopIngedrukt();
  if(status == Handmatig){
    handmatigeStatus();
  }
  //Hou de bovenste knop ingedrukt om de automatische mode te activeren
  else if(status == Automatisch){
    gaNaarCoordinaat(3);
  }
  //Druk de onderste knop in om de encoder te resetten. Dit moet op het nulpunt gebeuren
  Serial.println(Xencoder);
}

void isKnopIngedrukt(){
  status = Handmatig;
  //Encoder wordt gereset;
  if(digitalRead(resetKnopEncoder) == 0){
    Xencoder = 0;
  }
  //Status wordt op automatisch gezet
  if(digitalRead(knopSwitchStatus) == 0){
      status = Automatisch;
  }
}

void gaNaarCoordinaat(int coordinaatIndex){
  digitalWrite(brakeB, LOW);
  //beweeg naar links als coordinaat zich links bevind
  if(Xencoder > coordinaten[coordinaatIndex][0]){
    analogWrite(pwmB, 0);
    analogWrite(pwmA, snelheid);
    digitalWrite(dirA, LOW);
  }
  //beweeg naar rechts als coordinaat zich rechts bevind
  else if(Xencoder < coordinaten[coordinaatIndex][0]){
    analogWrite(pwmB, 0);
    analogWrite(pwmA, snelheid);
    digitalWrite(dirA, HIGH);
  }
  else{
  //stop
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 0);
    digitalWrite(brakeB, HIGH);
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
  //Joystick naar rechts
  if (xValue > 700) {
    richting += "1";
  }
  //Joystick naar links
  if (xValue < 300) {
    richting += "2";
  }
  //Joystick naar boven
  if (yValue > 700) {
    richting += "3";
  }
  //Joystick naar beneden
  if (yValue < 300) {
    richting += "4";
  }
  //Stop
  if (richting == "") {
    richting += "0";
  }
}

void handmatigBewegen() {
  //Zet de string om in een Int
  int var = richting.toInt();
  switch (var) {
    //X naar Rechts
    case 1:
      analogWrite(pwmA, snelheidHandmatig);
      digitalWrite(dirA, HIGH);
      analogWrite(pwmB, 0);
      break;

    //X naar Links
    case 2:
      analogWrite(pwmA, snelheidHandmatig);
      digitalWrite(dirA, LOW);
      analogWrite(pwmB, 0);
      break;

    //Y naar Beneden
    case 3:
      analogWrite(pwmB, snelheidHandmatig);
      digitalWrite(dirB, HIGH);
      analogWrite(pwmA, 0);
      break;

    //Y naar Boven
    case 4:
      analogWrite(pwmB, snelheidHandmatig);
      digitalWrite(dirB, LOW);
      analogWrite(pwmA, 0);
      break;
      
    //rechts omhoog
    case 14:
      analogWrite(pwmA, snelheidHandmatig);
      digitalWrite(dirA, HIGH);
      analogWrite(pwmB, snelheidHandmatig);
      digitalWrite(dirB, LOW);
      break;

    //links omhoog
    case 24:
      analogWrite(pwmA, snelheidHandmatig);
      digitalWrite(dirA, LOW);
      analogWrite(pwmB, snelheidHandmatig);
      digitalWrite(dirB, LOW);
      break;

    //links omlaag
    case 23:
      analogWrite(pwmA, snelheidHandmatig);
      digitalWrite(dirA, LOW);  
      analogWrite(pwmB, snelheidHandmatig);
      digitalWrite(dirB, HIGH);
      break;

    //rechts omlaag
    case 13:
      analogWrite(pwmA, snelheidHandmatig);
      digitalWrite(dirA, HIGH);
      analogWrite(pwmB, snelheidHandmatig);
      digitalWrite(dirB, HIGH);
      break;

    //Stop
    default:
      analogWrite(pwmA, 0);
      analogWrite(pwmB, 0);
  }
}
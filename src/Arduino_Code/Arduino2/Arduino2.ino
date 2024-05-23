#include <Wire.h>

#define VRY_PIN A2

////////////////////////////
// *** ORANJE ARDUINO MASTER*** //
////////////////////////////

// *** ALLE PINS *** //
//Encoder
const int YencoderPin = 2;
const int YrichtingPin = 4;
int Yencoder = 0;

bool yasOmhoog = 0;
bool bijCoordinaatAangekomen = 0;
bool resettenYencoder = 0;

int yValue = 0;
int snelheid = 255;

// Pins ledjes
const int redLED = 8;
const int yellowLED = 11;
const int greenLED = 13;

//Motorpins voor motor z-as
const int pwmA = 3;
const int dirA = 12;

// Afstandsensorpin voor z-as
const int distanceSensorZ = A3;

// Afstandsensorenpins voor magazijn
const int distanceSensorL = A0;

// Noodstop knoppen
const int buttonNoodStop = 5;
const int buttonNoodStopReset = 6;

// *** VARIABLEN *** ///
// Modus
enum Modus {STOP, HANDMATIG, AUTOMATISCH};
Modus huidigeModus = HANDMATIG;

//variabel voor afstandsensor
bool voorSafe = false;
bool achterSafe = false;

// Variable voor uitgeschovend detectie
bool uitgeschoven = false;

// NOODSTOP
bool noodStop = false;
bool handmatig = true;
bool automatisch = false;

int aantalProducten = 0;
int tijd = 0;
int delayOmhoog = 800;
bool omhoogGegaan = false;

String tweeNaarEen = "000000";
String input = "";

// *** seriele communicatie *** //
unsigned long sendmessageMillis = 0;

const int slaveAddress1 = 8;
const int slaveAddress2 = 9;
// *** seriele communicatie end *** //

void setup() {
  // put your setup code here, to run once:
  delay(1500);
  TCCR2B = TCCR2B & B11111000 | B00000110;  // for PWM frequency of 122.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

  //Pins voor motoren
  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(YencoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(YencoderPin), leesEncoder, RISING);

  

  //pinMode afstandsensoren
  pinMode(distanceSensorZ, INPUT);
  pinMode(distanceSensorL, INPUT);

  // Button noodstopreset
  pinMode(buttonNoodStopReset, INPUT_PULLUP);
  pinMode(buttonNoodStop, INPUT_PULLUP);

  // Ledjes
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  updateLEDs();

  Wire.begin(slaveAddress2);
  Wire.onReceive(serialRead);
  Wire.onRequest(serialWrite);
  Serial.begin(9600);
}

void loop() {
  functiesSensoren();
  switch(huidigeModus){
    case HANDMATIG:
      handmatigBewegen();
      break;
    case STOP:
      noodstopReset();
      break;
    case AUTOMATISCH:
      //functies automatisch
      break;
  }
  // Serial.println(Yencoder);
  serialRead();
  functiesStatussen();
}

//*Functies voor de communicatie tussen Arduinos
void serialRead() {
  input = "";
  Wire.requestFrom(slaveAddress1, 3);
  while (Wire.available()) {
    char c = Wire.read();
    input += c;
  }
  Serial.println(input);
  leesString();
  // delay(5);
}

void serialWrite(){
  encoderInString();
  Wire.write(tweeNaarEen.c_str());
  // delay(5);
}

void leesString() {
  //belangrijk stukje
  Serial.println(input);
  yasOmhoog = input.substring(0).toInt();
  bijCoordinaatAangekomen = input.substring(1).toInt();
  resettenYencoder = input.substring(2).toInt();
  if(resettenYencoder == 1){ //reset Yencoder op basis van input van Arduino 1
    Yencoder = 0;
  }
  delay(20);
  //belangrijk stukje einde
}

void leesEncoder() {
  if (digitalRead(YrichtingPin) == 0) {
    Yencoder++;
  } else {
    Yencoder--;
  }
}

void encoderInString() {
  String x = String(Yencoder);
  if (Yencoder > -1 && Yencoder < 10) {
    tweeNaarEen.setCharAt(4, x.charAt(0));
    for (int i = 1; i < 5; i++) {
      tweeNaarEen.setCharAt(i, 48);
    }
  }
  else if (Yencoder > -1 && Yencoder < 100) {
    tweeNaarEen.setCharAt(4, x.charAt(1));
    tweeNaarEen.setCharAt(3, x.charAt(0));
    tweeNaarEen.setCharAt(2, 48);
    tweeNaarEen.setCharAt(1, 48);
  }
  else if (Yencoder > -1 && Yencoder < 1000) {
    tweeNaarEen.setCharAt(4, x.charAt(2));
    tweeNaarEen.setCharAt(3, x.charAt(1));
    tweeNaarEen.setCharAt(2, x.charAt(0));
    tweeNaarEen.setCharAt(1, 48);    
  }
  else if(Yencoder > -1) {
    for (int i = 4; i > 0; i--) {
      tweeNaarEen.setCharAt(i, x.charAt(i-1));
    }
  }
  else {
    for(int i = 1; i < 5; i++){
      tweeNaarEen.setCharAt(i, 48);
    }
  }
}

//*Sensoren
unsigned long previousMillis = 0; // Variabele om de tijd bij te houden van de laatste keer dat de sensor is uitgelezen
const unsigned long interval = 200; // Interval van 100 milliseconden
void leesDistanceSensorZ(){
  unsigned long currentMillis = millis(); // Huidige tijd ophalen

  // CONTROLLEER OF HET 100MS IS GEWEEST SINDS DE LAATSTE METING
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Reset de timer

    float volts = analogRead(distanceSensorZ);// Value van de sensor in var zetten
  
    // PRINT AFSTAND NAAR SERIELE MONITOR
    // Serial.print("Afstand vork: "); Serial.println(volts); //distance is tussen 650 en 300
    
    if(volts >= 640){
      achterSafe = true;
      uitgeschoven = false;
      tweeNaarEen.setCharAt(0, 48); //Set 0, stuur naar Arduino 1 dat zas naar binnen staat
      //Serial.println("achter");
    }
    if (volts < 310){
      voorSafe = true;
      //Serial.println("voor");
    }
    if ( volts > 311 && volts < 640){ // NIet compleet accuraat, bijwerken
      //Serial.println("tussen 300 en 640");
      voorSafe = false;
      achterSafe = false;
  }
    if (volts < 640){
      uitgeschoven = true;
      tweeNaarEen.setCharAt(0, 49); //Set 1, Stuur naar arduino 1 dat zas naar buiten staat
    } 
  }
} 

unsigned long previousMillis2 = 0; // Variabele om de tijd bij te houden van de laatste keer dat de sensor is uitgelezen
const unsigned long interval2 = 200; // Interval van 100 milliseconden
void leesDistanceSchap(){
  unsigned long currentMillis = millis(); // Haal de huidige tijd op

  // Controleer of er 100 milliseconden zijn verstreken sinds de laatste meting
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis; // Reset de timer

    float afstandLinks = analogRead(distanceSensorL); //Value van de sensor in var zetten
  
    // Print de afstand naar de seriële monitor
    Serial.print("Afstand links: "); Serial.print(afstandLinks);
    Serial.println();
    // Serial.print(" & rechts: "); Serial.println(afstandRechts);

    if(afstandLinks < 250){
      Serial.println("NOODSTOP");
      analogWrite(pwmA, 0);
      noodStop = true;
      huidigeModus = STOP; updateLEDs(); // Ledjes veranderen en updaten
    }
  }
}

void functiesSensoren(){
  leesDistanceSensorZ();
  // leesDistanceSchap();
}

//*Statussen
// STOPLICHT LAMPIES
void updateLEDs() {
  // Turn off all LEDs
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);

  // LAMPJES SWITCHEN
  switch (huidigeModus) {
    case STOP:
      digitalWrite(redLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, LOW);
      noodStop = true;
      automatisch = false;
      handmatig = false;
      break;
    case HANDMATIG:
      digitalWrite(yellowLED, HIGH);
      digitalWrite(greenLED, LOW);
      handmatig = true;
      noodStop = false;
      automatisch = false;
      break;
    case AUTOMATISCH:
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);
      automatisch = true;
      handmatig = false;
      noodStop = false;
      break;
  }
}

void noodstopReset() {
  bool noodStopReset = digitalRead(buttonNoodStopReset);

  if(noodStopReset == LOW){
    if(noodStop == true){
      noodStop = false;
      huidigeModus = HANDMATIG; updateLEDs(); // Ledjes veranderen en updaten
      // Ga naar begin punt functie hier
    }
  }
}

void noodStopInitiatie(){
  bool noodStopStart = digitalRead(buttonNoodStop);
  if(noodStopStart == LOW){
    huidigeModus = STOP; updateLEDs(); // Ledjes veranderen en updaten
  }
}

void stuurStatus(){
  switch (huidigeModus) {
    case STOP:
      tweeNaarEen.setCharAt(5, 48); //Set getal 0
      break;
    case HANDMATIG:
      tweeNaarEen.setCharAt(5, 49); //Set getal 1
      break;
    case AUTOMATISCH:
      tweeNaarEen.setCharAt(5, 50); //Set getal 2
      break;
  }
}

//Gebruikt de bovenste gele knop om tussen handmatige en automatische mode te wisselen
unsigned long previousMillis3 = 0; // Variabele om de tijd bij te houden van de laatste keer dat de sensor is uitgelezen
const unsigned long interval3 = 400; // Interval van 200 milliseconden
void setStatus(){
  unsigned long currentMillis = millis(); // Haal de huidige tijd op
  // Controleer of er 100 milliseconden zijn verstreken sinds de laatste meting
  if (currentMillis - previousMillis3 >= interval3) {
    previousMillis3 = currentMillis; // Reset de timer
    bool setStatusBool = digitalRead(buttonNoodStopReset);
    
    if(setStatusBool == LOW && noodStop == false){
      switch (huidigeModus) {
        case HANDMATIG:
          huidigeModus = AUTOMATISCH; updateLEDs();
          break;
        case AUTOMATISCH:
          huidigeModus = HANDMATIG; updateLEDs();
          break;
      }
    }
  }

}

void functiesStatussen(){
  noodStopInitiatie();
  noodstopReset();
  setStatus();
  stuurStatus();
}

void leesJoystick() {
  yValue = analogRead(VRY_PIN);
}

void handmatigBewegen() {
  if(noodStop == false){
    leesJoystick();
    // Achteruit
    if (yValue > 700 && achterSafe == false ) {
      analogWrite(pwmA, 127);
      digitalWrite(dirA, HIGH);
    }
    // Vooruit
    else if (yValue < 300 && voorSafe == false) {
      analogWrite(pwmA, 127);
      digitalWrite(dirA, LOW);
    }

    // Stop
    else {
      analogWrite(pwmA, LOW);
    }
  }
}

void pakProduct() {
  if (aantalProducten == 0) {
    tijd = 1100;
  }

  else if (aantalProducten == 1) {
    tijd = 1000;
  }

  else if (aantalProducten == 2) {
    tijd = 500;
  }

  digitalWrite(dirA, LOW);
  analogWrite(pwmA, snelheid);
  delay(tijd);

  omhoogGegaan = false;
  tweeNaarEen.setCharAt(0, 49); //Set 1

  while (omhoogGegaan == false) {
    analogWrite(pwmA, 0);
  }
  analogWrite(pwmA, snelheid);
  digitalWrite(dirA, HIGH);
  delay(tijd);

  analogWrite(pwmA, 0);

  bijCoordinaatAangekomen = false;
  aantalProducten++;
}
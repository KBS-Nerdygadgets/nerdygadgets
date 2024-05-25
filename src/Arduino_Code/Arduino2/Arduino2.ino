#include <Wire.h>

#define VRY_PIN A2

////////////////////////////
// *** ORANJE ARDUINO *** //
////////////////////////////

// *** PINS *** //
//Encoder
const int YencoderPin = 2;
const int YrichtingPin = 4;
volatile int Yencoder = 0;

int yValue = 0;
int snelheid = 255;
int snelheidHandmatig = snelheid/2;

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
const int msSchap = 7;

// Noodstop knoppen
const int buttonNoodStop = 5;
const int buttonNoodStopReset = 6;
const int buttonSetStatus = A1;

// *** VARIABLEN *** ///
// Modus
enum Modus {STOP, HANDMATIG, AUTOMATISCH};
Modus huidigeModus = HANDMATIG;

enum Richting {Binnen, Buiten};

//variabel voor afstandsensor
bool voorSafe = false;
bool achterSafe = false;

// Variable voor uitgeschovend detectie
bool uitgeschoven = false;

// NOODSTOP
bool noodStop = false;
bool handmatig = true;
bool automatisch = false;

//begin
bool klaar = false;
int aantalProducten = 0;
int tijd = 0;
int delayOmhoog = 800;
bool omhoogGegaan = false;

String tweeNaarEen = "000000";
String input = "";

//Seriele communicatie
const int slaveAddress1 = 8;
const int slaveAddress2 = 9;

bool yasOmhoog = 0;
bool bijCoordinaatAangekomen = 0;
bool resettenYencoder = 0;

unsigned long lastButtonPressTime = 0; // Bewaard de tijd wanneer de knop het laatst is gebruikt
const unsigned long debounceDelay = 200; // Delay voor de knop

//*Setup
void setup() {
  // put your setup code here, to run once:
  delay(1500);
  TCCR2B = TCCR2B & B11111000 | B00000110;  // for PWM frequency of 122.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

  //pinMode motoren
  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(YencoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(YencoderPin), leesEncoder, RISING);

  //pinMode afstandsensoren
  pinMode(distanceSensorZ, INPUT);
  pinMode(distanceSensorL, INPUT);

  //pinMode schapMicroSwitch
  pinMode(msSchap, INPUT_PULLUP);

  // Button noodstopreset
  pinMode(buttonNoodStopReset, INPUT_PULLUP);
  pinMode(buttonNoodStop, INPUT_PULLUP);

  // Ledjes
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  updateLEDs();

  //Seriele communicatie setup
  Wire.begin(slaveAddress2);
  Wire.onReceive(serialRead);
  Wire.onRequest(serialWrite);
  Serial.begin(9600);
}

//*Loop
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
      if(bijCoordinaatAangekomen){
        pakProduct();
      }
      break;
  }
  serialRead();
  functiesStatussen();
  // Serial.println(Yencoder);
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
}

void serialWrite(){
  encoderInString();
  Wire.write(tweeNaarEen.c_str());
}

void leesString() {
  Serial.println(input);
  yasOmhoog = input.substring(0).toInt();
  bijCoordinaatAangekomen = input.substring(1).toInt();
  resettenYencoder = input.substring(2).toInt();
  if(resettenYencoder == 1){ //reset Yencoder op basis van input van Arduino 1
    Yencoder = 0;
  }
  delay(20);
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

//*Bewegen
void moveZ(Richting richting, int snelheid){
  //rechts
  if(richting == Binnen && uitgeschoven == false){
    analogWrite(pwmA, 0)
  }
  //links
  else if(richting == Buiten && uitgeschoven == true){
    analogWrite(pwmA, 0);
  }
  else{
    analogWrite(pwmA, snelheid);
    if(richting == Binnen){
      digitalWrite(dirA, HIGH);
    }
    if(richting == Buiten){
      digitalWrite(dirA, LOW);
    }
  }
}

//*Statussen
void functiesStatussen(){
  noodStopInitiatie();
  noodstopReset();
  setStatus();
  stuurStatus();
}

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

unsigned long previousMillis3 = 0; // Variabele om de tijd bij te houden van de laatste keer dat de sensor is uitgelezen
const unsigned long interval3 = 50; // Interval van 100 milliseconden
void noodstopReset() {
  unsigned long currentMillis = millis(); // Haal de huidige tijd op
  bool microSchap = analogRead(msSchap); //Value van de sensor in var zetten
  bool noodStopReset = digitalRead(buttonNoodStopReset); // status van de noodstop resetknop in een var zetten
  
    // Controleer of er 100 milliseconden zijn verstreken sinds de laatste meting
    if (currentMillis - previousMillis3 >= interval3) {
      previousMillis3 = currentMillis; // Reset de timer

    // Controleer of de knop is ingedrukt, het schap op zijn plek staat en of de debounce tijd voorbij is
    if(noodStopReset == LOW && microSchap == HIGH && noodStop == true && (currentMillis - lastButtonPressTime >= debounceDelay)){
      lastButtonPressTime = currentMillis; // Update de laatste knopdruk tijd
      Serial.println("Noodstop reset");
      if(noodStop == true){
        
        // Ga naar begin punt functie hier zetten
        beginSituatie();
        // het onderstaande willen we pas doen na dat de robot op zijn beginplek staat
        noodStop = false; 
        huidigeModus = HANDMATIG; updateLEDs(); // Ledjes updaten en modus veranderen
      }
    }
  }
}

void noodStopInitiatie(){
  bool noodStopStart = digitalRead(buttonNoodStop);
  if(noodStopStart == LOW){
    huidigeModus = STOP; updateLEDs(); // Ledjes veranderen en updaten
    Serial.println("Handmatige noodstop");
    noodStop = true;
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

//TODO fixen status knop
//Gebruikt de bovenste gele knop om tussen handmatige en automatische mode te wisselen
unsigned long previousMillis3 = 0; // Variabele om de tijd bij te houden van de laatste keer dat de sensor is uitgelezen
const unsigned long interval3 = 300; // Interval van 200 milliseconden
void setStatus(){
  unsigned long currentMillis = millis(); // Haal de huidige tijd op
  // Controleer of er 100 milliseconden zijn verstreken sinds de laatste meting
  if (currentMillis - previousMillis3 >= interval3) {
    previousMillis3 = currentMillis; // Reset de timer
    bool setStatusBool = digitalRead(buttonNoodStopReset);
    
    if(setStatusBool == LOW && noodStop == false){
      switch (huidigeModus) {
        case HANDMATIG:
          huidigeModus = AUTOMATISCH;
          updateLEDs();
          break;
        case AUTOMATISCH:
          huidigeModus = HANDMATIG;
          updateLEDs();
          break;
      }
    }
  }
}

//* Handmatig Functies
void leesJoystick() {
  yValue = analogRead(VRY_PIN);
}

void handmatigBewegen() {
  if(noodStop == false){
    leesJoystick();
    // Achteruit
    if (yValue > 700) {
      moveZ(Binnen, snelheidHandmatig)
    }
    // Vooruit
    else if (yValue < 300) {
      moveZ(Buiten, snelheidHandmatig);
    }
    // Stop
    else {
      analogWrite(pwmA, 0);
    }
  }
}

//*Automatisch Functies
void pakProduct() {
  if(!uitgeschoven && achterSafe){
    uitschuivenZas();
  }

  else if(uitgeschoven){
    analogWrite(pwmA, 0);
    tweeNaarEen.setCharAt(0, 49); //Set Z-as uitgeschoven op true
    if(yasOmhoog){
      //Achteruit
      moveZ(Binnen, snelheid);
      delay(tijd);
      analogWrite(pwmA, 0);
      bijCoordinaatAangekomen = false;
      aantalProducten++;
    }
  }

  else{
    Serial.println("Pakproduct doet het niet swa")
  }
}

void zAsUit(){
  if (aantalProducten == 0) {
    tijd = 1100;
  }

  else if (aantalProducten == 1) {
    tijd = 1000;
  }

  else if (aantalProducten == 2) {
    tijd = 500;
  }

  //Zas vooruit
  moveZ(Buiten, snelheid);
  delay(tijd);
}

//*Sensoren
void functiesSensoren(){
  leesDistanceSensorZ();
  leesMicroSchap();
}

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
      
    //De Z as staat helemaal naar binnen
    if(volts >= 640){
      achterSafe = true;
      uitgeschoven = false;
      tweeNaarEen.setCharAt(0, 48); //Set 0, stuur naar Arduino 1 dat zas naar binnen staat
    }
    else if (volts < 310){
      voorSafe = true;
    }
    else if ( volts > 311 && volts < 640){ // NIet compleet accuraat, bijwerken
      voorSafe = false;
      achterSafe = false;
    }
    //De Z as staat helemaal naar buiten
    if (volts < 640){
      uitgeschoven = true;
      tweeNaarEen.setCharAt(0, 49); //Set 1, Stuur naar arduino 1 dat zas naar buiten staat
    } 
  }
} 

unsigned long previousMillis2 = 0; // Variabele om de tijd bij te houden van de laatste keer dat de sensor is uitgelezen
const unsigned long interval2 = 500; // Interval van 100 milliseconden
void leesMicroSchap(){
  unsigned long currentMillis = millis(); // Haal de huidige tijd op

  // Controleer of er 100 milliseconden zijn verstreken sinds de laatste meting
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis; // Reset de timer
    //Serial.println("Testschap1"); //testen
    bool microSchap = digitalRead(msSchap); //Value van de sensor in var zetten
    // Print de afstand naar de seriÃ«le monitor
    
    if(microSchap == LOW){
      //Serial.println("Testschap2"); //testen
      Serial.println("Automatische noodstop");
      analogWrite(pwmA, 0);
      noodStop = true;
      huidigeModus = STOP; updateLEDs(); // Ledjes veranderen en updaten
    }
  }
}

void beginSituatie(){
  // Zolang de Z-as is uitgeschoven blijf dit doen
  while(uitgeschoven == true){
    leesDistanceSensorZ(); // Lees de huidige positie van de z-as

    // Controleer of de z-as nogsteeds is uitgeschoven
    if(uitgeschoven == true){
      // Beweeg z-as naar achter
      analogWrite(pwmA, 127); // Snelheid motor
      digitalWrite(dirA, HIGH); // Richting motor
    }
  }
  //while(klaar == false){
  //  leesBegin();
  //}
}

// leesBegin(){
//   String input = cString;
//   int checkBegin = input.substring(7).toInt();
//   delay(20);
//   if(checkBegin == 0){
//     klaar = false;
//   }
//   if(checkBegin == 1){
//     klaar = true;
//   }
// }

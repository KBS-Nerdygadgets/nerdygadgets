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

// Variable voor uitgeschovend detectie
bool uitgeschoven = false;

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
  switch(huidigeModus){
    case HANDMATIG:
      handmatigBewegen();
      break;
    case STOP:
      noodstopReset();
      break;
    case AUTOMATISCH:
      if(bijCoordinaatAangekomen){
        pakProduct();
      }
      else{
        digitalWrite(pwmA, 0);
      }
      break;
  }
  serialRead();
  functiesStatussen();
  functiesSensoren();
  Serial.println(input);
  // Serial.println(tweeNaarEen);
}

//*Functies voor de communicatie tussen Arduinos
void serialRead() {
  input = "";
  Wire.requestFrom(slaveAddress1, 3);
  while (Wire.available()) {
    char c = Wire.read();
    input += c;
  }
  leesString();
}

void serialWrite(){
  encoderInString();
  Wire.write(tweeNaarEen.c_str());
}

void leesString() {
  yasOmhoog = input.substring(0, 1).toInt();
  bijCoordinaatAangekomen = input.substring(1, 2).toInt();
  resettenYencoder = input.substring(2, 3).toInt();
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
  //Binnen
  if(richting == Binnen && !uitgeschoven){
    analogWrite(pwmA, 0);
  }
  //Buiten
  else if(richting == Buiten && voorSafe == false){
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
      break;
    case HANDMATIG:
      digitalWrite(yellowLED, HIGH);
      digitalWrite(greenLED, LOW);
      break;
    case AUTOMATISCH:
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);
      break;
  }
}

unsigned long previousMillis1 = 0; // Variabele om de tijd bij te houden van de laatste keer dat de sensor is uitgelezen
const unsigned long interval1 = 50; // Interval van 100 milliseconden
void noodstopReset() {
  unsigned long currentMillis = millis();                 // Haal de huidige tijd op
  bool microSchap = analogRead(msSchap);                  //Value van de sensor in var zetten
  bool noodStopReset = digitalRead(buttonNoodStopReset);  // status van de noodstop resetknop in een var zetten
     
  // Controleer of er 100 milliseconden zijn verstreken sinds de laatste meting
  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;                      // Reset de timer

    // Controleer of de knop is ingedrukt, het schap op zijn plek staat en of de debounce tijd voorbij is
    if(noodStopReset == LOW && microSchap == HIGH && huidigeModus == STOP && (currentMillis - lastButtonPressTime >= debounceDelay)){
      lastButtonPressTime = currentMillis;               // Update de laatste knopdruk tijd
      beginSituatie();
      // het onderstaande willen we pas doen na dat de robot op zijn beginplek staat
      huidigeModus = HANDMATIG;                           //Modus veranderen
      updateLEDs();                                       // Ledjes updaten
    }
  }
}

void noodStopInitiatie(){
  bool noodStopStart = digitalRead(buttonNoodStop);
  if(noodStopStart == LOW){
    huidigeModus = STOP; //Modus veranderen
    updateLEDs(); // Ledjes veranderen
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
  static bool vorigeKnopStatus = HIGH;  
  unsigned long currentMillis = millis(); // Haal de huidige tijd op
  bool setStatusBool = digitalRead(buttonNoodStopReset);  

  if(setStatusBool == LOW && vorigeKnopStatus == HIGH){
    // Controleer of er 100 milliseconden zijn verstreken sinds de laatste meting
    if (currentMillis - previousMillis3 >= interval3) {
      previousMillis3 = currentMillis; // Reset de timer

    
      if(setStatusBool == LOW && huidigeModus != STOP){
        switch (huidigeModus) {
          case HANDMATIG:
            huidigeModus = AUTOMATISCH;
            updateLEDs();
            break;
          case AUTOMATISCH:
            tweeNaarEen.setCharAt(0, 48); //Reset Automatische modus
            productOpgepakt = false; //Reset Automatische modus
            huidigeModus = HANDMATIG;
            updateLEDs();
            break;
        }
      }
    }
  }
  vorigeKnopStatus = setStatusBool;
}

//* Handmatig Functies
void leesJoystick() {
  yValue = analogRead(VRY_PIN);
}

void handmatigBewegen() {
  if(huidigeModus != STOP){
    leesJoystick();
    // Achteruit
    if (yValue > 700) {
      moveZ(Binnen, snelheidHandmatig);
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
    zAsUit();

}

void zAsUit(){
  //Zas vooruit
  moveZ(Buiten, snelheid);
  if(!voorSafe){
    digitalWrite(pwmA, 0); //Stop wanneer uitgeschoven
    tweeNaarEen.setCharAt(0, 49); //Stuur Z is uitgeschoven naar Arduino 1
  }
  // delay(tijd);
}

//*Sensoren
void functiesSensoren(){
  leesDistanceSensorZ();
  leesMicroSchap();
}

unsigned long previousMillis = 0; // Variabele om de tijd bij te houden van de laatste keer dat de sensor is uitgelezen
const unsigned long interval = 100; // Interval van 100 milliseconden
void leesDistanceSensorZ(){
  unsigned long currentMillis = millis(); // Huidige tijd ophalen

  // CONTROLLEER OF HET 100MS IS GEWEEST SINDS DE LAATSTE METING
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Reset de timer
    float volts = analogRead(distanceSensorZ);// Value van de sensor in var zetten
  
    // Serial.print("Afstand vork: "); Serial.println(volts); //distance is tussen 650 en 300
      
    //De Z as staat helemaal naar binnen
    if(volts >= 640){
      tweeNaarEen.setCharAt(0, 48); //seriële communicatie Z-as 0
      uitgeschoven = false;
    }
    else if (volts < 330){
      voorSafe = false;
    }
    else if ( volts > 330 && volts < 640){
      voorSafe = true;
    }

    //De Z as staat helemaal naar buiten
    if (volts < 640){
      tweeNaarEen.setCharAt(0, 49); //seriële communicatie Z-as 1
      uitgeschoven = true;
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
    bool microSchap = digitalRead(msSchap); //Value van de sensor in var zetten
    // Print de afstand naar de seriÃ«le monitor
    
    if(microSchap == LOW){
      analogWrite(pwmA, 0);
      huidigeModus = STOP;
      updateLEDs(); // Ledjes veranderen en updaten
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

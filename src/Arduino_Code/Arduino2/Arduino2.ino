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

//De snelheid in automatische en handmatige mode
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

// Microswitch pins voor magazijn
const int msSchap = 7;

// Noodstop knoppen
const int buttonNoodStop = 5;
const int buttonNoodStopReset = 6;
const int buttonSetStatus = A1;

// *** VARIABLEN *** ///
// status Modus
enum Modus {STOP, HANDMATIG, AUTOMATISCH};
Modus huidigeModus = HANDMATIG;

// Richting van Z-as
enum Richting {Binnen, Buiten};

//variabel voor afstandsensor
bool voorSafe = false;
bool uitgeschoven = false;

//begin
bool klaar = false;
int aantalProducten = 0;
int tijd = 0;
int delayOmhoog = 800;
bool omhoogGegaan = false;
bool productOpgepakt = false;

//Communicatie voor Arduino 1
String tweeNaarEen = "000000";
String EenNaarTwee = "";

//Seriele communicatie
const int slaveAddress1 = 8;
const int slaveAddress2 = 9;

bool yAsOmhoog = 0;
bool bijCoordinaatAangekomen = 0;
bool resettenYencoder = 0;

// Variabelen voor knoppen
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
  attachInterrupt(digitalPinToInterrupt(YencoderPin), YleesEncoder, RISING);

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
  //Robot doorloopt case afhankelijk van de status waarin hij zich bevind
  switch(huidigeModus){
    case HANDMATIG:
      handmatigBewegen();
      break;
    case STOP:
      noodstopReset();
      break;
    case AUTOMATISCH:
      if(!productOpgepakt){
        if(bijCoordinaatAangekomen){
          productOppakken();
        }
        else{
          digitalWrite(pwmA, 0);
        }
      }
      else{
        Serial.println("Product is succesvol opgepakt");
      }
      break;
  }
  serialRead();
  functiesStatussen();
  functiesSensoren();
}

//*Functies voor de communicatie tussen Arduinos
//Vang bericht van Arduino 1
void serialRead() {
  EenNaarTwee = "";
  Wire.requestFrom(slaveAddress1, 3);
  while (Wire.available()) {
    char c = Wire.read();
    EenNaarTwee += c;
  }
  //zet de string om in de nodige waardes
  leesString();
}

//Stuur bericht naar Arduino 1
void serialWrite(){
  //De encoder moet eerste omgezet worden in een string van 4 cijfers
  encoderInString();
  Wire.write(tweeNaarEen.c_str());
}

//Zet bericht om in variabelen
void leesString() {
  yAsOmhoog = EenNaarTwee.substring(0, 1).toInt();
  bijCoordinaatAangekomen = EenNaarTwee.substring(1, 2).toInt();
  resettenYencoder = EenNaarTwee.substring(2, 3).toInt();
  if(resettenYencoder == 1){ //reset Yencoder op basis van EenNaarTwee van Arduino 1
    Yencoder = 0;
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

//*Functies voor statussen
//Collectie van statussen functies
void functiesStatussen(){
  noodStopInitiatie();
  noodstopReset();
  setStatus();
  stuurStatus();
}

//Stuur de correcte status naar Arduino 1
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

//Bewegen Z as, inclusief beveiliging
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

//Set het stoplicht obv de status
void updateLEDs() {
  // Turn off all LEDs
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);

  // LAMPJES SWITCHEN
  switch (huidigeModus) {
    case STOP:
      digitalWrite(redLED, HIGH);
      break;
    case HANDMATIG:
      digitalWrite(yellowLED, HIGH);
      break;
    case AUTOMATISCH:
      digitalWrite(greenLED, HIGH);
      break;
  }
}

//Zet status naar handmatig vanuit noodstop
//Variabelen voor tijd tussen checks
unsigned long previousMillis1 = 0;
const unsigned long interval1 = 50;
void noodstopReset() {
  unsigned long currentMillis = millis();
  bool microSchap = analogRead(msSchap);                  //Value van de sensor in var zetten
  bool noodStopReset = digitalRead(buttonNoodStopReset);  // status van de noodstop resetknop in een var zetten
  
  // Controleer of er *interval* milliseconden zijn verstreken sinds de laatste meting
  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;

    // Controleer of de knop is ingedrukt, het schap op zijn plek staat en of de debounce tijd voorbij is
    if(noodStopReset == LOW && microSchap == HIGH && huidigeModus == STOP && (currentMillis - lastButtonPressTime >= debounceDelay)){
      lastButtonPressTime = currentMillis;
      // beginSituatie(); //!werkt niet compleet
      // het onderstaande willen we pas doen na dat de robot op zijn beginplek staat
      huidigeModus = HANDMATIG;                           //Modus veranderen
      updateLEDs();                                       // Ledjes updaten
    }
  }
}

//Initiatie van noodstop bij druk op de knop
void noodStopInitiatie(){
  bool noodStopStart = digitalRead(buttonNoodStop);
  if(noodStopStart == LOW){
    huidigeModus = STOP; //Modus veranderen
    updateLEDs(); // Ledjes veranderen
  }
}

//Gebruikt de bovenste gele knop om tussen handmatige en automatische mode te wisselen
//Variabelen voor de tijd tussen checks
unsigned long previousMillis3 = 0;
const unsigned long interval3 = 300;
void setStatus(){
  unsigned long currentMillis = millis(); // Haal de huidige tijd op
  // Controleer of er 100 milliseconden zijn verstreken sinds de laatste meting
  if (currentMillis - previousMillis3 >= interval3) {
    previousMillis3 = currentMillis; // Reset de timer
    bool setStatusBool = digitalRead(buttonNoodStopReset);
    
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

//*Automatisch Functies
//Pak product op, werkt obv input Arduino 1
void productOppakken() {
  //Stap 1
  zAsUit();
  //Stap 2, Y-as omhoog
  if(yAsOmhoog){
    //Stap 3
    zAsIn();
    if(!uitgeschoven){
      productOpgepakt = true;
    }
  }
}

//Stap 1 voor het product oppakken proces
void zAsUit(){
  //Zas vooruit
  moveZ(Buiten, snelheid);
  if(!voorSafe){
    digitalWrite(pwmA, 0); //Stop wanneer uitgeschoven
    tweeNaarEen.setCharAt(0, 49); //Stuur 1, Z is uitgeschoven naar Arduino 1
  }
}

//Stap 3 voor het product oppakken proces
void zAsIn(){
  //Zas binnen
  moveZ(Binnen, snelheid);
  if(!uitgeschoven){
    digitalWrite(pwmA, 0); //Stop wanneer uitgeschoven
    tweeNaarEen.setCharAt(0, 48); //Stuur 0, Z is binnen naar Arduino 1
  }
}

//lees de Y encoder
void YleesEncoder() {
  //Afhangend van de richting dat de motor opgaat, encoder meer of minder maken
  if (digitalRead(YrichtingPin) == 0) {
    Yencoder++;
  } else {
    Yencoder--;
  }
}

//* Handmatig Functies
//Beweeg in de richting dat de bestuurder aangeeft
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

//Lees input van de joystick
void leesJoystick() {
  yValue = analogRead(VRY_PIN);
}

//*Sensoren
//Collectie van sensoren functies
void functiesSensoren(){
  leesDistanceSensorZ();
  leesMicroSchap();
}

//Lees de afstand dat de sensor op de Z-as detecteerd
//Variabelen voor tijd tussen checks
unsigned long previousMillis = 0;
const unsigned long interval = 100;
void leesDistanceSensorZ(){
  unsigned long currentMillis = millis();

  // Controleer of er *interval* milliseconden zijn verstreken sinds de laatste keer dat de afstandssensor is gelezen
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float volts = analogRead(distanceSensorZ);
        
    //De Z as staat helemaal naar binnen
    if(volts >= 640){
      uitgeschoven = false;
    }
    else if (volts < 330){
      voorSafe = false;
    }
    else if ( volts > 330 && volts < 640){
      voorSafe = true;
    }

    //De Z as staat helemaal of deels naar buiten
    if (volts < 640){
      uitgeschoven = true;
    } 
  }
} 

//Lees de microswitch op het schap
//Variabelen voor tijd tussen checks
unsigned long previousMillis2 = 0;
const unsigned long interval2 = 500;
void leesMicroSchap(){
  unsigned long currentMillis = millis();

    // Controleer of er *interval* milliseconden zijn verstreken sinds de laatste keer dat de microswitch is gelezen
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    bool microSchap = digitalRead(msSchap);
    
    if(microSchap == LOW){
      analogWrite(pwmA, 0);
      huidigeModus = STOP;
      updateLEDs(); // Ledjes veranderen en updaten
    }
  }
}

//!Werken niet compleet
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

//!werken niet compleet
leesBegin(){
  String EenNaarTwee = cString;
  int checkBegin = EenNaarTwee.substring(7).toInt();
  delay(20);
  if(checkBegin == 0){
    klaar = false;
  }
  if(checkBegin == 1){
    klaar = true;
  }
}
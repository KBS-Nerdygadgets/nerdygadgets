#include <SoftwareSerial.h>
#define VRY_PIN A2

////////////////////////////
// *** ORANJE ARDUINO *** //
////////////////////////////

//Motorpins voor motor z-as
const int pwmA = 3;
const int dirA = 12;

// Afstandsensorpin voor z-as
const int distanceSensorZ = A3;

// Afstandsensorenpins voor magazijn
const int distanceSensorL = A4;
const int distanceSensorR = A5;

// y-as van de joystick
int yValue = 0;
int snelheid = 255;

//variabel voor afstandsensor
bool voorSafe = false;
bool achterSafe = false;

// Variable voor uitgeschovend detectie
bool uitgeschoven = false;

// NOODSTOP
bool noodstop = false;

int aantalProducten = 0;
int tijd = 0;
int delayOmhoog = 800;

bool bijCoordinaat = true;
bool omhoogGegaan = false;

//seriele communicatie
SoftwareSerial link(7, 10);  // Rx, Tx
byte greenLED = 12;
char cString[20];
byte chPos = 0;
unsigned long sendmessageMillis = 0;

String firstThreeChars;
//seriele communicatie end

void setup() {
  // put your setup code here, to run once:
  delay(1500);
  TCCR2B = TCCR2B & B11111000 | B00000110;  // for PWM frequency of 122.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

  //pinMode motor
  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  Serial.println(yValue);

    //pinMode afstandsensoren
  pinMode(distanceSensorZ, INPUT);
  pinMode(distanceSensorL, INPUT);
  pinMode(distanceSensorR, INPUT);
  
  Serial.begin(9600);

  //seriele communicatie
  link.begin(9600);
  pinMode(greenLED, OUTPUT);
  //seriele communicatie end
}

void loop() {
  // put your main code here, to run repeatedly:
  // leesJoystick();
  leesDistanceSensorZ();
  leesDistanceSchap();
  handmatigBewegen();
  //if(bijCoordinaat == true) {
  //  pakProduct();
  //}
  // Serial.println(yValue);

  //seriele communicatie
  // Specify the message to send
  const char* messageToSend = "3234890"; //dit is de message die je wilt sturen. "3234890" kan met alles vervangen worden, ook variabelen
  // Transmit the message
  if ((millis() - sendmessageMillis) > 200) {
    sendMessage(messageToSend);
    sendmessageMillis = millis();
  }
  while (link.available()) {
    char ch = link.read();
    if (chPos < sizeof(cString) - 1) {  // Avoid buffer overflow
      cString[chPos++] = ch;
    }
  }
  if (chPos > 0) {          // Check if there is any received data
    cString[chPos] = '\0';  // Terminate cString
    Serial.print(cString);
    chPos = 0;  // Reset position for the next message
  }
  String input = cString;
  firstThreeChars = input.substring(0, 3);  //string input van eerste 3 getallen (0, 3)

  if (firstThreeChars.toInt() == 001) { //iets als de eerste 3 karakters over serial 001 zijn. je kunt dit aanpassen zolang het een nummer is
    //Serial.print("success");  //print voor debugging
  }
  //seriele communicatie end
}


//seriele communicatie
void sendMessage(const char* message) {
  digitalWrite(greenLED, HIGH);
  link.println(message);
  //Serial.println(message); // Print to local screen for debugging
  digitalWrite(greenLED, LOW);
}
//seriele communicatie end

void leesJoystick() {
  yValue = analogRead(VRY_PIN);
}

void handmatigBewegen() {
  leesJoystick();
  //Achteruit
  if (yValue > 700) {
    analogWrite(pwmA, 127);
    digitalWrite(dirA, HIGH);
  }
  //Vooruit
  else if (yValue < 300) {
    analogWrite(pwmA, 127);
    digitalWrite(dirA, LOW);
  }

  else {
    analogWrite(pwmA, LOW);
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

  while (omhoogGegaan == false) {
    analogWrite(pwmA, 0);
  }


  analogWrite(pwmA, snelheid);
  digitalWrite(dirA, HIGH);
  delay(tijd);

  analogWrite(pwmA, 0);

  bijCoordinaat = false;
  aantalProducten++;
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
    Serial.print("Afstand vork: "); Serial.println(volts); //distance is tussen 650 en 300
    
    if(volts > 646){
      achterSafe = true;
      uitgeschoven = false;
      //Serial.println("achter");
    }
    if (volts < 310){
      voorSafe = true;
      //Serial.println("voor");
    }
    if ( volts > 311 && volts < 645){ // NIet compleet accuraat, bijwerken
      //Serial.println("tussen 300 en 640");
      voorSafe = false;
      achterSafe = false;
   }
    if (volts < 645){
      uitgeschoven = true;
   } 

  //  if (uitgeschoven == true){
  //     Serial.println("   uitgeschoven");
  //  }else{
  //     Serial.println("niet uitgeschoven");
  //  }
  }
} 

// SCHAP SENSOREN
void leesDistanceSchap(){
  unsigned long currentMillis = millis(); // Haal de huidige tijd op

  // Controleer of er 100 milliseconden zijn verstreken sinds de laatste meting
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Reset de timer

    float afstandLinks = analogRead(distanceSensorL); //Value van de sensor in var zetten
    float afstandRechts = analogRead(distanceSensorR); //Value van de sensor in var zetten
  
    // Print de afstand naar de seriële monitor
    // Serial.print("Afstand links: "); Serial.print(afstandLinks); Serial.print(" & rechts: "); Serial.println(afstandRechts);

    if(afstandLinks < 250 || afstandRechts < 250){
      Serial.println("NOODSTOP");
      noodstop = true;
    }
  }
}

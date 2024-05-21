#include <SoftwareSerial.h>

#define VRX_PIN  A2 // Arduino pin connected to VRX pin
#define VRY_PIN  A3 // Arduino pin connected to VRY pin

////////////////////////////
// *** GROENE ARDUINO *** //
////////////////////////////

//seriele communicatie
SoftwareSerial link(7, 10); // Rx, Tx
byte greenLED = 12;
char cString[20];
byte chPos = 0;
unsigned long sendmessageMillis = 0;

//Snelheid van de Motoren
const int snelheid = 255;
const int snelheidHandmatig = snelheid/2;

//Xas
const int pwmA = 3;
const int dirA = 12;
const int XencoderPin = 2;
const int XrichtingPin = 4;
int Xencoder = 0;
int xValue = 0; // To store value of the X axis

//Yas
const int pwmB = 11;
const int dirB = 13;
const int brakeB = 8;
int yValue = 0; // To store value of the Y axis

//Pins van de microswitches, ms = microswitch
const int msBeneden = A5;
const int msBoven = A4;

//Pins van de inductive (metaal)sensoren, ind = inductive
const int indLinks = 6;
const int indRechts = 5;

//Knoppen, hebben nu dezelfde pins als metaalsensor, moeten worden veranderd (nu niet aangesloten)
const int resetKnopEncoder = 5;
const int knopSwitchStatus = 6;

int status = 1;
String richting = "";

//753 pulsen per coordinaat
const int coordinaten[5][2] = {
  {155, 0},   //5:1
  {911, 0},   //5:2
  {1664, 0},  //5:3
  {2417, 0},  //5:4
  {3170, 0}   //5:5
};

//Uiteinde sensoren variablen
bool drukSwitchBoven = false;
bool drukSwitchBeneden = false;
bool metaalLinks = false;
bool metaalRechts = false;

//*Setup
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

  //pinMode microswitches
  pinMode(msBeneden, INPUT_PULLUP);
  pinMode(msBoven, INPUT_PULLUP);

  //pinMode inductive (metaal)sensoren
  pinMode(indLinks, INPUT);
  pinMode(indRechts, INPUT);

  //Interrupt voor encoder
  attachInterrupt(digitalPinToInterrupt(XencoderPin), leesEncoder, RISING);

  Serial.begin(9600);

  //seriele communicatie setup
  link.begin(9600);
  pinMode(greenLED, OUTPUT);
}

//*Loop
void loop() {
  leesMicroSwitches();
  leesInductiveSensoren();
  activeerStatus();
  comm1naar2();
  //Druk de onderste knop in om de encoder te resetten. Dit moet op het nulpunt gebeuren
  // Serial.print("Pulsen"); Serial.println(Xencoder);
}

//*Functies voor communicatie tussen Arduinos
// Specify the message to send
void comm1naar2(){
  const char* messageToSend = "1to2";
  // Transmit the message
  if ((millis() - sendmessageMillis) > 200) {
  sendMessage(messageToSend);
  sendmessageMillis = millis();
  }
  while (link.available()) {
    char ch = link.read();
    if (chPos < sizeof(cString) - 1) { // Avoid buffer overflow
      cString[chPos++] = ch;
    }
  }
  if (chPos > 0) { // Check if there is any received data
    cString[chPos] = '\0'; // Terminate cString
    // Serial.print(cString);
    chPos = 0; // Reset position for the next message
  }
}
// Function to transmit a message over the serial connection
void sendMessage(const char* message) {
  digitalWrite(greenLED, HIGH);
  // link.println(message);
  //Serial.println(message); // Print to local screen for debugging
  digitalWrite(greenLED, LOW);
}

//*Functies voor de knoppen
//Lees of de 2 knoppen zijn ingedrukt
void activeerStatus(){
  status = 1;

  //Encoder wordt gereset;
  if(digitalRead(resetKnopEncoder) == 0){
    Xencoder = 0;
  }

  //Status wordt op automatisch gezet
  if(digitalRead(knopSwitchStatus) == 0){
      status = 2;
  }

  //Als knop niet is ingedrukt 
  if(status == 1){
    handmatigeStatus();
  }

  //Hou de bovenste knop ingedrukt om de automatische mode te activeren
  else if(status == 2){
    gaNaarCoordinaat(3);
  }
}


//*Functies voor statussen
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
  // Joystick naar rechts
  if (xValue > 700 && metaalRechts == false) {
    richting += "1";
  }

  // Joystick naar links
  if (xValue < 300 && metaalLinks == false) {
    richting += "2";
  }

  // Joystick naar boven
  if (yValue > 700 && drukSwitchBeneden == false) {
    richting += "3";
  }

  // Joystick naar beneden
  if (yValue < 300 && drukSwitchBoven == false) {
    richting += "4";
  }

  // Stop
  if (richting == "") {
    richting += "0";
  }

  //Serial.println(richting);
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

//*Functies voor sensoren
unsigned long previousMillis = 0; // Variabele om de tijd bij te houden van de laatste keer dat de microswitches zijn gelezen
const unsigned long interval = 300; // Interval van 100 milliseconden

void leesMicroSwitches(){
  // Haal de huidige tijd op
  unsigned long currentMillis = millis();
  
  // Controleer of er 100 milliseconden zijn verstreken sinds de laatste keer dat de microswitches zijn gelezen
  if (currentMillis - previousMillis >= interval) {
    // Reset de timer
    previousMillis = currentMillis;
    
    // Lees de status van de schakelaars
    bool switch1State = digitalRead(msBeneden);
    bool switch2State = digitalRead(msBoven);
    
    // Controleer of een van de schakelaars geactiveerd is
    // Print naar de seriële monitor welke schakelaar is ingedrukt
    if (switch1State == HIGH) {
      //Serial.println("Switch beneden is ingedrukt!");
      drukSwitchBeneden = true; 
    }
    if (switch2State == HIGH) {
      //Serial.println("Switch boven is ingedrukt!");
      drukSwitchBoven = true; 
    }
    if (switch1State == LOW && switch2State == LOW) {
     //Serial.println("testmicro");
     drukSwitchBoven = false;
     drukSwitchBeneden = false;
    }
  }
}

unsigned long previousMillis2 = 0; // Variabele om de tijd bij te houden van de laatste keer dat de microswitches zijn gelezen
const unsigned long interval2 = 300; // Interval van 100 milliseconden

void leesInductiveSensoren(){
  unsigned long currentMillis = millis(); // Haal de huidige tijd op

  // Controleer of er 100 milliseconden zijn verstreken sinds de laatste keer dat de microswitches zijn gelezen
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis; // Reset de timer
     
    // Lees de status van de schakelaars
    bool indLinksState = digitalRead(indLinks);
    bool indRechtsState = digitalRead(indRechts);
  
    // Controleer of een van de schakelaars geactiveerd is
    // Print naar de seriële monitor welke schakelaar is ingedrukt
    if (indLinksState == LOW) {
    //Serial.println("Nabijheid gedetecteerd aan de linkerkant");
    metaalLinks = true;
    }
    if (indRechtsState == LOW) {
    //Serial.println("Nabijheid gedetecteerd aan de rechterkant");
    metaalRechts = true;
    } 
    if (indLinksState == HIGH && indRechtsState == HIGH) {
    //Serial.println("testmetaal");
    metaalLinks = false;
    metaalRechts = false;
    }
  }
}

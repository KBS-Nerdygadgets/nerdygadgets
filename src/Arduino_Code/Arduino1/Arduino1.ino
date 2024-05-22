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
int xValue = 0; // Joystick Xas
bool XasAangekomen = false; //Automatische mode

//Yas
const int pwmB = 11;
const int dirB = 13;
const int brakeB = 8;
int Yencoder = 0;
int yValue = 0; // Joystick Yas
bool YasAangekomen = false; //Automatische mode

//Pins van de microswitches, ms = microswitch
const int msBeneden = 10;
const int msBoven = 7;

//Pins van de inductive (metaal)sensoren, ind = inductive
const int indLinks = 6;
const int indRechts = 5;

//Status enum voor switch case
enum Modus {STOP, HANDMATIG, AUTOMATISCH};
Modus huidigeModus = HANDMATIG;

int status = 1;
String richting = "";
String input = "";

String eenNaarTwee = "000";

//Encoder waardes van de coordinaten
const int coordinaten[25][2] = {
  {155, 125},   //5:1
  {911, 125},   //5:2
  {1664, 125},  //5:3
  {2417, 125},  //5:4
  {3170, 125},  //5:5

  {155, 542},   //4:1
  {911, 542},   //4:2
  {1664, 542},  //4:3
  {2417, 542},  //4:4
  {3170, 542},  //4:5

  {155, 960},   //3:1
  {911, 960},   //3:2
  {1664, 960},  //3:3
  {2417, 960},  //3:4
  {3170, 960},  //3:5

  {155, 1377},   //2:1
  {911, 1377},   //2:2
  {1664, 1377},  //2:3
  {2417, 1377},  //2:4
  {3170, 1377},  //2:5

  {155, 1796},   //1:1
  {911, 1796},   //1:2
  {1664, 1796},  //1:3
  {2417, 1796},  //1:4
  {3170, 1796}   //1:5
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

  //pinMode microswitches
  pinMode(msBeneden, INPUT_PULLUP);
  pinMode(msBoven, INPUT_PULLUP);

  //pinMode inductive (metaal)sensoren
  pinMode(indLinks, INPUT);
  pinMode(indRechts, INPUT);

  //Interrupt voor encoder
  attachInterrupt(digitalPinToInterrupt(XencoderPin), XleesEncoder, RISING);

  Serial.begin(9600);

  //seriele communicatie setup
  link.begin(9600);
}

//*Loop
void loop() {
  functiesSensoren();
  switch(huidigeModus){
    case HANDMATIG:
      // Serial.println("Handmatig");
      handmatigeStatus();
      break;
    case STOP:
      // Serial.println("Noodstop");
      analogWrite(pwmA, 0);
      analogWrite(pwmB, 0);
      //functies noodstop
      break;
    case AUTOMATISCH:
      // Serial.println("Automatisch");
      gaNaarCoordinaat(13);
      break;
  }
  serialRead();
  leesString();
  setStatus();
  serialWrite(eenNaarTwee);
  // Serial.println(Yencoder);
  // Serial.println(Xencoder);
}

//*Functies voor communicatie tussen Arduinos
//Vang bericht van Arduino 2
void serialRead() {
  while (link.available()) {
    char ch = link.read();

    if (chPos < sizeof(cString) - 1) { // Avoid buffer overflow
      cString[chPos++] = ch;
    }
  }
  if (chPos > 0) { // Check if there is any received data
    cString[chPos] = '\0'; // Terminate cString
    chPos = 0; // Reset position for the next message
  }
}

//Zet bericht om in variabelen
void leesString() {
  input = cString;
  Serial.println(input);
  Yencoder = input.substring(1, 5).toInt();
  status = input.substring(5).toInt();
  setStatus();
  delay(20);
}

// Set het bericht dat je wil versturen
void serialWrite(String message){
  const char* messageToSend = message.c_str();
  // Transmit the message
  if ((millis() - sendmessageMillis) > 150) {
  sendMessage(messageToSend);
  sendmessageMillis = millis();
  }
}

//Stuur bericht naar Arduino 2
void sendMessage(const char* message) {
  link.println(message);
  //Serial.println(message); // Print to local screen for debugging
}

//*Functies voor statussen
//Set de status op basis van input van Arduino 2
void setStatus() {
  if (status == 0) {
    huidigeModus = STOP;
  }

  if (status == 1) {
    huidigeModus= HANDMATIG;
  }

  if (status == 2) {
    huidigeModus = AUTOMATISCH;
  }
}
//beweeg naar de coordinaat
void gaNaarCoordinaat(int coordinaatIndex){
  //*Xas
  //beweeg naar links als coordinaat zich links bevind
  if(Xencoder > coordinaten[coordinaatIndex][0]){
    XasAangekomen = false;
    analogWrite(pwmA, snelheid);
    digitalWrite(dirA, LOW);
  }
  //beweeg naar rechts als coordinaat zich rechts bevind
  else if(Xencoder < coordinaten[coordinaatIndex][0]){
    XasAangekomen = false;
    analogWrite(pwmA, snelheid);
    digitalWrite(dirA, HIGH);
  }
  //stop
  // else{
  //   analogWrite(pwmA, 0);
  //   XasAangekomen = true;
  // }

  //*Yas
  //beweeg naar beneden als coordinaat zich boven bevind
  if(Yencoder > coordinaten[coordinaatIndex][1]){
    YasAangekomen = false;
    analogWrite(pwmB, snelheid);
    digitalWrite(dirA, LOW);
  }
  //beweeg naar boven als coordinaat zich boven bevind
  else if(Yencoder < coordinaten[coordinaatIndex][1]){
    YasAangekomen = false;
    analogWrite(pwmB, snelheid);
    digitalWrite(dirA, HIGH);
  }
  //stop
  else{
    analogWrite(pwmB, 0);
    YasAangekomen = true;
  }

  //Aangekomen op coordinaat? geef door naar Arduino 2
  if(YasAangekomen && XasAangekomen){
    eenNaarTwee.setCharAt(1, 49);
  }
}
//lees de Xencoder
void XleesEncoder() {
  if (digitalRead(XrichtingPin) == 1) {
    Xencoder++;
  } else {
    Xencoder--;
  }
}

//*Handmatige functies
//collectie van handmatige functies
void handmatigeStatus(){
  leesJoystick();
  geefRichting();
  handmatigBewegen();
}

//Lees input van de joystick
void leesJoystick() {
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
}

//kies een richting op basis van joystick input
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

//Beweeg motoren in richting
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
//Collectie van sensoren functies
void functiesSensoren(){
  leesMicroSwitches();
  leesInductiveSensoren();
}

//Lees de status van de microswitches op de Yas
unsigned long previousMillis = 0; // Variabele om de tijd bij te houden van de laatste keer dat de microswitches zijn gelezen
const unsigned long interval = 300; // Interval van 300 milliseconden
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
      // Serial.println("Switch beneden is ingedrukt!");
      drukSwitchBeneden = true;
      eenNaarTwee.setCharAt(2, 49); //Set getal 1
    }
    if (switch2State == HIGH) {
      // Serial.println("Switch boven is ingedrukt!");
      drukSwitchBoven = true; 
    }
    if (switch1State == LOW && switch2State == LOW) {
     //Serial.println("testmicro");
      drukSwitchBoven = false;
      drukSwitchBeneden = false;
      eenNaarTwee.setCharAt(2, 48); //Set getal 0
    }
  }
}

//Lees de status van de inductieve sensoren op de Xas
unsigned long previousMillis2 = 0; // Variabele om de tijd bij te houden van de laatste keer dat de microswitches zijn gelezen
const unsigned long interval2 = 300; // Interval van 300 milliseconden
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
      // Serial.println("Nabijheid gedetecteerd aan de linkerkant");
      metaalLinks = true;
      Xencoder = 0; //Xas bevind zich bij nulpunt, reset encoder
    }
    if (indRechtsState == LOW) {
      // Serial.println("Nabijheid gedetecteerd aan de rechterkant");
      metaalRechts = true;
    } 
    if (indLinksState == HIGH && indRechtsState == HIGH) {
    //Serial.println("testmetaal");
      metaalLinks = false;
      metaalRechts = false;
    }
  }
}
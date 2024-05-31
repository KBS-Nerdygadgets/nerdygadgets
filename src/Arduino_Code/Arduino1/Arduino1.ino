#include <Wire.h>

#define VRX_PIN  A2 // Arduino pin connected to VRX pin
#define VRY_PIN  A3 // Arduino pin connected to VRY pin

////////////////////////////
// *** GROENE ARDUINO *** //
////////////////////////////

//seriele communicatie
const int slaveAddress1 = 8;
const int slaveAddress2 = 9;

//Snelheid van de Motoren
const int snelheid = 255;
const int snelheidHandmatig = snelheid/2;

//Xas
const int pwmA = 3;
const int dirA = 12;
const int XencoderPin = 2;
const int XrichtingPin = 4;
volatile int Xencoder = 0;
int xValue = 0; // Joystick Xas
bool XasAangekomen = false; //Automatische mode

//Yas
const int pwmB = 11;
const int dirB = 13;
const int brakeB = 8;
int yValue = 0; // Joystick Yas
bool YasAangekomen = false; //Automatische mode

//Pins van de microswitches
const int msBeneden = 10;
const int msBoven = 7;

//Pins van de inductive (metaal)sensoren
const int indLinks = 6;
const int indRechts = 5;

//Status enum voor switch case
enum Modus {STOP, HANDMATIG, AUTOMATISCH};
Modus huidigeModus = HANDMATIG;

enum Richting {Rechts, Links, Boven, Beneden};

bool ZasUitgeschoven = false;
int Yencoder = 0;
int status = 1;
String richting = "";
String input = "";

//Automatische modus variabelen
bool YomhoogOmOpTePakken = false;
bool productOpgepakt = false;

//string wordt gestuurd naar Arduino 2
String eenNaarTwee = "000";

//Encoder waardes van de coordinaten
const int coordinaten[25][2] = {
  {169, 69},   //5:1
  {925, 69},   //5:2
  {1721, 69},  //5:3
  {2436, 69},  //5:4
  {3141, 69},  //5:5

  {169, 601},   //4:1
  {925, 601},   //4:2
  {1721, 601},  //4:3
  {2436, 601},  //4:4
  {3141, 601},  //4:5

  {169, 1119},   //3:1
  {925, 1119},   //3:2
  {1721, 1080},  //3:3
  {2436, 1119},  //3:4
  {3141, 1119},  //3:5

  {169, 1637},   //2:1
  {925, 1637},   //2:2
  {1721, 1637},  //2:3
  {2436, 1637},  //2:4
  {3141, 1637},  //2:5

  {169, 2154},   //1:1
  {925, 2154},   //1:2
  {1721, 2154},  //1:3
  {2436, 2154},  //1:4
  {3141, 2154}   //1:5
};

//Uiteinde sensoren variablen
bool drukSwitchBoven = false;
bool drukSwitchBeneden = false;
bool metaalLinks = false;
bool metaalRechts = false;

//Noodsstop
bool noodStop = false;

//Z-as
bool uitgeschoven = false;

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
  pinMode(brakeB, OUTPUT);

  //pinMode microswitches
  pinMode(msBeneden, INPUT_PULLUP);
  pinMode(msBoven, INPUT_PULLUP);

  //pinMode inductive (metaal)sensoren
  pinMode(indLinks, INPUT);
  pinMode(indRechts, INPUT);

  //Interrupt voor encoder
  attachInterrupt(digitalPinToInterrupt(XencoderPin), XleesEncoder, RISING);

  //Setup communicatie met Arduino 2
  Wire.begin(slaveAddress1);
  Wire.onReceive(serialRead);
  Wire.onRequest(serialWrite);
  Serial.begin(9600);
}

//*Loop
void loop() {
  //Robot doorloopt case afhankelijk van de status waarin hij zich bevind
  switch(huidigeModus){
    case HANDMATIG:
      handmatigeStatus();
      break;
    case STOP:
      analogWrite(pwmA, 0);
      analogWrite(pwmB, 0);
      break;
    case AUTOMATISCH:
      if(!productOpgepakt){
        automatischeFuncties(12);
      }
      else{
        Serial.println("Product is succesvol opgepakt");
      }
      break;
  }
  serialRead();
  functiesSensoren();
}

//*Functies voor communicatie tussen Arduinos
//Vang bericht van Arduino 2
void serialRead() {
  input = "";
  Wire.requestFrom(slaveAddress2, 6);
  while (Wire.available()) {
    char c = Wire.read();
    input += c;
  }
  //zet de string om in de nodige waardes
  leesString();
}

//Struur bericht naar Arduino 2
void serialWrite(){
  Wire.write(eenNaarTwee.c_str());
}

//Zet bericht om in variabelen
void leesString() {
  ZasUitgeschoven = input.substring(0,1).toInt();
  Yencoder = input.substring(1, 5).toInt();
  status = input.substring(5, 6).toInt();
  //Nadat status is gelezen moet de status aangepast worden als het nodig is
  setStatus();
}

//*Functies voor statussen
//Set de status
void setStatus() {
  if (status == 0) {
    huidigeModus = STOP;
  }

  if (status == 1) {
    huidigeModus= HANDMATIG;

    //Variabelen en communicatie van automatische mode moet gereset worden
    eenNaarTwee.setCharAt(0, 48);
    eenNaarTwee.setCharAt(1, 48);
    XasAangekomen = false;
    YasAangekomen = false;
    YomhoogOmOpTePakken = false;
    productOpgepakt = false;
  }

  if (status == 2) {
    huidigeModus = AUTOMATISCH;
  }
}

//Bewegen X as, inclusief beveiliging
void moveX(Richting richting, int snelheid){
  if(richting == Rechts && metaalRechts == 1){
    analogWrite(pwmA, 0);
  }
  else if(richting == Links && metaalLinks == 1){
    analogWrite(pwmA, 0);
  }
  else{
    analogWrite(pwmA, snelheid);
    //rechts
    if(richting == Rechts){
      digitalWrite(dirA, HIGH);
    }
    //links
    if(richting == Links){
      digitalWrite(dirA, LOW);
    }
  }
}

//Bewegen Y as, inclusief beveiliging
void moveY(Richting richting, int snelheid){
  analogWrite(brakeB, 0);
    //als drukswitches zijn geactiveerd, geen beweging
  if(richting == Boven && drukSwitchBoven == 1){
    analogWrite(brakeB, 1);  //!Werkte niet
    analogWrite(pwmB, 0);
  }
  //als drukswitches zijn geactiveerd, geen beweging
  else if(richting == Beneden && drukSwitchBeneden == 1){
    analogWrite(brakeB, 1); //!Werkte niet
    analogWrite(pwmB, 0);
  }
  else{
    analogWrite(pwmB, snelheid);
    //beneden
    if(richting == Beneden){
      digitalWrite(dirB, HIGH);
    }
    //boven
    else if(richting == Boven){
      digitalWrite(dirB, LOW);
    }
  }
}

//*Automatische functies
//collectie van automatische functies
void automatischeFuncties(int coordinaat){
  gaNaarCoordinaat(coordinaat);
  //Aangekomen op coordinaat? geef door naar Arduino 2
  if(YasAangekomen && XasAangekomen){
    productOppakken();
    if(!ZasUitgeschoven && YomhoogOmOpTePakken){
      productOpgepakt = true;
    }
  }
}

//beweeg naar de coordinaat
void gaNaarCoordinaat(int coordinaatIndex){
  //*Xas
  //beweeg naar links als coordinaat zich links bevind
  if(!XasAangekomen){
    if(Xencoder > (coordinaten[coordinaatIndex][0])){
      moveX(Links, snelheid);
    }
    //beweeg naar rechts als coordinaat zich rechts bevind
    else if(Xencoder < (coordinaten[coordinaatIndex][0])){
      moveX(Rechts, snelheid);
    }
    //stop
    else{
      analogWrite(pwmA, 0);
      XasAangekomen = true;
    }
  }

  //*Yas
  //beweeg naar beneden als coordinaat zich beneden bevind
  if(!YasAangekomen){
    analogWrite(brakeB, 0);
    if(Yencoder > coordinaten[coordinaatIndex][1]){
      moveY(Beneden, snelheid);
    }
    //beweeg naar boven als coordinaat zich boven bevind
    else if(Yencoder < coordinaten[coordinaatIndex][1]){
      moveY(Boven, snelheid);
    }
    //stop
    else{
      analogWrite(pwmB, 0);
      analogWrite(brakeB, 1); //!Werkte niet
      YasAangekomen = true;
    }
  }
}

//Pak product op, werkt obv input Arduino 2
void productOppakken(){
  eenNaarTwee.setCharAt(1, 49); //Set aangekomen op 1
  if(ZasUitgeschoven){
    //Y-as 100 pulsen omhoog laten bewegen wanneer Zas is uitgeschoven
    int tempYencoder = Yencoder + 100;
    while(Yencoder < tempYencoder && !YomhoogOmOpTePakken){
      //Voor veiligheid moeten sensoren blijven lezen en blijven lezen van Arduino 2
      functiesSensoren();
      serialRead();
      moveY(Boven, snelheid);
    }
    YomhoogOmOpTePakken = true;
    analogWrite(pwmB, 0);
    eenNaarTwee.setCharAt(0, 49); //Set y-as omhooggegaan op true
  }
}

//lees de Xencoder
void XleesEncoder() {
  //Afhangend van de richting dat de motor opgaat, encoder meer of minder maken
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
  if (xValue > 700) {
    richting += "1";
  }

  // Joystick naar links
  if (xValue < 300) {
    richting += "2";
  }

  // Joystick naar boven
  if (yValue > 700) {
    richting += "3";
  }

  // Joystick naar beneden
  if (yValue < 300) {
    richting += "4";
  }

  // Stop
  if (richting == "") {
    richting += "0";
  }
}

//Beweeg motoren in richting
void handmatigBewegen() {
  //Zet de string om in een Int
  int var = richting.toInt();
  switch (var) {
    //X naar Rechts
    case 1:
      moveX(Rechts, snelheidHandmatig);
      analogWrite(pwmB, 0); //Stop Y
      break;

    //X naar Links
    case 2:
      moveX(Links, snelheidHandmatig);
      analogWrite(pwmB, 0); //Stop Y
      break;

    //Y naar Beneden
    case 3:
      moveY(Beneden, snelheidHandmatig);
      analogWrite(pwmA, 0); //Stop X
      break;

    //Y naar Boven
    case 4:
      moveY(Boven, snelheidHandmatig);
      analogWrite(pwmA, 0); //Stop X
      break;
      
    //rechts omhoog
    case 14:
      moveX(Rechts, snelheidHandmatig);
      moveY(Boven, snelheidHandmatig);
      break;

    //links omhoog
    case 24:
      moveX(Links, snelheidHandmatig);
      moveY(Boven, snelheidHandmatig);
      break;

    //links omlaag
    case 23:
      moveX(Links, snelheidHandmatig);
      moveY(Beneden, snelheidHandmatig);
      break;

    //rechts omlaag
    case 13:
      moveX(Rechts, snelheidHandmatig);
      moveY(Beneden, snelheidHandmatig);
      break;

    //Stop
    default:
      analogWrite(pwmA, 0); //Stop X
      analogWrite(pwmB, 0); //Stop Y
      analogWrite(brakeB, 1); //Rem Y as
  }
}

//*Functies voor sensoren
//Collectie van sensoren functies
void functiesSensoren(){
  leesMicroSwitches();
  leesInductiveSensoren();
}

//Lees de status van de microswitches op de Yas
//Variabelen voor tijd tussen checks
unsigned long previousMillis = 0;
const unsigned long interval = 300;
void leesMicroSwitches(){
  unsigned long currentMillis = millis();
  
  // Controleer of er *interval* milliseconden zijn verstreken sinds de laatste keer dat de microswitches zijn gelezen
  if (currentMillis - previousMillis >= interval) {
    // Reset de timer
    previousMillis = currentMillis;
    
    // Lees de status van de schakelaars
    bool switch1State = digitalRead(msBeneden);
    bool switch2State = digitalRead(msBoven);
    
    // Controleer of een van de schakelaars geactiveerd is
    if (switch1State == HIGH) {
      drukSwitchBeneden = true;
      eenNaarTwee.setCharAt(2, 49); //Set getal 1, geef door dat Y-encoder moet resetten
    }
    if (switch2State == HIGH) {
      drukSwitchBoven = true; 
    }
    if (switch1State == LOW && switch2State == LOW) {
      drukSwitchBoven = false;
      drukSwitchBeneden = false;
      eenNaarTwee.setCharAt(2, 48); //Set getal 0, geef door dat Y-encoder niet meer moet resetten
    }
  }
}

//Lees de status van de inductieve sensoren op de Xas
//Variabelen voor tijd tussen checks
unsigned long previousMillis2 = 0;
const unsigned long interval2 = 300;
void leesInductiveSensoren(){
  unsigned long currentMillis = millis();

  // Controleer of er *interval* milliseconden zijn verstreken sinds de laatste keer dat de microswitches zijn gelezen
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    
    // Lees de status van de schakelaars
    bool indLinksState = digitalRead(indLinks);
    bool indRechtsState = digitalRead(indRechts);
  
    // Controleer of een van de schakelaars geactiveerd is
    if (indLinksState == LOW) {
      metaalLinks = true;
      Xencoder = 0; //Xas bevind zich bij nulpunt, reset encoder
    }
    if (indRechtsState == LOW) {
      metaalRechts = true;
    } 
    if (indLinksState == HIGH && indRechtsState == HIGH) {
      metaalLinks = false;
      metaalRechts = false;
    }
  }
}
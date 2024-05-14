#include <SoftwareSerial.h>

#define VRX_PIN  A2 // Arduino pin connected to VRX pin
#define VRY_PIN  A3 // Arduino pin connected to VRY pin

//seriele communicatie
SoftwareSerial link(7, 10); // Rx, Tx
byte greenLED = 12;
char cString[20];
byte chPos = 0;
sendmessageMillis = 0
//seriele communicatie end

//Motorpin voor motor x-as
int pwmA = 3;
int dirA = 12;

//Motorpin voor motor y-as
int pwmB = 11;
int dirB = 13;

//De assen van de joystick
int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis

//Pins van de microswitches, ms = microswitch
int msBeneden = A4;
int msBoven =  A5;

//Pins van de inductive (metaal)sensoren, ind = inductive
int indLinks = 5;
int indRechts = 6;

//Uiteinde sensoren variablen
boolean drukSwitchBoven = false;
boolean drukSwitchBeneden = false;
boolean metaalLinks = false;
boolean metaalRechts = false;

String richting = "";

void setup() {
    TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz
  //pinMode motoren
  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(dirB, OUTPUT);

  //pinMode microswitches
  pinMode(msBeneden, INPUT);
  pinMode(msBoven, INPUT);

  //pinMode inductive (metaal)sensoren
  pinMode(indLinks, INPUT);
  pinMode(indRechts, INPUT);

  Serial.begin(9600) ;


  //seriele communicatie
  link.begin(9600);
  pinMode(greenLED, OUTPUT);
  //seriele communicatie end
}

void loop() {
  // read analog X and Y analog values
  leesJoystick();
  geefRichting();
  handmatigBewegen();
  leesMicroSwitches();
  leesInductiveSensoren();

  //seriele communicatie
  // Specify the message to send
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
    Serial.print(cString);
    chPos = 0; // Reset position for the next message
  }
}
// Function to transmit a message over the serial connection
void sendMessage(const char* message) {
  digitalWrite(greenLED, HIGH);
  link.println(message);
  //Serial.println(message); // Print to local screen for debugging
  digitalWrite(greenLED, LOW);
}
//seriele communicatie end

void leesJoystick() {
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
}

void geefRichting() {
  richting = "";
  if (xValue > 700 && metaalLinks == false) {
    richting += "1";
  }

  if (xValue < 300 && metaalLinks == false) {
    richting += "2";
  }

  if (yValue > 700 && drukSwitchBoven == false ) {
    richting += "3";
  }

  if (yValue < 300 && drukSwitchBeneden == false) {
    richting += "4";
  }

  if (richting == "") {
    richting += "0";
  }

  Serial.println(richting);
}

void handmatigBewegen() {
  int var = richting.toInt();
  switch (var) {
    case 2:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, LOW);
      analogWrite(pwmB, 0);
      break;

    case 1:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, HIGH);
      analogWrite(pwmB, 0);
      break;
    
    case 4:
      analogWrite(pwmB, 255);
      digitalWrite(dirB, LOW);
      analogWrite(pwmA, 0);
      break;
    
    case 3:
      analogWrite(pwmB, 127);
      digitalWrite(dirB, HIGH);
      analogWrite(pwmA, 0);
      break;
    
    case 14:
      analogWrite(pwmA, 127);
      analogWrite(pwmB, 255);
      digitalWrite(dirA, HIGH);
      digitalWrite(dirB, LOW);
      break;

    case 24:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, LOW);
      analogWrite(pwmB, 255);
      digitalWrite(dirB, LOW);
      break;
  
    case 23:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, LOW);  
      analogWrite(pwmB, 127);
      digitalWrite(dirB, HIGH);
      break;

    case 13:
      analogWrite(pwmA, 127);
      digitalWrite(dirA, HIGH);
      analogWrite(pwmB, 127);
      digitalWrite(dirB, HIGH);
      break;

    default:
      analogWrite(pwmA, 0);
      analogWrite(pwmB, 0);
  }
}

void leesMicroSwitches(){
  // Lees de status van de schakelaars
  bool switch1State = digitalRead(msBeneden);
  bool switch2State = digitalRead(msBoven);
  
  // Controleer of een van de schakelaars geactiveerd is
  if (switch1State == HIGH || switch2State == HIGH) {
    // Als een van de schakelaars geactiveerd is, stop de motor
    digitalWrite(pwmB, LOW);

    // Print naar de seriële monitor welke schakelaar is ingedrukt
    if (switch1State == HIGH) {
      Serial.println("Switch beneden is ingedrukt!");
      drukSwitchBeneden = true; 
    }
    if (switch2State == HIGH) {
      Serial.println("Switch boven is ingedrukt!");
      drukSwitchBoven = true; 
    }
  } else {
    drukSwitchBoven = false;
    drukSwitchBeneden = false;
  }
}

void leesInductiveSensoren(){
  // Lees de status van de schakelaars
  bool indLinksState = digitalRead(indLinks);
  bool indRechtsState = digitalRead(indRechts);
  
  // Controleer of een van de schakelaars geactiveerd is
  if (indLinksState == LOW || indRechtsState == LOW) {
    // Als een van de schakelaars geactiveerd is, stop de motor
    digitalWrite(pwmB, LOW);

    // Print naar de seriële monitor welke schakelaar is ingedrukt
    if (indLinksState == LOW) {
    Serial.println("Nabijheid gedetecteerd aan de linkerkant");
    metaalLinks = true;
    }
    if (indRechtsState == LOW) {
    Serial.println("Nabijheid gedetecteerd aan de rechterkant");
    metaalRechts = true;
    } 
  } else {
    metaalLinks = false;
    metaalRechts = false;
  }
}

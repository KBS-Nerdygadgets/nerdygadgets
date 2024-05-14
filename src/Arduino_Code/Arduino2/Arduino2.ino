#include <SoftwareSerial.h>
#define VRY_PIN A2

//Z as
int pwmA = 3;
int dirA = 12;

//Encoder
const int YencoderPin = 2;
const int YrichtingPin = 4;
int Yencoder = 0;

int yValue = 0;
int snelheid = 255;

int aantalProducten = 0;
int tijd = 0;
int delayOmhoog = 800;

String tweeNaarEen = "00000";

bool bijCoordinaat = false;

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

  //Pins voor motoren
  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(YencoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(YencoderPin), leesEncoder, RISING);

  Serial.begin(9600);
  Serial.println(yValue);

  //seriele communicatie
  link.begin(9600);
  pinMode(greenLED, OUTPUT);
  //seriele communicatie end
}

//*Setup
void loop() {
  // put your main code here, to run repeatedly:
  // leesJoystick();
  handmatigBewegen();
  encoderInString();
  Serial.println(Yencoder);
  serialWrite(tweeNaarEen);
  serialRead();
  // pakProduct();
}

//*Functies voor communicatie tussen Arduinos
void sendMessage(const char* message) {
  digitalWrite(greenLED, HIGH);
  link.println(message);
  //Serial.println(message); // Print to local screen for debugging
  digitalWrite(greenLED, LOW);
}

void serialWrite(String message) {
  // Specify the message to send
  const char* messageToSend = message.c_str(); //dit is de message die je wilt sturen. "3234890" kan met alles vervangen worden, ook variabelen
  // Transmit the message
  if ((millis() - sendmessageMillis) > 50) {
    sendMessage(messageToSend);
    sendmessageMillis = millis();
  }
}

void serialRead() {
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
}

//*Functies voor Statussen
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
  analogWrite(pwmA, 0);

  int encoder = Yencoder + 100;

  while(Yencoder < encoder){
    tweeNaarEen.setCharAt(0, 49);
    serialWrite(tweeNaarEen);
  }
  tweeNaarEen.setCharAt(0, 48);

  analogWrite(pwmA, snelheid);
  digitalWrite(dirA, HIGH);
  delay(tijd);
  analogWrite(pwmA, 0);

  bijCoordinaat = false;
  aantalProducten++;
  if(aantalProducten == 3){
    aantalProducten = 0;
  }
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
    tweeNaarEen = "00000";
  }
}


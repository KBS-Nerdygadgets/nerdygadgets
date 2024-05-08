#define VRY_PIN A2

int pwmA = 3;
int dirA = 12;

int yValue = 0;

//seriele communicatie
SoftwareSerial link(7, 10); // Rx, Tx
byte greenLED = 12;
char cString[20];
byte chPos = 0;
sendmessageMillis = 0
//seriele communicatie end

void setup() {
  // put your setup code here, to run once:
  TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz
  // TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz

  pinMode(pwmA, OUTPUT);
  pinMode(dirA, OUTPUT);
  Serial.begin(9600);

  //seriele communicatie
  link.begin(9600);
  pinMode(greenLED, OUTPUT);
  //seriele communicatie end
}

void loop() {
  // put your main code here, to run repeatedly:
  leesJoystick();
  handmatigBewegen();
  Serial.println(yValue);

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
}

void leesJoystick() {
  yValue = analogRead(VRY_PIN);
}

void handmatigBewegen() {
  if (yValue > 700) {
    analogWrite(pwmA, 127);
    digitalWrite(dirA, LOW);
  }
  
  else if (yValue < 300) {
    analogWrite(pwmA, 127);
    digitalWrite(dirA, HIGH);
  }
  
  else {
    analogWrite(pwmA, LOW);
  }
}

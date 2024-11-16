// Arduino pins:
#define S0 6
#define S1 7
#define S2 12
#define S3 11
#define sensorOut 13
#define redLED 2
#define greenLED 3
#define blueLED 4

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

int redMin;
int redMax;
int greenMin;
int greenMax;
int blueMin;
int blueMax;

int color = 0;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(sensorOut, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  Serial.begin(9600);
  
  calibrate(); 
}
void loop() {
  readColor();
  decideColor(); 
  printColor(); 
}
void decideColor() {
  redColor = constrain(redColor, 0, 255);
  greenColor = constrain(greenColor, 0, 255);
  blueColor = constrain(blueColor, 0, 255);

  int maxVal = max(redColor, max(greenColor, blueColor));

  if (redColor == maxVal) {
    color = 6; // Red
    analogWrite(redLED, 255);
    analogWrite(greenLED, 0);
    analogWrite(blueLED, 0);
  } else if (greenColor == maxVal) {
    color = 7; // Green
    analogWrite(redLED, 0);
    analogWrite(greenLED, 255);
    analogWrite(blueLED, 0);
  } else if (blueColor == maxVal) {
    color = 8; // Blue
    analogWrite(redLED, 0);
    analogWrite(greenLED, 0);
    analogWrite(blueLED, 255);
  } else {
    color = 0; // Unknown
    analogWrite(redLED, 0);
    analogWrite(greenLED, 0);
    analogWrite(blueLED, 0);
  }
}
void calibrate() {
  Serial.println("Calibrating...");
  Serial.println("Aim at WHITE object.");
  digitalWrite(13, HIGH);
  delay(2000);

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redMin = pulseIn(sensorOut, LOW);
  delay(100);

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  greenMin = pulseIn(sensorOut, LOW);
  delay(100);

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blueMin = pulseIn(sensorOut, LOW);
  delay(100);

  Serial.println("Aim at BLACK object.");
  digitalWrite(13, LOW);
  delay(2000);
  digitalWrite(13, HIGH);

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redMax = pulseIn(sensorOut, LOW);
  delay(100);

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  greenMax = pulseIn(sensorOut, LOW);
  delay(100);

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blueMax = pulseIn(sensorOut, LOW);
  delay(100);

  Serial.println("Calibration done.");
  digitalWrite(13, LOW);
}
void printColor() {
  Serial.print("R = ");
  Serial.print(redColor);
  Serial.print(" G = ");
  Serial.print(greenColor);
  Serial.print(" B = ");
  Serial.print(blueColor);
  Serial.print(" Detected Color: ");
  
  switch (color) {
    case 6: Serial.println("RED"); break;
    case 7: Serial.println("GREEN"); break;
    case 8: Serial.println("BLUE"); break;
    default: Serial.println("UNKNOWN"); break;
  }
}
void readColor() {
 // Red:
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  redFrequency = pulseIn(sensorOut, LOW);
  redColor = map(redFrequency, redMin, redMax, 255, 0);
  delay(100);
// Green:
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  greenFrequency = pulseIn(sensorOut, LOW);
  greenColor = map(greenFrequency, greenMin, greenMax, 255, 0);
  delay(100);
// Blue:
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blueFrequency = pulseIn(sensorOut, LOW);
  blueColor = map(blueFrequency, blueMin, blueMax, 255, 0);
  delay(100);
}

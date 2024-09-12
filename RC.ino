// Robot Car Project
// Final project for robotics workshop in Beta
// Team ER

//speeds forward
#define leftspeed 81
#define rightspeed 89
//speed adjustRight..right forwardSpeed
#define leftspeed2 89
//speed adjustLeft..left forwardSpeed
#define rightspeed2 70
//speeds turnAround
#define leftspeed3 57
#define rightspeed3 41
//speed turnRight
#define leftspeed4 50
//speed turnLeft
#define rightspeed4 41

// Pin definitions
int irSensorFrontLeft = 12;
int irSensorFrontRight = A0;
int irSensorLeft = 13;
int irSensorRight = A1;
int enableLeftMotor = 10;
int leftMotorForward = 8;
int leftMotorBackward = 9;
int enableRightMotor = 5;
int rightMotorForward = 6;
int rightMotorBackward = 7;
// irsensor values
int frontLeftSensorValue;
int frontRightSensorValue;
int leftSensorValue;
int rightSensorValue;

// Functions
// Movement Functions
void moveForward() {
  analogWrite(enableLeftMotor, leftspeed);
  analogWrite(enableRightMotor, rightspeed);
  delay(14);
  analogWrite(enableLeftMotor, 0);
  analogWrite(enableRightMotor, 0);
  delay(15);
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}

void adjustLeft() {
  analogWrite(enableLeftMotor, leftspeed);
  analogWrite(enableRightMotor, rightspeed2);
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}

void adjustRight() {
  analogWrite(enableLeftMotor, leftspeed2);
  analogWrite(enableRightMotor, rightspeed);
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}

void turnLeft() {
  analogWrite(enableLeftMotor, 0);
  analogWrite(enableRightMotor, rightspeed4 );
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}

void turnRight() {
  analogWrite(enableLeftMotor, leftspeed4);
  analogWrite(enableRightMotor, 0);
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

void stopMotors() {
  analogWrite(enableLeftMotor, 0);
  analogWrite(enableRightMotor, 0);
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

void turnAround() {
  analogWrite(enableLeftMotor, leftspeed3 );
  analogWrite(enableRightMotor, rightspeed3 );
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}

void setup() {

  pinMode(irSensorFrontLeft, INPUT);
  pinMode(irSensorFrontRight, INPUT);
  pinMode(irSensorLeft, INPUT);
  pinMode(irSensorRight, INPUT);
  pinMode(leftMotorForward, OUTPUT);
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);
  pinMode(enableLeftMotor, OUTPUT);
  pinMode(enableRightMotor, OUTPUT);
}

void loop() {

  // IR sensors with reversed logic
  // Black line .. HIGH , White line .. LOW
  frontLeftSensorValue = digitalRead(irSensorFrontLeft);
  frontRightSensorValue = digitalRead(irSensorFrontRight);
  leftSensorValue = digitalRead(irSensorLeft);
  rightSensorValue = digitalRead(irSensorRight);

  // Line following logic with maze-solving behavior
  if (leftSensorValue == LOW && frontLeftSensorValue == HIGH && frontRightSensorValue == HIGH) {
    moveForward();
  }
  else if (leftSensorValue == LOW && rightSensorValue == HIGH && frontLeftSensorValue == LOW && frontRightSensorValue == LOW) {
    turnRight();
  }
  else if (leftSensorValue == LOW && rightSensorValue == LOW && frontLeftSensorValue == LOW && frontRightSensorValue == LOW) {
    turnAround(); 
  }
  else if (frontLeftSensorValue == LOW && frontRightSensorValue == HIGH) {
    adjustRight();
  } else if (frontLeftSensorValue == HIGH && frontRightSensorValue == LOW) {
    adjustLeft();
  }
   else if (leftSensorValue == HIGH && rightSensorValue == HIGH && frontLeftSensorValue == HIGH && frontRightSensorValue == HIGH) {
    stopMotors(); 
  }
   else {
    turnLeft(); 
  }
}
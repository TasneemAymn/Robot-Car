// Robot Car Project
// Final project for robotics workshop in Beta
// Team ER

#define BLYNK_PRINT Serial  

#define BLYNK_TEMPLATE_ID              "TMPL2KCrruHLP"
#define BLYNK_TEMPLATE_NAME            "Robot Car Project"
#define BLYNK_AUTH_TOKEN               "umNDAFL8xWFozGB_op2WOShTwZbLYyDD"

// Include libraries
#include <BlynkSimpleStream.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// WiFi credentials
char ssid[] = "";  
char pass[] = "";  

// Initialize SoftwareSerial to communicate with ESP8266
#define RX_PIN  2
#define TX_PIN  3
SoftwareSerial espSerial(RX_PIN, TX_PIN); 

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
int trigPin = A3;
int echoPin = A4;
int servoPin = A5;
// irsensor values
int frontLeftSensorValue;
int frontRightSensorValue;
int leftSensorValue;
int rightSensorValue;
//Variables
bool endPoint = false; // To check if maze is ended
bool uncertaintyEnd = false; //A state of uncertainty b/w the end point and the multiplicity of branches
bool blynkControlActive = false; // To check if Blynk control is active

Servo ultrasonicSensor ;        // Servo motor to rotate ultrasonic sensor

// Functions

// for obstacle detected
bool obstacleDetected() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.035 / 2;

  if (distance < 15){
    return true ;
  }else{
    return false ; 
  }
}

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

void moveBackward() {
  analogWrite(enableLeftMotor, rightspeed);
  analogWrite(enableRightMotor, leftspeed);
  delay(14);
  analogWrite(enableLeftMotor, 0);
  analogWrite(enableRightMotor, 0);
  delay(15);
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, HIGH);
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

// Define Blynk virtual pins
#define Forward V1
#define Backward V2
#define Right V3
#define Left V4
#define Around V5

// Manual car control by Blynk
BLYNK_WRITE(Forward) {
  int value = param.asInt();
  if (value == 1) {
    moveForward();
    blynkControlActive = true;
  } else {
    blynkControlActive = false;
  }
}
BLYNK_WRITE(Backward) {
  int value = param.asInt();
  if (value == 1) {
    moveBackward();
    blynkControlActive = true;
  } else {
    blynkControlActive = false;
  }
}

BLYNK_WRITE(Right) {
  int value = param.asInt();
  if (value == 1) {
    turnRight();
    blynkControlActive = true;
  } else {
    blynkControlActive = false;
  }
}

BLYNK_WRITE(Left) {
  int value = param.asInt();
  if (value == 1) {
    turnLeft();
    blynkControlActive = true;
  } else {
    blynkControlActive = false;
  }
}

BLYNK_WRITE(Around) {
  int value = param.asInt();
  if (value == 1) {
    turnAround();
    blynkControlActive = true;
  } else {
    blynkControlActive = false;
  }
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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Define pin modes for TX and RX for SoftwareSerial
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  
  // Initialize servo motor
  ultrasonicSensor.attach(servoPin);
}

void loop() {
  Blynk.run();

   if (!blynkControlActive){
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
    }else if (leftSensorValue == HIGH && rightSensorValue == HIGH && frontLeftSensorValue == HIGH && frontRightSensorValue == HIGH && endPoint == false) {
      uncertaintyEnd = true;
      turnLeft();
      moveForward();
      delay(10);

      if(leftSensorValue == LOW && rightSensorValue == LOW && frontLeftSensorValue == LOW && frontRightSensorValue == LOW && uncertaintyEnd == true){
        moveBackward();
        while(leftSensorValue != HIGH && rightSensorValue != HIGH && frontLeftSensorValue != HIGH && frontRightSensorValue != HIGH){
          delay(10);
        }
        stopMotors();
        endPoint = true;
      }else{
        uncertaintyEnd = false;
        moveForward();
      }
    }else {
      turnLeft(); 
    }

    // Obstacle avoidance
    if (obstacleDetected()) {

      // Stop the robot when an obstacle is detected
      stopMotors();

      // Scan the environment to check the sides
      bool leftClear = false;
      bool rightClear = false;
      bool frontClear = false;

      // Continue checking until the obstacle is cleared
      while (obstacleDetected()) {
        // Check front side
        ultrasonicSensor.write(90); 
        delay(300); 
        frontClear = !obstacleDetected() && frontLeftSensorValue == HIGH && frontRightSensorValue == HIGH; 

        // Check left side
        ultrasonicSensor.write(0); 
        delay(300);
        leftClear = !obstacleDetected() && leftSensorValue == HIGH ; 

        // Check right side
        ultrasonicSensor.write(180); 
        delay(300);
        rightClear = !obstacleDetected() && rightSensorValue == HIGH ; 

        // Decide direction
        if (frontClear) {
        moveForward(); 
        break;
        } else if (leftClear && !rightClear) {
          turnLeft(); 
          break;
        } else if (!leftClear && rightClear) {
          turnRight(); 
          break;
        } else if (!leftClear && !rightClear) {
          stopMotors(); 
        }
      }

      ultrasonicSensor.write(90); // Return servo to forward-facing position
    }
  }
}
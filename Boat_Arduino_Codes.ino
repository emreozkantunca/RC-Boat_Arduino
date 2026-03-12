// C++ code
//12/2024

#include <Servo.h>  // Include the Servo library
Servo myServo;

// Analog pins
int pot = A5;
int pot2 = A4;
int reading,num, newPos, currMap;


// Define motor control pins for Motor 1
const int motor1Pin1 = 2;  
const int motor1Pin2 = 3;  
  

// Define the servo control pin
const int servoPin = 9;    

// Define timing variables
unsigned long previousMotorMillis = 0;
const long motorRunInterval = 5000;
const long motorStopInterval = 5000;
bool motorForward = true;
bool motorRunning = true;

void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);

  pinMode(pot, INPUT);
  pinMode(pot2, INPUT);

  Serial.begin(9600);

  myServo.attach(servoPin);
  currMap = analogRead(pot);
  reading = currMap;
  
  Serial.println("RC Boat Started");
}

/*
 *Get the current reading from potentiometer 
 * 10-Bit: 0-1023
*/ 

int readPot(){
  int newReading = analogRead(pot);
  if (abs(newReading - reading) > 3){   // deadband for noise
    reading = newReading;
  }
  return reading;
}
/*
 *Take a position and set the servo in the correct range
 */
void mapServo(int pos){
  num = map(pos, 675, 1023, 0, 180);
  myServo.write(num);
}

void loop() {
   newPos = readPot();
  mapServo(newPos);
  
  if(currMap != newPos){
    Serial.println("Pot Position = ");
    Serial.print(newPos);
    Serial.println(" Servo Angle = ");
    Serial.print(num);
    currMap = newPos;
  }
  
  unsigned long currentMillis = millis();
  // Control DC motors
  if (motorRunning) {
    if (currentMillis - previousMotorMillis >= motorRunInterval) {
      previousMotorMillis = currentMillis;

      // Stop the motors for 5 seconds
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
 

      motorRunning = false;  // Motors are now stopping
    }
  } else {
    if (currentMillis - previousMotorMillis >= motorStopInterval) {
      previousMotorMillis = currentMillis;

      // After the stop, toggle motor direction
      if (motorForward) {
        // Move Forward
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);
   
        motorForward = false;
      } else {
        // Move Reverse
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        motorForward = true;
      }

      motorRunning = true;  // Motors are now running again
    }
  }
}
 

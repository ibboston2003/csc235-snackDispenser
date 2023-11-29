int trigPin = 4;
int echoPin = 3;
int rangeValue;

#include <Stepper.h>

boolean isTurning = false;

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);


void setup() {
  // put your setup code here, to run once:
  myStepper.setSpeed(60);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(getDistance(echoPin, trigPin));
  //Serial.println(getSmoothReading(echoPin, trigPin, getDistance(echoPin, trigPin)));
  float smooth = getSmoothReading(echoPin, trigPin, getDistance(echoPin, trigPin));
  Serial.println(smooth);
  turnTime(smooth);

  // myStepper.step(1);
}

float getDistance(int aEchoPin, int aTrigPin) {

  // local variables- will return distance
  float duration;
  float dist;

  // make sure that it is off before starting
  digitalWrite(aTrigPin, LOW);
  delayMicroseconds(2);

  // send out a signal for 10 microseconds
  digitalWrite(aTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(aTrigPin, LOW);

  // now calculate the amount of time it took
  // for the signal to return

  // pulseIn gets the amount of time that the echoPin
  // reads as HIGH
  duration = pulseIn(aEchoPin, HIGH);

  // we only care about the distance one way,
  // not the round trip

  // and convert the time it took to travel one way
  // to cm (distance=rate/time)
  dist = duration / 2 * 0.0344;

  float distInches = dist * 0.3937;

  // return the result
  return distInches;
}

float getSmoothReading(int echoPins, int trigPins, int dist) {
  int currentReading;
  const int arrayLen = 10;
  static int readings[arrayLen];
  static int index = 0;

  static float sum = 0;

  currentReading = dist;
  //subtract the old reading from the sum
  sum -= readings[index];
  readings[index] = currentReading;
  sum += currentReading;

  index++;
  if (index >= arrayLen) {
    index = 0;
  }
  return sum / arrayLen;
}

void turnTime(float smoothReading) {

  if (smoothReading <= 2.0) {
    if (!isTurning) {
      Serial.println("turning");
      myStepper.step(100);
      delay(500);
      isTurning = true;
    }
  } else {
    isTurning = false;
  }
  /*
  if (smoothReading <= 2.0) {
      Serial.println("turning");
      myStepper.step(stepsPerRevolution);
      //delay(3000);
  }
  */
}

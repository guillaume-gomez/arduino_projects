
// sensor
const int sensorPin = A0; 
int sensorValue;
int sensorLow = 1023;
int sensorHigh = 0;

const int buttonPin = A1; 
const int potPin = A2; 

int buttonValue; 
int potValue; 

//led pin integrated
const int ledPin = 13;

void setup() {

  Serial.begin(115200); 

  // Make the LED pin an output and turn it on
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  // calibrate for the first five seconds after program runs
  while (millis() < 5000) {
    // record the maximum sensor value
    sensorValue = analogRead(A0);
    if (sensorValue > sensorHigh) {
      sensorHigh = sensorValue;
    }
    // record the minimum sensor value
    if (sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
  }
  // turn the LED off, signaling the end of the calibration period
  digitalWrite(ledPin, LOW);
}

void loop() {
  //read the input from A0 and store it in a variable
  buttonValue = analogRead(buttonPin) / 1000;
  potValue = analogRead(potPin); 
  sensorValue = analogRead(sensorPin);

  // map the sensor values to a wide range of pitches
  int min = 0; 
  int max = 1000; 
  int value = map(sensorValue, sensorLow, sensorHigh, min, max);
  value = constrain(value, min, max); 

  Serial.print(buttonValue); 
  Serial.print(","); 
  Serial.print(potValue); 
  Serial.print(","); 
  Serial.println(value); 
  
 //Serial.write('\n');
  // wait for a moment
  delay(10);
}

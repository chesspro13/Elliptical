/*
 * Dev: Brandon Mauldin 
 * 
 * Purpose: Slave device for elliptical
 * Responsabilities:
 *    > Receive commands from master to increase/decrease resistance
 *    > Act as hall effect sensor and report elliptical speed to master
 *    > Report the resistance level
 * 
 * Notes:
 *  > Enable pin needs to be pulled low to work 
 */

#include <Wire.h>

  // I2C communication with master device
String in;
String response;

  // Time Keeping
unsigned long now;

  // Speed detecion/calculation
int speedSensor = 5;
bool speedPulse;

long lastPulse;
float curSpeed;

  // Not used yet, but for speed detection/calculation
long distance;
long steps;
int spm;

  // Stepper motor controls
int motorStep = 12;
int motorDirection = 11;
int stepperEnable =  10;

  // Target intinsity level 0-9
int target = 1;

  // Number of steps it takes to get from zero to full
const int fullSteps = 60*2;

  // Where the stepper motor is
int curPos = 0;

bool debug;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting slave stuff...");
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);

    // Motor controls
  pinMode( motorDirection, OUTPUT);
  pinMode( motorStep, OUTPUT); 
  pinMode( stepperEnable, OUTPUT);
  digitalWrite( motorDirection, LOW);
  digitalWrite( motorStep, LOW);

  zeroStepper();

    // Hall effect sensor
  pinMode( speedSensor, INPUT_PULLUP );
}

  // Get the stepper motor to whatever position was set to be the target by the master device
void setTarget(int i)
{
    // Multiplication because I swithed to half steps
  i*=2;

    // Debug output  
  Serial.println("Setting the new target");
  Serial.println("Cur Pos: " + String(curPos));
  Serial.println("Target: " + String( i*6*2));

    // Verify new target is within range
  if( i < 0 && i > 9 )
    return;
    
    // Return if the current position is already where it needs to be
  if( i * 6 == curPos )
  return;
  
    // Actually moves the stepper motor depending on where it is 
  while( curPos < i * 6 )
    up();
  while( curPos > i * 6 )
    down();

}

  // Sends the stepper motor up (clockwise?) one half step
void up()
{
  Serial.println("Going up!");
  digitalWrite(motorDirection, HIGH);
  digitalWrite(motorStep, HIGH);
  delayMicroseconds(2000);
  digitalWrite(motorStep, LOW);
  curPos++;
}

  // Sends the stepper motor down (counter-clockwise?) one half step
void down()
{
  Serial.println("Going down!");
  digitalWrite(motorDirection, LOW);
  digitalWrite(motorStep, HIGH);
  delayMicroseconds(2000);
  digitalWrite(motorStep, LOW);
  curPos--;
}

  // Disables the stepper motor cotroller so the elliptical magnets pull the shaft to it's 
  // 'zero' position
void zeroStepper()
{
  digitalWrite( stepperEnable, HIGH);  
  delay( 500);
  digitalWrite( stepperEnable, LOW);
  curPos = fullSteps;
}

void loop() {

    // Timekeeping
  now = millis();

    // Parse what was sent from the master so the response can be prepared
    // Or just accomplish the task that the master sent
  if( in == "up" )
  {
    if( debug )
      Serial.println("GOING UP");  
    in = "";
    up();
  }else if( in == "down" )
  {
    if( debug )
      Serial.println("GOING DOWN");
    in = "";
    down();
  }else if( in.indexOf("arget") > 0)
  {
    target = in.substring(in.indexOf("[")+1,in.indexOf("]")).toInt();
    if( debug )
      Serial.println("My new target is: [" + String(target) + "]");
    in = "";
    setTarget( target );
  }else if( in == "pos" )
  {
    response = String( curPos );
    in = "";
  }else if( in == "speed")
  {
    response = String( curSpeed );
    in = "";
  }

    // Speed Sensor
    // Checks to see if the sensor has been pulled high AND the last state the sensor
    // was in was low to make sure it only gets triggered on the FIRST pulse
  if( digitalRead(speedSensor) == HIGH && !speedPulse )
  {
      // User has taken a step
    steps++;
    if( debug )
    {
      Serial.println("Distance: " + String( (float)(20/12) ));
      Serial.println("Time: " + String( (float)(now-lastPulse)/(float)1000 ) + " seconds");
      Serial.println("Speed: " + String((float)(20/12) / ((float)(now-lastPulse)/(float)1000)) + "mph");
    }

    curSpeed = (float)1.666 / ((float)(now-lastPulse)/(float)1000);
    if( debug )
      Serial.println(curSpeed);

    speedPulse = true;
    lastPulse = now;
  }else if( digitalRead(speedSensor) == LOW)
  {
    speedPulse = false;
  }
  
}

  // function that executes whenever data is requested by master
  // this function is registered as an event, see setup()
void requestEvent() {
    // Last letter is omitted for some reason??
  response += "X";
    // String gets converted into byte array to be sent over I2C
  byte bts[response.length()];
  response.getBytes(bts,response.length());
  
  Wire.write(bts,response.length()); // respond with message of 16 bytes
}

  // function that executes whenever data is received from master
  // this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  String d = "";
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    d += c;
  }
  in = d;
}

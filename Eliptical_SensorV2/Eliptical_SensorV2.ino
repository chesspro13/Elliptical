/*
 * Dev: Brandon Mauldin 
 * 
 * Purpose: Slave device for elliptical
 * Responsabilities:
 *    > Receive commands from master to increase/decrease resistance
 *    > Act as hall effect sensor and report elliptical speed to master
 *    > Report the resistance level
 * 
 */

#include <Wire.h>

String in;
String response;

unsigned long now;
bool forward;

int speedSensor = 5;
bool speedPulse;
int motorStep = 12;
int motorDirection = 11;
int stepperEnable =  10;

const int increment = 10;
int incrementRange = 0;
int increments[increment];

long distance;
long steps;
int spm;
long lastPulse;
float curSpeed;

int target = 1;

const int fullSteps = 60*2;

const int speedPin;

int curPos = 0;

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
  /*testPolarity();
  Serial.println("Getting data...");
  incrementRange = averagePos("range");
  Serial.println("Here: " + String((upperLimit - lowerLimit)/increment));
  for(int i = 0; i < increment;i++)
  {
    increments[i]= ((upperLimit - lowerLimit)/increment)*(i+1);
  }

  increments[0] = ((upperLimit - lowerLimit)/increment);

  Serial.println( "IR: " + String( incrementRange ));
  Serial.println("Ranges realitive: ");
  for( int i = increment-1; i >=0; i--)
  {
    Serial.println(String(i) + ": (" + String(increments[i]-incrementRange) + "," + String(increments[i]+incrementRange) + ")");
  }
  Serial.println("Ranges Accurite: ");
  for( int i = increment-1; i >=0; i--)
  {
    Serial.println(String(i) + ": (" + String((increments[i]-incrementRange)+lowerLimit) + "," + String((increments[i]+incrementRange)+lowerLimit) + ")");
  }*/
}

void setTarget(int i)
{
  i*=2;
  Serial.println("Setting the new target");
  Serial.println("Cur Pos: " + String(curPos));
  Serial.println("Target: " + String( i*6*2));
  if( i < 0 && i > 9 )
    return;
    
  if( i * 6 == curPos )
  return;
  
  {
    while( curPos < i * 6 )
      up();
    while( curPos > i * 6 )
      down();
  }
  
  /*
    int curPos = averagePos("average");
    int p = curPos - (lowerLimit + increments[i]);
    Serial.println( "Cur Pos: " + String(curPos) );
    Serial.println( "Offset: " + String( p));
    Serial.println( "Target offset: " + String( incrementRange ) );
    Serial.println( "Power level: " + String( i ) );
    if( p <= 20 && p >= -20)
    {
      return;
    }else
      motorRunTime = millis() + 1;
  
    Serial.println("Target: " + String(lowerLimit + increments[target]));
    if( curPos > lowerLimit + increments[target] )
    {
      Serial.println("Chasing target down");
      down();
    }
    if( curPos < lowerLimit + increments[i] )
    {
      Serial.println("Chasing target up");
      up();
    }
 */   
}


void up()
{
  Serial.println("Going up!");
  digitalWrite(motorDirection, HIGH);
  digitalWrite(motorStep, HIGH);
  delayMicroseconds(2000);
  digitalWrite(motorStep, LOW);
  //digitalWrite( stepperEnable, HIGH);
  curPos++;
}

void down()
{
  Serial.println("Going down!");
  digitalWrite(motorDirection, LOW);
  digitalWrite(motorStep, HIGH);
  delayMicroseconds(2000);
  digitalWrite(motorStep, LOW);
  curPos--;
}

void zeroStepper()
{
  digitalWrite( stepperEnable, HIGH);  
  delay( 500);
  digitalWrite( stepperEnable, LOW);
  curPos = fullSteps;
}


void testPolarity()
{
  /*

  int base = averagePos("average");
  digitalWrite( mot1, HIGH );
  digitalWrite( mot2, LOW );
  delay(125);
  digitalWrite( mot1, LOW );
  digitalWrite( mot2, LOW );
  int upper = averagePos("average");
  if( upper > base )
    forward = true;

  Serial.println("Base: " + String( base ) + "\tUpper: " + String(upper));
  */
}

void loop() {
  //delay(5);
  Serial.println("==============================================");
  now = millis();
  Serial.println("In: " + in);

  //TODO: Speed Sensor

  if( in == "Jennifer" )
  {
    response = "Beautiful";
    in = "";
  }else if( in == "up" )
  {
    Serial.println("GOING UP");
    
    in = "";
      up();
  }else if( in == "down" )
  {
    Serial.println("GOING DOWN");
    in = "";
      down();
  }else if( in.indexOf("arget") > 0)
  {
    target = in.substring(in.indexOf("[")+1,in.indexOf("]")).toInt();
    Serial.println("My new target is: [" + String(target) + "]");
    in = "";
    setTarget( target );
    //motorRunTime = now + 5;
  }else if( in == "pos" )
  {
    response = String( curPos );
    in = "";
  }else if( in == "speed")
  {
    response = String( curSpeed );
    in = "";
  }
  
  if(digitalRead(speedSensor) == LOW)
  {
    Serial.println("COUPLED SUCESSFULLY!!!");
  }

  if( digitalRead(speedSensor) == HIGH && !speedPulse )
  {
    long now = millis();
    steps++;
    Serial.println("Distance: " + String( 20/12 ));
    Serial.println("Time: " + String( (now-lastPulse)/1000 ) + " seconds");
    Serial.println("Speed: " + String((20/12) / ((now-lastPulse)/1000)) + "mph");
    Serial.println("----------------");
    Serial.println("Distance: " + String( (float)(20/12) ));
    Serial.println("Time: " + String( (float)(now-lastPulse)/(float)1000 ) + " seconds");
    Serial.println("Speed: " + String((float)(20/12) / ((float)(now-lastPulse)/(float)1000)) + "mph");

    curSpeed =  ((20)/((now - lastPulse)/1000));
    Serial.println(curSpeed);
    Serial.println("Time: " + String(now-lastPulse));
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

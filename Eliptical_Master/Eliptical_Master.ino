/*
 * Dev: Brandon Mauldin 
 * 
 * Purpose: Master device for elliptical
 * Responsabilities:
 *    > Get input from user and send data to the slave
 *    > Comunicate with raspberry pi for distance data
 * 
 */

#include <SoftwareSerial.h>
#include <Encoder.h>
#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  // Requesting data
bool requestingData;

  // Button Input stuff
byte buttonPin1 = 4;
byte buttonPin2 = 5;
bool btn1;
bool btn2;
bool lastBtn1;
bool lastBtn2;
bool ledON = true;

int dipDebug = 3;
bool debug;

String line1;
String line2;

long speedUpdate;

  // Bluetooth stuff
SoftwareSerial HM10(10, 8);
char appData;
String inData = "";
int target = 3;

float curSpeed;

void setup() {

  // I2C
  Wire.begin();        // join i2c bus (address optional for master)

  // Serial Monitor
  Serial.begin(9600);  // start serial for output

  // Bluetooth 
  HM10.begin(9600);

  // Onboard LED Diognostic
  pinMode(13, OUTPUT); // Onboard LED
  digitalWrite( 13, LOW);

  // OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  delay( 500 );
  display.display();
  
  // Buttons
  pinMode( buttonPin1, INPUT_PULLUP );
  pinMode( buttonPin2, INPUT_PULLUP );
  pinMode( dipDebug, INPUT );
}

void overOled(String str, int line)
{
  if( line ==1)
    line1 = str;
  else if( line ==2);
    line2 = str;
    
  display.clearDisplay();
  display.setTextSize(2.5);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(18,0);
  display.println(line1);
  
  display.setCursor(18,16);
  display.println(line2);
  
  display.display();
}

  // Send data over I2C
void overI2C(String toSend)
{
  // Add extra letter to be left out from the data
  toSend += "X";
  
  byte bts[toSend.length()];
  toSend.getBytes(bts,toSend.length());
  // Send data to the slave
  Wire.beginTransmission(8);
  Wire.write(bts, toSend.length());
  Wire.endTransmission();
}



void loop() {

  long now = millis();

  btn1 = digitalRead(buttonPin1);
  btn2 = digitalRead(buttonPin2);
  

      // BUTTON EVENTS
  // Up button
  if( btn1 == false && btn1 != lastBtn1 )
  { 
    upAction();
    HM10.write("Up");
  }
  // Down button
  if( btn2 == false && btn2 != lastBtn2 )
  {
    downAction();
    HM10.write("Down");
  }
  

      // GETTING DATA FROM THE SLAVE
  if( requestingData )
  {
    Wire.requestFrom(8, 16);    // request 6 bytes from slave device #8
  
    // Get data from the slave
    String resp = "";
    while (Wire.available()) { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      if( c != 255 )
        resp += char(c);
      //Serial.print("\"" + c + "\"");
    }
    Serial.println(resp);
  }
      // BLUETOOTH STUFF
  HM10.listen();

  delay(10);
  String s = "";
  while (HM10.available() > 0) {   // if HM10 sends something then read
    appData = HM10.read();
    inData = String(appData);  // save the data in string format
    s += inData;
    delay(5);
  }

  
    // BLE DATA PARSING
  // This is what came back from the ble
  Serial.println( "Inbound: " + s );
  if( s.length() > 0)
  {
    if( s == "Up" || s ==  "up" )
    {
      upAction();
    }
    if( s == "Down" || s == "down" )
    {
      downAction();
    }
  }

  if( now > speedUpdate )
  {
    speedUpdate += 1000;
    
    // Speed
    overI2C("speed");
    delay(10);
    String resp =  request();
    curSpeed = resp.toFloat();
    overOled("Speed: " + String(resp), 2);
    HM10.print("Speed:" + resp);
  }
  
  lastBtn1 = btn1;
  lastBtn2 = btn2;
}

void upAction()
{
  debug = false;//digitalRead(dipDebug);
  String resp;
  
  if( target + 1 < 10  )
    target++;

  if( debug )
  {
    overI2C("up");
    overI2C("pos");
    resp = request();
    Serial.println("Dubugging");
    Serial.println("Response from sensor: [" + resp + "]");
    overOled("D::Level: " + String(target+1),1);
  }
  else
  {
    overI2C("pos");
    resp = request();
    Serial.println("Response from sensor: [" + resp + "]");
    delay(100);
    overI2C("target[" + String(target) + "]");
    overOled("Level: " + String(target+1),1);
  }
  
  Serial.println("New Target: " + String(target));
  HM10.print("level:" + String(target));
}

void downAction()
{
  
  debug = false;//digitalRead(dipDebug);
  String resp;
  if( target - 1 >= 0 )
    target--;
  
  if( debug )
  {
    overI2C("down");
    overI2C("pos");
    resp = request();
    Serial.println("Dubugging");
    Serial.println("Response from sensor: [" + resp + "]");
    overOled("D::Level: " + String(target+1),1);
  }
  else
  {
    overI2C("pos");
    resp = request();
    Serial.println("Response from sensor: [" + resp + "]");
    delay(100);
    overI2C("target[" + String(target) + "]");
    overOled("Level: " + String(target+1),1);
  }
  Serial.println("New Target: " + String(target));
  HM10.print("level:" + String(target));
}

String request()
{
  Wire.requestFrom(8, 16);    // request 6 bytes from slave device #8
  
  // Get data from the slave
  String resp = "";
  while (Wire.available()) { // slave may send less than requested
    byte c = Wire.read(); // receive a byte as character
    if( c != 255 )
      resp += char(c);
    //Serial.print("\"" + c + "\"");
  }
  return resp;
}

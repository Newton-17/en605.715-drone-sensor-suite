#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


//**************************************************************************
// Type Defines and Constants
//**************************************************************************

#define  ERROR_LED_PIN  13 //Led Pin: Typical Arduino Board


#define ERROR_LED_LIGHTUP_STATE  LOW // the state that makes the led light up on your board, either low or high

// Select the serial port the project should use and communicate over
// Sombe boards use SerialUSB, some use Serial
#define SERIAL  SerialUSB

// Pins
#define SDA 20
#define SCL 21

#define SLAVE_ADDRESS 0x08



//**************************************************************************
// Screen Info Definitions
//**************************************************************************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define SDA 20
#define SCL 21

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//**************************************************************************
// BN0 INFO & Return Object
//**************************************************************************
char imuData[50];
char imuDataX[50];
char imuDataY[50];
char imuDataZ[50];
Adafruit_BNO055 bno = Adafruit_BNO055(55);


//**************************************************************************
// IMU Functions
//**************************************************************************
void readIMU(){
  sensors_event_t event;
  bno.getEvent(&event);
  
  float x = event.orientation.x;
  float y = event.orientation.y;
  float z = event.orientation.z;
  sprintf(imuDataX, "X%.2f", x);
  sprintf(imuDataY, "Y%.2f", y);
  sprintf(imuDataZ, "Z%.2f", z);
}

void setup()
{
  SERIAL.begin(9600);
  
  if (!bno.begin())
  {
      /* There was a problem detecting the BNO055 ... check your connections */
      SERIAL.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
      while (1);
  }

  Wire.begin(); // join i2c bus as master
}

void loop()
{
   readIMU();
   
   //writeToDisplayIMU();
   Wire.beginTransmission(9);
   Wire.write(imuDataX);
   Wire.endTransmission();
   delay(100);

   Wire.beginTransmission(9);
   Wire.write(imuDataY);
   Wire.endTransmission();
   delay(100);

   Wire.beginTransmission(9);
   Wire.write(imuDataZ);
   Wire.endTransmission();

   
   delay(100);
}

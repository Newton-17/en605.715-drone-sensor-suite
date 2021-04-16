#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SSD1306Wire.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

// Display Variable
SSD1306Wire display(0x3c, 4, 5);

// Write The Roll Pitch Yaw

void writeToDisplayIMU(float x, float y, float z)
{
  char imuBuffer[50];
  display.clear();
  sprintf(imuBuffer, "X: %.2f\nY: %.2f\nZ: %.2f", x, y, z);
  display.drawString(0, 0, imuBuffer);
  display.display();
}

void writeToDisplayError()
{
  display.init();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  char errorBuffer[] = "Ooops, no BNO055 detected \n... \nCheck your wiring or I2C ADDR!";
  display.drawString(0, 0, errorBuffer);
  display.display();
}

void writeToDisplayWelcome()
{
  display.init();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  char welcomeBuffer[] = "Welcome to the \n Drone IMU Sensor";
  display.drawString(0, 0, welcomeBuffer);
  display.display();
}

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test");
  Serial.println("");
  


  Wire.setClockStretchLimit(2000);

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    writeToDisplayError();
    while (1);
  }

  delay(1000);
  writeToDisplayWelcome(); 

  bno.setExtCrystalUse(true);
  Serial.print("X,Y,Z");
}

void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  /* Display the floating point data */
  Serial.print("");
  Serial.print(event.orientation.x, 4);
  Serial.print(",");
  Serial.print(event.orientation.y, 4);
  Serial.print(",");
  Serial.print(event.orientation.z, 4);
  Serial.println("");

  writeToDisplayIMU(event.orientation.x, event.orientation.y, event.orientation.z);

  delay(100);
}
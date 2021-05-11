//**************************************************************************
// FreeRtos on Samd21
// By Scott Briscoe
//
// Project is a simple example of how to get FreeRtos running on a SamD21 processor
// Project can be used as a template to build your projects off of as well
//**************************************************************************

#include <Seeed_Arduino_FreeRTOS.h>
#include <queue.h>
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
#define SERIAL          SerialUSB

// Pins
#define SDA 20
#define SCL 21

//**************************************************************************
// Screen Info Definitions
//**************************************************************************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define SDA 20
#define SCL 21

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

//**************************************************************************
// global variables
//**************************************************************************
TaskHandle_t Handle_aTask;
TaskHandle_t Handle_bTask;
QueueHandle_t xQueue;
QueueHandle_t zQueue;
QueueHandle_t yQueue;


//**************************************************************************
// Can use these function for RTOS delays
// Takes into account procesor speed
//**************************************************************************
void myDelayUs(int us) {
    vTaskDelay(us / portTICK_PERIOD_US);
}

void myDelayMs(int ms)
{
  vTaskDelay( (ms * 1000) / portTICK_PERIOD_US );  
}

void myDelayMsUntil(TickType_t* previousWakeTime, int ms) {
    vTaskDelayUntil(previousWakeTime, (ms * 1000) / portTICK_PERIOD_US);
}

//*****************************************************************
// Create a thread that prints out A to the screen every two seconds
// this task will delete its self after printing out afew messages
//*****************************************************************
static void readIMU(void* pvParameters) {
    Adafruit_BNO055 bno = Adafruit_BNO055(55);


    /* Initialise the sensor */
    if (!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    while (1) {
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
        float x = event.orientation.x;
        float y = event.orientation.y;
        float z = event.orientation.z;
        xQueueSend( xQueue, &z, portMAX_DELAY );
        xQueueSend( yQueue, &y, portMAX_DELAY );
        xQueueSend( zQueue, &z, portMAX_DELAY );
        taskYIELD();
    }

    // delete ourselves.
    // Have to call this or the system crashes when you reach the end bracket and then get scheduled.
    SERIAL.println("Thread A: Deleting");
    vTaskDelete(NULL);
}


//*****************************************************************
// Create a thread that prints out B to the screen every second
// this task will run forever
//*****************************************************************
static void displayOLED(void* pvParameters) {
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        SERIAL.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    // Queue Info
    float lxValue;
    float lyValue;
    float lzValue;

    display.display();
    while (1) {
      if (xQueueReceive( xQueue, &lxValue, portMAX_DELAY ) == pdPASS) {}
      if (xQueueReceive( yQueue, &lyValue, portMAX_DELAY ) == pdPASS) {}
      if (xQueueReceive( zQueue, &lzValue, portMAX_DELAY ) == pdPASS) {}
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print("X: ");
      display.println(lxValue);
      display.print("Y: ");
      display.println(lyValue);
      display.print("Z: ");
      display.println(lzValue);
      display.display();
      taskYIELD();
    }

    SERIAL.println("Thread B: Deleting");
    vTaskDelete(NULL);

}


//*****************************************************************

void setup() {
    SERIAL.begin(115200);

    vNopDelayMS(1000); // prevents usb driver crash on startup, do not omit this
    //while (!SERIAL) ;  // Wait for serial terminal to open port before starting program

    Wire.setClock(2000);
    xQueue = xQueueCreate( 5, sizeof( float ) );
    yQueue = xQueueCreate( 5, sizeof( float ) );
    zQueue = xQueueCreate( 5, sizeof( float ) );

    SERIAL.println("");
    SERIAL.println("******************************");
    SERIAL.println("        Program start         ");
    SERIAL.println("******************************");

    vSetErrorLed(ERROR_LED_PIN, ERROR_LED_LIGHTUP_STATE);

    // Create the threads that will be managed by the rtos
    // Sets the stack size and priority of each task
    // Also initializes a handler pointer to each task, which are important to communicate with and retrieve info from tasks
    xTaskCreate(readIMU,         "READ IMU DATA",          256, NULL, tskIDLE_PRIORITY + 2, &Handle_aTask);
    xTaskCreate(displayOLED,     "DISPLAY IMU DATA",       256, NULL, tskIDLE_PRIORITY + 3, &Handle_bTask);

    // Start the RTOS, this function will never return and will schedule the tasks.
    vTaskStartScheduler();

}

//*****************************************************************
// This is now the rtos idle loop
// No rtos blocking functions allowed!
//*****************************************************************
void loop() {
    // Optional commands, can comment/uncomment below
    SERIAL.print("."); //print out dots in terminal, we only do this when the RTOS is in the idle state
    vNopDelayMS(100);
}


//*****************************************************************

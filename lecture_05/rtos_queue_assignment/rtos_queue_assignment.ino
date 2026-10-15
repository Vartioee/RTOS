#include <Arduino.h>

QueueHandle_t temperatureQueue;


// --------------------------------------------------
// Sensor Task
// --------------------------------------------------

void sensorTask(void *parameter)
{
  int temperature = 20;

  while (1)
  {
    // TODO 1:
    // Send the temperature to the queue


    // TODO 2:
    // Increase the temperature by 1


    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}


// --------------------------------------------------
// Display Task
// --------------------------------------------------

void displayTask(void *parameter)
{
  int receivedTemperature;

  while (1)
  {
    // TODO 3:
    // Receive a temperature value from the queue


    // TODO 4:
    // If a value was received, print:
    // Temperature: XX C


    vTaskDelay(pdMS_TO_TICKS(100));
  }
}


// --------------------------------------------------
// Setup
// --------------------------------------------------

void setup()
{
  Serial.begin(115200);

  // TODO 5:
  // Create a queue that can store 5 integers


  // TODO 6:
  // Create the Sensor Task


  // TODO 7:
  // Create the Display Task
}


void loop()
{
}
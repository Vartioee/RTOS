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
    // TODO 1: Send the temperature to the queue
    // portMAX_DELAY tells the task to wait indefinitely if the queue is full
    xQueueSend(temperatureQueue, &temperature, portMAX_DELAY);

    // TODO 2: Increase the temperature by 1
    temperature++;

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
    // TODO 3: Receive a temperature value from the queue
    // TODO 4: If a value was received, print: Temperature: XX C
    if (xQueueReceive(temperatureQueue, &receivedTemperature, portMAX_DELAY) == pdPASS) 
    {
      Serial.print("Temperature: ");
      Serial.print(receivedTemperature);
      Serial.println(" C");
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// --------------------------------------------------
// Setup
// --------------------------------------------------
void setup()
{
  Serial.begin(115200);
  delay(500);

  // TODO 5: Create a queue that can store 5 integers
  temperatureQueue = xQueueCreate(5, sizeof(int));

  if (temperatureQueue != NULL) {
    // TODO 6: Create the Sensor Task
    xTaskCreatePinnedToCore(sensorTask, "Sensor Task", 2048, NULL, 1, NULL, 1);

    // TODO 7: Create the Display Task
    xTaskCreatePinnedToCore(displayTask, "Display Task", 2048, NULL, 1, NULL, 1);
  }
}

void loop()
{
  // FreeRTOS scheduler takes over
}
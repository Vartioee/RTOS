#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Both tasks use this queue.
QueueHandle_t myQueue = NULL;

// Change these values for the experiment.
const int producerMs = 1000;
const int consumerMs = 1000;

void producerTask(void *parameter)
{
  int value = 1;
  while (1)
  {
    // Copy the number. Do not wait for free space.
    if (xQueueSend(myQueue, &value, 0) != pdPASS)
    {
      Serial.println("Queue full: value not sent");
    }
    // Increase the number even if sending failed.
    value++;
    vTaskDelay(pdMS_TO_TICKS(producerMs));
  }
}

void consumerTask(void *parameter)
{
  int value;
  while (1)
  {
    // Receive one number. Do not wait for data.
    if (xQueueReceive(myQueue, &value, 0) == pdPASS)
    {
      Serial.printf("Received: %d\n", value);
    }
    vTaskDelay(pdMS_TO_TICKS(consumerMs));
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Create a queue that can hold five integers.
  myQueue = xQueueCreate(5, sizeof(int));
  if (myQueue == NULL)
  {
    Serial.println("Queue creation failed");
    return;
  }

  // Both tasks run on core 1 with priority 1.
  BaseType_t producerResult = xTaskCreatePinnedToCore(
    producerTask, "Producer", 4096, NULL, 1, NULL, 1
  );
  BaseType_t consumerResult = xTaskCreatePinnedToCore(
    consumerTask, "Consumer", 4096, NULL, 1, NULL, 1
  );

  if (producerResult != pdPASS || consumerResult != pdPASS)
  {
    Serial.println("Task creation failed");
  }
}

void loop()
{
  // The producer and consumer perform the work.
  delay(1000);
}

 

 
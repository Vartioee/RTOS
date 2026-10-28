#include <Arduino.h>

TaskHandle_t taskB_handle = NULL;

void taskA(void *parameter) {
  for (;;) {
    const char *msg1 = "Task A is printing slowly...";
    for (int i = 0; msg1[i] != '\0'; i++) {
      Serial.print(msg1[i]);
      delay(50); // Slow print to allow observation
      
      // Send notification to Task B halfway through the message
      if (i == 12) {
        xTaskNotifyGive(taskB_handle);
      }
    }
    Serial.println();
    vTaskDelay(pdMS_TO_TICKS(2000)); // Pause between runs
  }
}

void taskB(void *parameter) {
  for (;;) {
    // Wait indefinitely for notification (enters Blocked state)
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    
    Serial.println("\n[ >>> Task B is running! <<< ]");
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Core 1, Task A Priority = 1, Task B Priority = 2 (Scenario A)
  xTaskCreatePinnedToCore(taskB, "Task B", 2048, NULL, 2, &taskB_handle, 1);
  xTaskCreatePinnedToCore(taskA, "Task A", 2048, NULL, 1, NULL, 1);
}

void loop() {
  // FreeRTOS scheduler handles tasks
}
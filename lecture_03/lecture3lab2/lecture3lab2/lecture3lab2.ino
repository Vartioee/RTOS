#include <Arduino.h>

#define RGB_BUILTIN 38
#define RGB_BRIGHTNESS 50

volatile uint32_t blinkInterval = 500;

TaskHandle_t serialTaskHandle = NULL;
TaskHandle_t ledTaskHandle = NULL;

void serialTask(void *parameter) {
  for (;;) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim();

      if (input == "suspend") {
        if (ledTaskHandle != NULL) {
          vTaskSuspend(ledTaskHandle);
          Serial.println("[SYSTEM] LED Task Suspended.");
        }
      } else if (input == "resume") {
        if (ledTaskHandle != NULL) {
          vTaskResume(ledTaskHandle);
          Serial.println("[SYSTEM] LED Task Resumed.");
        }
      } else {
        int val = input.toInt();
        if (val == 250 || val == 500 || val == 1000) {
          blinkInterval = val;
          Serial.print("[SYSTEM] New interval accepted: ");
          Serial.print(blinkInterval);
          Serial.println(" ms");
        } else {
          Serial.println("[ERROR] Invalid input. Enter 250, 500, 1000, 'suspend', or 'resume'.");
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50)); // Non-blocking check
  }
}

void ledTask(void *parameter) {
  for (;;) {
    neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // Red ON
    vTaskDelay(pdMS_TO_TICKS(blinkInterval));
    neopixelWrite(RGB_BUILTIN, 0, 0, 0);              // OFF
    vTaskDelay(pdMS_TO_TICKS(blinkInterval));
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Both tasks pinned to Core 1 at Priority 1
  xTaskCreatePinnedToCore(serialTask, "Serial Task", 2048, NULL, 1, &serialTaskHandle, 1);
  xTaskCreatePinnedToCore(ledTask, "LED Task", 2048, NULL, 1, &ledTaskHandle, 1);
  
  Serial.println("System Ready. Commands: 250, 500, 1000, suspend, resume");
}

void loop() {
  // FreeRTOS scheduler handles execution
}
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RGB_BUILTIN 38

// Task notification bits
const uint32_t EVENT_UPDATE   = (1 << 0);
const uint32_t EVENT_COMPLETE = (1 << 1);

// RTOS Handles
TaskHandle_t appTaskHandle = NULL;
TimerHandle_t periodicTimer = NULL;
TimerHandle_t oneShotTimer = NULL;

// Session State
enum SessionState { READY, RUNNING, COMPLETED };
SessionState currentState = READY;
uint32_t sessionStartTime = 0;

// Timer Callbacks 
void periodicCallback(TimerHandle_t xTimer) {
  xTaskNotify(appTaskHandle, EVENT_UPDATE, eSetBits);
}

void oneShotCallback(TimerHandle_t xTimer) {
  xTaskNotify(appTaskHandle, EVENT_COMPLETE, eSetBits);
}

// Helper functions for UI
void updateDisplay(String line1, String line2) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(line1);
  if (line2.length() > 0) {
    display.println(line2);
  }
  display.display();
}

void controlLED(bool on) {
  if (on) {
    neopixelWrite(RGB_BUILTIN, 0, 10, 10); // On
  } else {
    neopixelWrite(RGB_BUILTIN, 0, 0, 0);  // Off
  }
}

// Application Task
void applicationTask(void *pvParameters) {
  uint32_t events;
  
  // Show initial ready screen
  updateDisplay("Ready", "");
  controlLED(false);

  while (true) {
    // 1. Check Serial Input (Non-blocking)
    while (Serial.available() > 0) {
      char c = Serial.read();
      if (c == 's' && currentState != RUNNING) {
        // Start Session
        currentState = RUNNING;
        sessionStartTime = millis();
        
        controlLED(true);
        updateDisplay("Session starts", "Remaining: 10 s");
        
        // Start timers
        xTimerStart(periodicTimer, 0);
        xTimerStart(oneShotTimer, 0);
      }
    }

    // 2. Wait for RTOS Timer Events (20ms max block to keep Serial responsive)
    if (xTaskNotifyWait(0, EVENT_UPDATE | EVENT_COMPLETE, &events, pdMS_TO_TICKS(20)) == pdPASS) {
      
      // Handle completion first to prevent late updates from overwriting the screen
      if (events & EVENT_COMPLETE) {
        xTimerStop(periodicTimer, 0);
        controlLED(false);
        currentState = COMPLETED;
        updateDisplay("Session complete", "");
      } 
      else if (events & EVENT_UPDATE) {
        if (currentState == RUNNING) {
          // Calculate remaining time using millis() 
          uint32_t elapsed = (millis() - sessionStartTime) / 1000;
          int32_t remaining = 10 - elapsed;
          if (remaining < 0) remaining = 0;
          
          updateDisplay("Session running", "Remaining: " + String(remaining) + " s");
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize pins Wire.begin(SDA_PIN, SCL_PIN);
  Wire.begin(8, 9);
  // Initialize OLED (I2C address 0x3C)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Initialize Timers
  periodicTimer = xTimerCreate("AutoReload", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, periodicCallback);
  oneShotTimer = xTimerCreate("OneShot", pdMS_TO_TICKS(10000), pdFALSE, (void *)1, oneShotCallback);
  
  if (periodicTimer == NULL || oneShotTimer == NULL) {
    Serial.println("Timer creation failed!");
    for(;;);
  }

  // Create Application Task
  xTaskCreate(applicationTask, "AppTask", 4096, NULL, 1, &appTaskHandle);
}

void loop() {
  // RTOS manages the execution, so Arduino's loop remains empty
  vTaskDelete(NULL);
}
# Timed Display Study

## 1. Why does this system need both a one-shot and an auto-reload timer?
> One-shot is to make sure session lasts 10 seconds and auto-reload handels the 1s countdown on display.

## 2. Which task executes the timer callbacks?
> FreeRTOS timer service task executes all software timer callbacks.

## 3. Why do OLED updates belong in the application task?
> Updating the display over I2C is slow. Placing it there shields the other software clocks from delays or breaking.

## 4. When are the timers dormant?
> Right after when they are created in setup and after the session finishes.

## 5. How does your program prevent extra s commands from extending the session?
> In my program I use `enum SessionState` to avoid creating multiple bool variables that can overlap and create situations where 2 states are active at the same time. After flashing or rebooting the program `SessionState` flag `READY`. When the `s` that starts the program is written into serial, it sets a `SessionState` flag to `RUNNING` and the program ignores other `s` characters. The `EVENT_COMPLETE` resets the flag and the `SessionState` flag to `COMPLETE` so a new one can start.

## 6. Arduino loop
> `vTaskDelete(NULL)` is used to give memory freedom for FreeRTOS to handle everything in custom tasks.

### Libraries
``` cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"

```

| ESP32-S3 | Display |
|----------|---------|
|   3.3V   |   VCC   |
|   GND    |   GND   |
|   PIN8   |   SDA   |
|   PIN9   |   SCL   |

| Test                                       | Expected result                                                           | Results |
|------------------------------------------- |---------------------------------------------------------------------------|---------|
| Restart the board and wait over 10 seconds | Ready screen remains visible; LED stays off                               |  pass   |
| Send s                                     | Session starts, countdown appears and LED turns on                        |  pass   |
| Send s repeatedly during the session       | Original session continues without extending                              |  pass   |
| Allow the session to finish                | Completion screen appears and LED turns off after approximately 10 seconds|  pass   |
| Wait after completion                      | Completion screen remains visible                                         |  pass   |
| Send s again                               | A new session starts with the full duration                               |  pass   |

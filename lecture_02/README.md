# FreeRTOS Task Scheduling & States Lab

> This lab demonstrates how non-blocking delay functions like vTaskDelay() enable efficient multitasking in FreeRTOS. Unlike a CPU-hogging busy loop that wastes processing cycles, vTaskDelay() moves the calling task into the Blocked state. This frees up the CPU core so lower or equal priority tasks can execute smoothly without interruption.

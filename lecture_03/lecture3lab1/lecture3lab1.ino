TaskHandle_t taskAHandle = NULL;
TaskHandle_t taskBHandle = NULL;


xTaskCreatePinnedToCore(taskB, "Task B",
2048, NULL, 2, &taskBHandle, 1);
xTaskCreatePinnedToCore(taskA, "Task A",
2048, NULL, 1, &taskAHandle, 1);
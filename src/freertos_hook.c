#include "FreeRTOS.h"
#include "task.h"

void vApplicationMallocFailedHook(void)
{
    while (1);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    while (1);
}

void vApplicationIdleHook(void)
{
}

void vApplicationTickHook(void)
{
}

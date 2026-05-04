/**
 * @file    Freertos main file
 * @author  MootSeeker
 * @date    2024-09-02
 * @brief   Main file for FreeRTOS tasks and hooks.
 * @license MIT License
 */

#include "lvgl/lvgl.h"

#include "FreeRTOS.h"
#include "task.h"

#if LV_USE_OS == LV_OS_FREERTOS

#include "hal/hal.h"
#include <stdio.h>
//部件定义
static lv_obj_t * label_speed;
static lv_obj_t * btn_up;
static lv_obj_t * btn_down;
static lv_obj_t * btn_stop;
static int32_t speed_val=0;

static int32_t screen_width(void)
{
    return lv_display_get_horizontal_resolution(lv_display_get_default());
}

static int32_t screen_height(void)
{
    return lv_display_get_vertical_resolution(lv_display_get_default());
}

// ........................................................................................................
/**
 * @brief   Malloc failed hook
 *
 * This function is called when a memory allocation (malloc) fails. It logs the available heap size and enters
 * an infinite loop to halt the system.
 *
 * @param   None
 * @return  None
 */
void vApplicationMallocFailedHook(void)
{
    printf("Malloc failed! Available heap: %ld bytes\n", xPortGetFreeHeapSize());
    for( ;; );
}

// ........................................................................................................
/**
 * @brief   Idle hook
 *
 * This function is called when the system is idle. It can be used for low-power mode operations or other
 * maintenance tasks that need to run when the CPU is not busy.
 *
 * @param   None
 * @return  None
 */
void vApplicationIdleHook(void) {}

static void lv_example_label(void)//速度提示
{   //根据屏幕宽度选择合适的字体
    // if(screen_width()<=320){
    //     font=&lv_font_montserrat_10;
    // }
    // else if(screen_width()<=480){
    //     font=&lv_font_montserrat_14;
    // }
    // else {
    //     font=&lv_font_montserrat_20;
    // }
    label_speed=lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label_speed, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_label_set_text(label_speed, "Speed: 0 RPM");
    lv_obj_align(label_speed, LV_ALIGN_TOP_MID, 0, screen_height()/12);

}
static void event_btn_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code==LV_EVENT_CLICKED){
       lv_obj_t * btn = lv_event_get_target(e);
       if(btn==btn_up){
           speed_val+=10;
    }
         else if(btn==btn_down){
              speed_val-=10;
         }
         else if(btn==btn_stop){
              speed_val=0;
         }
         char buf[20];
         sprintf(buf, "Speed: %d RPM", speed_val);
         lv_label_set_text(label_speed, buf);
}
}
static void lv_example_btnup(void)//加速按钮
{
    btn_up=lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn_up, screen_width()/4, screen_height()/6);
    lv_obj_align(btn_up, LV_ALIGN_CENTER, -screen_width()/3, 0);
    lv_obj_add_event_cb(btn_up, event_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label = lv_label_create(btn_up);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_label_set_text(label, "SPEED UP");
    lv_obj_set_align(label, LV_ALIGN_CENTER);

}
static void lv_example_btndown(void)//减速按钮
{
    btn_down=lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn_down, screen_width()/4, screen_height()/6);
    lv_obj_align(btn_down, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn_down, event_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label = lv_label_create(btn_down);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_label_set_text(label, "SPEED DOWN");
    lv_obj_set_align(label, LV_ALIGN_CENTER);

}

static void lv_example_btn_stop(void)//停止按钮
{
    btn_stop=lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn_stop, screen_width()/4, screen_height()/6);
    lv_obj_align(btn_stop, LV_ALIGN_CENTER, screen_width()/3, 0);
    lv_obj_set_style_bg_color(btn_stop, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);//默认状态为红色
    lv_obj_set_style_bg_color(btn_stop, lv_color_hex(0xCC0000), LV_PART_MAIN | LV_STATE_PRESSED);//按下时变暗

    lv_obj_add_event_cb(btn_stop, event_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label = lv_label_create(btn_stop);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_label_set_text(label, "STOP");
    lv_obj_set_align(label, LV_ALIGN_CENTER);

}
// ........................................................................................................
/**
 * @brief   Stack overflow hook
 *
 * This function is called when a stack overflow is detected in a task. It logs the task name and enters
 * an infinite loop to halt the system.
 *
 * @param   xTask        Handle of the task that caused the stack overflow
 * @param   pcTaskName   Name of the task that caused the stack overflow
 * @return  None
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("Stack overflow in task %s\n", pcTaskName);
    for(;;);
}

// ........................................................................................................
/**
 * @brief   Tick hook
 *
 * This function is called on each tick interrupt. It can be used to execute periodic operations
 * that need to occur at a fixed time interval.
 *
 * @param   None
 * @return  None
 */
void vApplicationTickHook(void) {}

static void event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code==LV_EVENT_VALUE_CHANGED){
        printf("Button is checked\n");

}
}
// ........................................................................................................
/**
 * @brief   Create Hello World screen
 *
 * This function creates a simple LVGL screen with a "Hello, World!" label centered on the screen.
 *
 * @param   None
 * @return  None
 */
lv_obj_t * obj2;
lv_obj_t * obj1;

void create_creen()
{
    lv_example_label();
    lv_example_btnup();
    lv_example_btndown();
    lv_example_btn_stop();

}

// ........................................................................................................
/**
 * @brief   LVGL task
 *
 * This task initializes LVGL and runs the main loop, periodically calling the LVGL task handler.
 * It is responsible for managing the LVGL state and rendering updates.
 *
 * @param   pvParameters   Task parameters (not used in this example)
 * @return  None
 */
void lvgl_task(void *pvParameters)
{

    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    sdl_hal_init(320, 480);
    /* Show simple hello world screen */
    create_creen();

    while (true){
        lv_timer_handler(); /* Handle LVGL tasks */
        vTaskDelay(pdMS_TO_TICKS(5)); /* Short delay for the RTOS scheduler */
    }
}

// ........................................................................................................
/**
 * @brief   Another task
 *
 * This task simulates some background work by periodically printing a message.
 *
 * @param   pvParameters   Task parameters (not used in this example)
 * @return  None
 */
void another_task(void *pvParameters)
{
    /* Create some load in an infinite loop */
    while (true){
        printf("Second Task is running :)\n");
        /* Delay the task for 500 milliseconds */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// ........................................................................................................
/**
 * @brief   FreeRTOS main function
 *
 * This function sets up and starts the FreeRTOS tasks, including the LVGL task and another demo task.
 *
 * @param   None
 * @return  None
 */
int main(int argc, char **argv)
{
    LV_UNUSED(argc);
    LV_UNUSED(argv);
    /* Initialize LVGL (Light and Versatile Graphics Library) and other resources */

    /* Create the LVGL task */
    if (xTaskCreate(lvgl_task, "LVGL Task", 4096, NULL, 1, NULL) != pdPASS) {
        printf("Error creating LVGL task\n");
        /* Error handling */
    }

    /* Create another task */
    if (xTaskCreate(another_task, "Another Task", 1024, NULL, 1, NULL) != pdPASS) {
        printf("Error creating another task\n");
        /* Error handling */
    }

    /* Start the scheduler */
    vTaskStartScheduler();
}

#endif

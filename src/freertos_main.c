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
static int16_t val = 0;                         /* 当前值 */

static const lv_font_t *font;                   /* 定义字体 */

static lv_obj_t *label_load;                    /* 加载标题标签 */
static lv_obj_t *label_per;                     /* 百分比标签 */
static lv_obj_t *bar;   
static lv_obj_t *obj;                       /* 进度条 */

static void event_led_cd(lv_event_t * e);

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
static void lv_example_led1(){
    if(screen_width()<480){
        font = &lv_font_montserrat_14;
    }
    else{


        font = &lv_font_montserrat_28;
    }
    obj=lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj, screen_width()*5/6,screen_height()*3/5);
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xefefef), LV_PART_MAIN | LV_STATE_DEFAULT);

    //创建LED1
    lv_obj_t * led1 = lv_led_create(obj);
    lv_obj_set_size(led1,screen_width()/5,screen_height()/5);
    lv_obj_align(led1, LV_ALIGN_CENTER, -screen_width()/5, -screen_height()/15);
    lv_led_set_color(led1, lv_color_hex(0xFF0000));    
    lv_led_off(led1);
    lv_obj_add_event_cb(led1, event_led_cd, LV_EVENT_CLICKED, NULL);

    //添加标签
    lv_obj_t * label1=lv_label_create(obj);
    lv_label_set_text(label1,"ROOM 1");
    lv_obj_set_style_text_font(label1,font,LV_STATE_DEFAULT);
    lv_obj_align_to(label1, led1, LV_ALIGN_OUT_BOTTOM_MID, 0, screen_height()/15);

}

//创建LED2
static void lv_example_led2(){
    //创建LED1
    lv_obj_t * led2 = lv_led_create(obj);
    lv_obj_set_size(led2,screen_width()/5,screen_height()/5);
    lv_obj_align(led2, LV_ALIGN_CENTER,  screen_width()/5, -screen_height()/15);
    lv_led_set_color(led2, lv_color_hex(0xFF0000)); 
    lv_led_off(led2);
    lv_obj_add_event_cb(led2, event_led_cd, LV_EVENT_CLICKED, NULL);



    //添加标签
    lv_obj_t * label2=lv_label_create(obj);
    lv_label_set_text(label2,"ROOM 2");
    lv_obj_set_style_text_font(label2,font,LV_STATE_DEFAULT);
    lv_obj_align_to(label2, led2, LV_ALIGN_OUT_BOTTOM_MID, 0, screen_height()/15);

}
static void event_led_cd(lv_event_t * e)
{
    lv_obj_t *led = lv_event_get_target(e);
    lv_led_toggle(led);
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

void create_screen()
{
    lv_example_led1();
    lv_example_led2();
    
}

// ........................................................................................................
/**spinner
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
    create_screen();

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

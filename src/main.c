#include "lvgl/lvgl.h"

static void create_ui(void)
{
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello STM32 FreeRTOS LVGL");
    lv_obj_center(label);
}

int main(void)
{
    lv_init();

    create_ui();

    while(1) {
        lv_timer_handler();
    }

    return 0;
}

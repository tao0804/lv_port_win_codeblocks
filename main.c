
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <unistd.h>

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static const wchar_t * title = L"LVGL port Windows CodeBlocks.      https://lvgl.io | https://docs.lvgl.io";
static lv_obj_t * list1;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#define  OUTLINE_W    40 //电池图标宽度
#define  OUTLINE_H    20 //电池图标高度

void lv_anim_cb(void* p, int32_t v)
{
    //实现变色效果，电池电量低于20% 红色
    static int32_t cnt;
    if (cnt >= OUTLINE_W * 0.2 && v < OUTLINE_W * 0.2)
    {
        lv_obj_set_style_bg_color(p, lv_color_hex(0xff0000), 0);
    }
    else if (v >= OUTLINE_W * 0.2 && cnt < OUTLINE_W * 0.2)
    {
        lv_obj_set_style_bg_color(p, lv_color_hex(0xff00), 0);
    }
    cnt = v;

    //修改电量颜色obj宽度
    lv_obj_set_width(p, v);

    //修改电池百分比
    lv_obj_t *text = lv_obj_get_child(lv_obj_get_parent(p), -1);
    lv_label_set_text_fmt(text, "%d", v*100/(OUTLINE_W-4));
}
void my_battery(void)
{
    lv_obj_t* outline = lv_obj_create(lv_scr_act());

    //设置border和pading
    lv_obj_set_style_border_width(outline, 2, 0);
    lv_obj_set_style_pad_all(outline, 0, 0);

    //设置圆角
    lv_obj_set_style_radius(outline, 8, 0);

    //关闭滚动条
    lv_obj_clear_flag(outline, LV_OBJ_FLAG_SCROLLABLE);

    //设置宽高
    lv_obj_set_size(outline, OUTLINE_W, OUTLINE_H);

    //居中对齐
    lv_obj_align(outline, LV_ALIGN_TOP_RIGHT, 0, 0);

    //电池电量填充obj
    lv_obj_t* pad = lv_obj_create(outline);


    //设置outline
    lv_obj_set_style_outline_width(pad, 0, 0);
    lv_obj_set_style_outline_pad(pad, 0, 0);
    lv_obj_set_style_border_width(pad, 0, 0);
    //设置背景色
    lv_obj_set_style_bg_color(pad, lv_color_hex(0xff0000), 0);

    //设置宽高
    lv_obj_set_size(pad, OUTLINE_W, OUTLINE_H-4);
    lv_obj_set_style_border_width(pad, 0, 0);

    //设置圆角
    lv_obj_set_style_radius(pad, 8, 0);

    //居中对齐
    lv_obj_align(pad, LV_ALIGN_LEFT_MID, 0, 0);

    //关闭滚动条
    lv_obj_clear_flag(pad, LV_OBJ_FLAG_SCROLLABLE);

    //电池百分比
    lv_obj_t* label = lv_label_create(outline);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    //设置动画， 模仿电池电量变化
    lv_anim_t a;
    lv_anim_init(&a);

    /*Set the "animator" function*/
    lv_anim_set_exec_cb(&a, lv_anim_cb);

    /*Set the "animator" function*/
    lv_anim_set_var(&a, pad);

    /*Length of the animation [ms]*/
    lv_anim_set_time(&a, 10000);

    /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_values(&a, 0, OUTLINE_W-4);

    /*Time to wait before starting the animation [ms]*/
    lv_anim_set_delay(&a, 1000);

    /*Play the animation backward too with this duration. Default is 0 (disabled) [ms]*/
    lv_anim_set_playback_time(&a, 0);

    /*Delay before playback. Default is 0 (disabled) [ms]*/
    lv_anim_set_playback_delay(&a, 0);

    /*Number of repetitions. Default is 1.  LV_ANIM_REPEAT_INFINIT for infinite repetition*/
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    /*Delay before repeat. Default is 0 (disabled) [ms]*/
    lv_anim_set_repeat_delay(&a, 1000);

    /* START THE ANIMATION
     *------------------*/
    lv_anim_start(&a);                             /*Start the animation*/
}

void lv_example_tabview_1(void)
{
    lv_obj_t * btn;

    /*Create a Tab view object*/
    lv_obj_t * tabview;
    tabview = lv_tabview_create(lv_scr_act());
    lv_obj_set_size(tabview, 220, 270);  // 设置整体大小
    lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, 20);
    lv_tabview_set_tab_bar_size(tabview, 30);  // 设置选项卡高度

    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Config");
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Comm");
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "About");

    static lv_style_t style;
    lv_style_init(&style);
    /*Set a background color and a radius*/
    lv_style_set_radius(&style, 0);
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP); // 设置透明背景
//    lv_style_set_bg_color(&style, lv_palette_lighten(LV_PALETTE_GREEN, 2));
//    lv_style_set_border_color(&style, lv_palette_lighten(LV_PALETTE_GREEN, 1));
    lv_style_set_border_width(&style, 0);


    /*Add content to the tab1*/
    list1 = lv_list_create(tab1);
    lv_obj_set_size(list1, 220, 210);
    lv_obj_align(list1, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_style(list1, &style, 0);

    btn = lv_list_add_button(list1, NULL, "Freq:");
    lv_obj_add_event_cb(btn, NULL, LV_EVENT_ALL, NULL);
    lv_group_remove_obj(btn);

    btn = lv_list_add_button(list1, NULL, "Rate:");
    lv_obj_add_event_cb(btn, NULL, LV_EVENT_ALL, NULL);
    lv_group_remove_obj(btn);

    btn = lv_list_add_button(list1, NULL, "Volume:");
    lv_obj_add_event_cb(btn, NULL, LV_EVENT_ALL, NULL);
    lv_group_remove_obj(btn);

    btn = lv_list_add_button(list1, NULL, "Brightness:");
    lv_obj_add_event_cb(btn, NULL, LV_EVENT_ALL, NULL);
    lv_group_remove_obj(btn);


    /*Add content to the tab2*/
    lv_obj_t * table = lv_table_create(tab2);

    /*Fill the first column*/
    lv_table_set_cell_value(table, 0, 0, "ISSI");
    lv_table_set_cell_value(table, 1, 0, "TXCnt");
    lv_table_set_cell_value(table, 2, 0, "RXCnt");

    /*Fill the second column*/
    lv_table_set_cell_value(table, 0, 1, "0db");
    lv_table_set_cell_value(table, 1, 1, "0");
    lv_table_set_cell_value(table, 2, 1, "0");

    /*Set a smaller height to the table. It'll make it scrollable*/
    lv_table_set_col_width(table, 0, 100);
    lv_table_set_col_width(table, 1, 100);
//    lv_obj_set_height(table, 220);
    lv_obj_align(table, LV_ALIGN_TOP_MID, 0, 0);


    /*Add content to the tab3*/
    lv_obj_t * label = lv_label_create(tab3);
    lv_label_set_text(label, "Version: 0.0.1");

    // 去除所有的滚动条
    lv_obj_set_scrollbar_mode(table, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(tabview, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(tab2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(list1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL for LVGL*/
    lv_display_t * display = lv_windows_create_display(title, 240, 320, 100, FALSE, FALSE);
    lv_windows_acquire_pointer_indev(display);

    /*Output prompt information to the console, you can also use printf() to print directly*/
    LV_LOG_USER("LVGL initialization completed!");

    /*Run the demo*/
    lv_example_tabview_1();
    my_battery();

    while(1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();
        usleep(5000);       /*Just to let the system breath*/
    }
    return 0;
}

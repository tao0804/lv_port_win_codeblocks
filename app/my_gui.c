#include "my_gui.h"
#include "lvgl/lvgl.h"
#include "lv_drivers/win32drv/win32drv.h"

#define  BATTERY_OUTLINE_W    40 //电池图标宽度
#define  BATTERY_OUTLINE_H    20 //电池图标高度

static void my_gui_battery_cb(void* p, int32_t v)
{
    //实现变色效果，电池电量低于20% 红色
    static int32_t cnt;
    if (cnt >= BATTERY_OUTLINE_W * 0.2 && v < BATTERY_OUTLINE_W * 0.2)
    {
        lv_obj_set_style_bg_color(p, lv_color_hex(0xff0000), 0);
    }
    else if (v >= BATTERY_OUTLINE_W * 0.2 && cnt < BATTERY_OUTLINE_W * 0.2)
    {
        lv_obj_set_style_bg_color(p, lv_color_hex(0xff00), 0);
    }
    cnt = v;

    //修改电量颜色obj宽度
    lv_obj_set_width(p, v);

    //修改电池百分比
    lv_obj_t *text = lv_obj_get_child(lv_obj_get_parent(p), -1);
    lv_label_set_text_fmt(text, "%d", v*100/(BATTERY_OUTLINE_W-4));
}

static void my_gui_battery_init(void)
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
    lv_obj_set_size(outline, BATTERY_OUTLINE_W, BATTERY_OUTLINE_H);

    //居中对齐
    lv_obj_align(outline, LV_ALIGN_CENTER, 0, 0);

    //电池电量填充obj
    lv_obj_t* pad = lv_obj_create(outline);


    //设置outline
    lv_obj_set_style_outline_width(pad, 0, 0);
    lv_obj_set_style_outline_pad(pad, 0, 0);
    lv_obj_set_style_border_width(pad, 0, 0);
    //设置背景色
    lv_obj_set_style_bg_color(pad, lv_color_hex(0xff0000), 0);

    //设置宽高
    lv_obj_set_size(pad, BATTERY_OUTLINE_W, BATTERY_OUTLINE_H-4);
    lv_obj_set_style_border_width(pad, 0, 0);

    //设置圆角
    lv_obj_set_style_radius(pad, 8, 0);

    //右上显示
    lv_obj_align(outline, LV_ALIGN_TOP_RIGHT, 0, 0);

    //关闭滚动条
    lv_obj_clear_flag(pad, LV_OBJ_FLAG_SCROLLABLE);

    //电池百分比
    lv_obj_t* label = lv_label_create(outline);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    //设置动画， 模仿电池电量变化
    lv_anim_t a;
    lv_anim_init(&a);

    /*Set the "animator" function*/
    lv_anim_set_exec_cb(&a, my_gui_battery_cb);

    /*Set the "animator" function*/
    lv_anim_set_var(&a, pad);

    /*Length of the animation [ms]*/
    lv_anim_set_time(&a, 10000);

    /*Set start and end values. E.g. 0, 150*/
    lv_anim_set_values(&a, 0, BATTERY_OUTLINE_W-4);

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

static void my_gui_tabview_init(void)
{
    /*Create a Tab view object*/
    lv_obj_t * tabview;
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 40);  // tab_size 设置选项卡高度
    lv_obj_set_size(tabview, 240, 300);  // 设置整体大小
    lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, 20);

#if 1
    // tabview中增加一个tab
    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Config");
    lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_OFF);     // 关滚动栏

    // 创建样式，应用于list，直角边，透明背景，无边界值
    static lv_style_t style;    // 需要为static，不然不起作用
    lv_style_init(&style);
    lv_style_set_radius(&style, 0);
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP); // 设置透明背景
    lv_style_set_border_width(&style, 0);

    /*Add content to the tab1*/
    lv_obj_t * list1;
    list1 = lv_list_create(tab1);
    lv_obj_set_size(list1, 240, 260);
    lv_obj_align(list1, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(list1, &style, 0);

    // 创建按键
    lv_obj_t * btn1;
    lv_obj_t * btn2;
    lv_obj_t * btn3;
    lv_obj_t * btn4;
    btn1 = lv_list_add_btn(list1, NULL, "Freq:");
    lv_obj_add_event_cb(btn1, NULL, LV_EVENT_ALL, NULL);

    btn2 = lv_list_add_btn(list1, NULL, "Rate:");
    lv_obj_add_event_cb(btn2, NULL, LV_EVENT_ALL, NULL);

    btn3 = lv_list_add_btn(list1, NULL, "Volume:");
    lv_obj_add_event_cb(btn3, NULL, LV_EVENT_ALL, NULL);

    btn4 = lv_list_add_btn(list1, NULL, "Brightness:");
    lv_obj_add_event_cb(btn4, NULL, LV_EVENT_ALL, NULL);

    // 创建group
    lv_group_t * group1 = lv_group_create();
    lv_indev_set_group(lv_win32_keypad_device_object, group1);     // 将键盘和组关联
    lv_group_add_obj(group1, btn1);
    lv_group_add_obj(group1, btn2);
    lv_group_add_obj(group1, btn3);
    lv_group_add_obj(group1, btn4);
    lv_group_set_default(group1);   // 设置为默认组
#endif

#if 1
    // tabview中增加一个tab
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Comm");
    lv_obj_set_scrollbar_mode(tab2, LV_SCROLLBAR_MODE_OFF);

    /*Add content to the tab2*/
    lv_obj_t * table = lv_table_create(tab2);
    lv_group_remove_obj(table); // 不分配group
    lv_table_set_cell_value(table, 0, 0, "ISSI");
    lv_table_set_cell_value(table, 0, 1, "0db");
    lv_table_set_cell_value(table, 1, 0, "TXCnt");
    lv_table_set_cell_value(table, 1, 1, "0");
    lv_table_set_cell_value(table, 2, 0, "RXCnt");
    lv_table_set_cell_value(table, 2, 1, "0");

    /*Set a smaller height to the table. It'll make it scrollable*/
    lv_table_set_col_width(table, 0, 105);
    lv_table_set_col_width(table, 1, 105);
    // lv_obj_set_height(table, 220);
    lv_obj_align(table, LV_ALIGN_TOP_MID, 0, 0);
#endif

#if 1
    // tabview中增加一个tab
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "About");
    lv_obj_set_scrollbar_mode(tab3, LV_SCROLLBAR_MODE_OFF);

    /*Add content to the tab3*/
    lv_obj_t * label = lv_label_create(tab3);
    lv_label_set_text(label, "Version: 0.0.2");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);
#endif
}

void my_gui_init(void)
{
    my_gui_tabview_init();
    my_gui_battery_init();
}

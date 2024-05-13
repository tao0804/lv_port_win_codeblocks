#include "my_gui.h"
#include "lvgl/lvgl.h"
#include "lv_drivers/win32drv/win32drv.h"

void my_gui_init(void)
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

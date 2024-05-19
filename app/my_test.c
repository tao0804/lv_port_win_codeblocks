#include "my_test.h"
#include "lvgl/lvgl.h"
#include "lv_drivers/win32drv/win32drv.h"

void lv_100ask_demo_course_3_3_1(void)
{
    // ����һ���飬�Ժ���Ҫʹ�ü��̻��������ť���ƵĲ���(����)��ӽ�ȥ�����ҽ������豸�������
    // ��������������ΪĬ���飬��ô������Щ�ڴ���ʱ����ӵ�Ĭ����Ĳ���(����)�Ϳ���ʡ�� lv_group_add_obj()
    lv_group_t * g = lv_group_create();

    // �����洴����������ΪĬ����
    // ����Ժ󴴽��Ĳ���(����)��ʹ��Ĭ�����Ǳ���Ҫ���䴴��֮ǰ���ú�Ĭ���飬������Ч
    lv_group_set_default(g);

    // �������豸�������(ʹ��ǰ�ȴ�����ע�͵���ͷ�ļ�)
    lv_indev_set_group(lv_win32_keypad_device_object, g);     // ����
    // lv_indev_set_group(lv_win32_encoder_device_object, g);      // ����ϵĹ���(������)


    /* ����һ��btn����(����) */
    lv_obj_t * btn1 = lv_btn_create(lv_scr_act());       // ����һ��btn����(����),���ĸ������ǻ��Ļ����
    lv_obj_set_size(btn1, 100, 50);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -100);

    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());       // ����һ��btn����(����),���ĸ������ǻ��Ļ����
    lv_obj_set_size(btn2, 100, 50);
    lv_obj_align_to(btn2, btn1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t * btn3 = lv_btn_create(lv_scr_act());       // ����һ��btn����(����),���ĸ������ǻ��Ļ����
    lv_obj_set_size(btn3, 100, 50);
    lv_obj_align_to(btn3, btn2, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t * btn4 = lv_btn_create(lv_scr_act());       // ����һ��btn����(����),���ĸ������ǻ��Ļ����
    lv_obj_set_size(btn4, 100, 50);
    lv_obj_align_to(btn4, btn3, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    // lv_obj_t * slider = lv_slider_create(lv_scr_act());
    // lv_obj_align_to(slider, btn4, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    // ������(����)��ӵ��飬���������Ĭ���飬�������ʡ��
    lv_group_add_obj(g, btn1);
    lv_group_add_obj(g, btn2);
    lv_group_add_obj(g, btn3);
    lv_group_add_obj(g, btn4);
    // lv_group_add_obj(g, slider);

    lv_group_focus_obj(btn1);
}

static lv_obj_t * list1;

static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked: %s", lv_list_get_btn_text(list1, obj));
    }
}

void lv_tao_example_list_1(void)
{
    /*Create a list*/
    list1 = lv_list_create(lv_scr_act());
    lv_obj_set_size(list1, 240, 320);
    lv_obj_center(list1);

    /*Add buttons to the list*/
    lv_obj_t * btn;

    lv_list_add_text(list1, "File");
    btn = lv_list_add_btn(list1, LV_SYMBOL_FILE, "New");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_DIRECTORY, "Open");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_SAVE, "Save");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_CLOSE, "Delete");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_EDIT, "Edit");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_list_add_text(list1, "Connectivity");
    btn = lv_list_add_btn(list1, LV_SYMBOL_BLUETOOTH, "Bluetooth");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_GPS, "Navigation");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_USB, "USB");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_BATTERY_FULL, "Battery");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_list_add_text(list1, "Exit");
    btn = lv_list_add_btn(list1, LV_SYMBOL_OK, "Apply");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
    btn = lv_list_add_btn(list1, LV_SYMBOL_CLOSE, "Close");
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);
}
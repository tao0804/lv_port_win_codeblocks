#include "my_gui.h"
#include "lvgl/lvgl.h"
#include "lv_drivers/win32drv/win32drv.h"

#define  BATTERY_OUTLINE_W    40 //���ͼ����
#define  BATTERY_OUTLINE_H    20 //���ͼ��߶�

static void my_gui_battery_cb(void* p, int32_t v)
{
    //ʵ�ֱ�ɫЧ������ص�������20% ��ɫ
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

    //�޸ĵ�����ɫobj���
    lv_obj_set_width(p, v);

    //�޸ĵ�ذٷֱ�
    lv_obj_t *text = lv_obj_get_child(lv_obj_get_parent(p), -1);
    lv_label_set_text_fmt(text, "%d", v*100/(BATTERY_OUTLINE_W-4));
}

static void my_gui_battery_init(void)
{
    lv_obj_t* outline = lv_obj_create(lv_scr_act());

    //����border��pading
    lv_obj_set_style_border_width(outline, 2, 0);
    lv_obj_set_style_pad_all(outline, 0, 0);

    //����Բ��
    lv_obj_set_style_radius(outline, 8, 0);

    //�رչ�����
    lv_obj_clear_flag(outline, LV_OBJ_FLAG_SCROLLABLE);

    //���ÿ��
    lv_obj_set_size(outline, BATTERY_OUTLINE_W, BATTERY_OUTLINE_H);

    //���ж���
    lv_obj_align(outline, LV_ALIGN_CENTER, 0, 0);

    //��ص������obj
    lv_obj_t* pad = lv_obj_create(outline);


    //����outline
    lv_obj_set_style_outline_width(pad, 0, 0);
    lv_obj_set_style_outline_pad(pad, 0, 0);
    lv_obj_set_style_border_width(pad, 0, 0);
    //���ñ���ɫ
    lv_obj_set_style_bg_color(pad, lv_color_hex(0xff0000), 0);

    //���ÿ��
    lv_obj_set_size(pad, BATTERY_OUTLINE_W, BATTERY_OUTLINE_H-4);
    lv_obj_set_style_border_width(pad, 0, 0);

    //����Բ��
    lv_obj_set_style_radius(pad, 8, 0);

    //������ʾ
    lv_obj_align(outline, LV_ALIGN_TOP_RIGHT, 0, 0);

    //�رչ�����
    lv_obj_clear_flag(pad, LV_OBJ_FLAG_SCROLLABLE);

    //��ذٷֱ�
    lv_obj_t* label = lv_label_create(outline);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    //���ö����� ģ�µ�ص����仯
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

struct my_gui_config_value
{
    uint16_t cfg_range;
    uint16_t cfg_value;
    char ** cfg_str;
};

char *cfg_freq[] = {"2430MHz", "2440MHz", "2450MHz"};
char *cfg_rate[] = {"100Kbps","500Kbps","1Mbps"};
char *cfg_percent[] = {"0\%", "10\%", "20\%", "30\%", "40\%", "50\%", "60\%", "70\%", "80\%", "90\%", "100\%"};

// ��ʱ����
static void my_gui_btn_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    uint32_t key = *(uint32_t*)lv_event_get_param(e);
    lv_obj_t *label = lv_obj_get_user_data(btn);    // ��ȡlabel����
    struct my_gui_config_value *cfg = lv_obj_get_user_data(label);    // ��ȡcfg

    if (event_code != LV_EVENT_KEY) {
        return; // ���ص�ֻ����KEY�¼�
    }

    if (key == LV_KEY_RIGHT) {
        // ����������
        if (cfg->cfg_value + 1 < cfg->cfg_range) {
            cfg->cfg_value++;
            lv_label_set_text(label, cfg->cfg_str[cfg->cfg_value]);
        }
    } else if (key == LV_KEY_LEFT) {
        // ��С������
        if (cfg->cfg_value > 0) {
            cfg->cfg_value--;
            lv_label_set_text(label, cfg->cfg_str[cfg->cfg_value]);
        }
    }

}

// �����ı�������
static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);
    LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}

static void my_gui_tabview_init(void)
{
    /*Create a Tab view object*/
    lv_obj_t * tabview;
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 40);  // tab_size ����ѡ��߶�
    lv_obj_set_size(tabview, 240, 300);  // ���������С
    lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, 20);
	lv_group_t * g1 = lv_group_create();

#if 1
    // tabview������һ��tab
    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Real-time");
    lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_OFF);     // �ع�����

	static lv_style_t style1;    // ��ҪΪstatic����Ȼ��������
    lv_style_init(&style1);
    lv_style_set_radius(&style1, 0);	// tao:��Բ��
    lv_style_set_bg_opa(&style1, LV_OPA_TRANSP); // ����͸��
    lv_style_set_border_width(&style1, 0);	// �ޱ߿�

    /*Add content to the tab2*/
    lv_obj_t * table1 = lv_table_create(tab1);
    lv_obj_set_scrollbar_mode(table1, LV_SCROLLBAR_MODE_OFF);     // �ع�����
    lv_group_remove_obj(table1); // ������group
	lv_obj_add_style(table1, &style1, 0);

	// tao:���ñ�������������
	lv_table_set_row_cnt(table1, 4);
	lv_table_set_col_cnt(table1, 2);

	// ����һ��label,����120���,�۵��Ҷ���
	lv_obj_t * label1 = lv_label_create(table1);
	lv_label_set_text(label1, "Authorization Code:");
	lv_obj_set_width(label1, 120);
	lv_obj_align(label1, LV_ALIGN_TOP_MID, -45, 0);

	lv_obj_t * ta = lv_textarea_create(table1);	//����table����Textarea
	lv_textarea_set_one_line(ta, true);			// �����ı�����Ϊ����ģʽ
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 140, 4);
    lv_textarea_set_accepted_chars(ta, "0123456789");		// ���������ֻ����������
    lv_textarea_set_max_length(ta, 5);		// ������볤��Ϊ5���ַ�
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);
	lv_group_add_obj(g1, ta);

	// lv_obj_t * btn;
	// lv_obj_add_event_cb(btn, my_gui_btn_cb, LV_EVENT_VALUE_CHANGED, ta);


	// ʵ��ֵ�͵�λ��Ҫת��һ���ַ���,��ʱ��gpt��δ���
	lv_table_set_cell_value(table1, 1, 0, "Current Voltage");
    lv_table_set_cell_value(table1, 1, 1, "0 V");
    lv_table_set_cell_value(table1, 2, 0, "Current Current");
    lv_table_set_cell_value(table1, 2, 1, "0 A");
    lv_table_set_cell_value(table1, 3, 0, "Current Power");
    lv_table_set_cell_value(table1, 3, 1, "0 W");

    // lv_table_set_cell_value(table1, 0, 0, "Current Voltage");
    // lv_table_set_cell_value(table1, 0, 1, "0 V");
    // lv_table_set_cell_value(table1, 1, 0, "Current Current");
    // lv_table_set_cell_value(table1, 1, 1, "0 A");
    // lv_table_set_cell_value(table1, 2, 0, "Current Power");
    // lv_table_set_cell_value(table1, 2, 1, "0 W");

    /*Set a smaller height to the table. It'll make it scrollable*/
    lv_table_set_col_width(table1, 0, 120);
    lv_table_set_col_width(table1, 1, 120);
    lv_obj_align(table1, LV_ALIGN_TOP_MID, 0, 0);

    
#endif

#if 1
    // tabview������һ��tab
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Charge");
    lv_obj_set_scrollbar_mode(tab2, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style2;    // ��ҪΪstatic����Ȼ��������
    lv_style_init(&style2);
    lv_style_set_radius(&style2, 0);	// tao:��Բ��
    lv_style_set_bg_opa(&style2, LV_OPA_TRANSP); // ����͸��
    lv_style_set_border_width(&style2, 0);	// �ޱ߿�

    /*Add content to the tab2*/
    lv_obj_t * table = lv_table_create(tab2);
    lv_group_remove_obj(table); // ������group
	lv_obj_add_style(table, &style2, 0);

	// tao:���ñ�������������
	lv_table_set_row_cnt(table, 4);
	lv_table_set_col_cnt(table, 2);

    lv_table_set_cell_value(table, 0, 0, "Charge Voltage");
    lv_table_set_cell_value(table, 0, 1, "0 V");
    lv_table_set_cell_value(table, 1, 0, "Charge Current");
    lv_table_set_cell_value(table, 1, 1, "0 A");
    lv_table_set_cell_value(table, 2, 0, "Quantity of Electricity");
    lv_table_set_cell_value(table, 2, 1, "0 %");
	lv_table_set_cell_value(table, 3, 0, "Scheduled Time");
    lv_table_set_cell_value(table, 3, 1, "0 H");

    /*Set a smaller height to the table. It'll make it scrollable*/
    lv_table_set_col_width(table, 0, 120);
    lv_table_set_col_width(table, 1, 120);
    lv_obj_align(table, LV_ALIGN_TOP_MID, 0, 0);
    // lv_obj_set_height(table, 220);
#endif

#if 1
    // tabview������һ��tab
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "Discharge");
    lv_obj_set_scrollbar_mode(tab3, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style3;    // ��ҪΪstatic����Ȼ��������
    lv_style_init(&style3);
    lv_style_set_radius(&style3, 0);	// tao:��Բ��
    lv_style_set_bg_opa(&style3, LV_OPA_TRANSP); // ����͸��
    lv_style_set_border_width(&style3, 0);	// �ޱ߿�

    /*Add content to the tab2*/
	lv_obj_t * table3 = lv_table_create(tab3);
    lv_group_remove_obj(table3); // ������group
	lv_obj_add_style(table3, &style3, 0);

	// tao:���ñ�������������
	lv_table_set_row_cnt(table3, 4);
	lv_table_set_col_cnt(table3, 2);

    lv_table_set_cell_value(table3, 0, 0, "Discharge Voltage");
    lv_table_set_cell_value(table3, 0, 1, "0 V");
    lv_table_set_cell_value(table3, 1, 0, "Discharge Current");
    lv_table_set_cell_value(table3, 1, 1, "0 A");
    lv_table_set_cell_value(table3, 2, 0, "Remaining Capacity");
    lv_table_set_cell_value(table3, 2, 1, "0 %");
	lv_table_set_cell_value(table3, 3, 0, "Accumulated Discharge Power");
    lv_table_set_cell_value(table3, 3, 1, "0 KWH");

	lv_table_set_col_width(table3, 0, 120);
    lv_table_set_col_width(table3, 1, 120);
    lv_obj_align(table3, LV_ALIGN_TOP_MID, 0, 0);
#endif
}

void my_gui_init(void)
{
    my_gui_tabview_init();
    my_gui_battery_init();
}

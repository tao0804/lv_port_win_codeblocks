#include "my_gui.h"
#include "lvgl/lvgl.h"
#include "lv_drivers/win32drv/win32drv.h"

#define  BATTERY_OUTLINE_W    40 //电池图标宽度
#define  BATTERY_OUTLINE_H    20 //电池图标高度

static lv_obj_t * tabview;
static lv_obj_t *btn, *btn1, *btn2, *btn3;
static lv_obj_t *group1, *group2;		// a little trick 
static lv_obj_t * ta;
uint8_t cur_id;

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

struct my_gui_config_value
{
    uint16_t cfg_range;
    uint16_t cfg_value;
    char ** cfg_str;
};

char *cfg_freq[] = {"2430MHz", "2440MHz", "2450MHz"};
char *cfg_rate[] = {"100Kbps","500Kbps","1Mbps"};
char *cfg_percent[] = {"0\%", "10\%", "20\%", "30\%", "40\%", "50\%", "60\%", "70\%", "80\%", "90\%", "100\%"};

// 暂时不用
static void my_gui_btn_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    uint32_t key = *(uint32_t*)lv_event_get_param(e);
    lv_obj_t *label = lv_obj_get_user_data(btn);    // 获取label对象
    struct my_gui_config_value *cfg = lv_obj_get_user_data(label);    // 获取cfg

    if (event_code != LV_EVENT_KEY) {
        return; // 本回调只处理KEY事件
    }

    if (key == LV_KEY_RIGHT) {
        // 增大配置项
        if (cfg->cfg_value + 1 < cfg->cfg_range) {
            cfg->cfg_value++;
            lv_label_set_text(label, cfg->cfg_str[cfg->cfg_value]);
        }
    } else if (key == LV_KEY_LEFT) {
        // 减小配置项
        if (cfg->cfg_value > 0) {
            cfg->cfg_value--;
            lv_label_set_text(label, cfg->cfg_str[cfg->cfg_value]);
        }
    }
}

static void my_gui_set_group(lv_group_t * g)
{
    if(g == group1){
		lv_group_remove_all_objs(group1);	// 重置group的内容
		lv_group_add_obj(group1, lv_tabview_get_tab_btns(tabview));	// tabview按键也加进group1
		lv_group_add_obj(group1, ta);
		lv_group_add_obj(group1, btn1);
		lv_group_add_obj(group1, btn2);
		lv_group_add_obj(group1, btn3);
		lv_indev_set_group(lv_win32_keypad_device_object, group1);	// 大写一个牛
	}else if(g == group2){
		lv_group_remove_all_objs(group2);
		lv_group_add_obj(group2, lv_tabview_get_tab_btns(tabview));
		lv_indev_set_group(lv_win32_keypad_device_object, group2);
	}
}

static void my_gui_tv_cb(lv_event_t * e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
	lv_tabview_t *tv = tabview;
	uint32_t key = *(uint32_t *)lv_event_get_param(e);		// 获取参数值赋给key

	// 创全局变量cur_id
	// uint8_t act_id;

	if(event_code != LV_EVENT_KEY){
		return;	// 非key事件不处理
	}

	if (key == LV_KEY_RIGHT){
		if(cur_id < 2){
			cur_id = cur_id + 1;
		}
		else{
			cur_id = 0;
		}
		LV_LOG_USER("right, cur:%d", cur_id);
	}else if(key == LV_KEY_LEFT){
		if(cur_id > 0){
			cur_id = cur_id - 1;
		}
		else{
			cur_id = 2;
		}
		LV_LOG_USER("left, cur:%d", cur_id);
	}
	
	if(cur_id == 0){
		my_gui_set_group(group1);
	}else{
		my_gui_set_group(group2);
	}
	lv_tabview_set_act(tv, cur_id, LV_ANIM_OFF);	// 切换活动的选项卡
}


// 处理文本框输入
static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);		// 获取触发事件
    LV_LOG_USER("Enter was pressed. The current text is: %s", lv_textarea_get_text(ta));
}

static void my_gui_tabview_init(void)
{
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 40);  // tab_size 设置选项卡高度
    lv_obj_set_size(tabview, 240, 300);  // 设置整体大小
    lv_obj_align(tabview, LV_ALIGN_TOP_MID, 0, 20);

#if 1
    // tabview中增加一个tab1
    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Real-time");
    lv_obj_set_scrollbar_mode(tab1, LV_SCROLLBAR_MODE_OFF);     // 关滚动栏

	//创建样式,应用于list,直角边,透明背景,无边界值 
	static lv_style_t style1;
    lv_style_init(&style1);
    lv_style_set_radius(&style1, 0);
    lv_style_set_bg_opa(&style1, LV_OPA_TRANSP);
    lv_style_set_border_width(&style1, 0);

    lv_obj_t * list1 = lv_list_create(tab1);
	lv_obj_set_size(list1, 240, 300);
	lv_obj_align(list1, LV_ALIGN_TOP_MID, 0, 0);
	lv_obj_add_style(list1, &style1, 0);

	// 创建按键,创建textarea
	btn = lv_list_add_btn(list1, NULL, "Authorize\nCode:"); // v8.3版本中,如果字符串太长,默认情况下会启用滚动效果
	ta = lv_textarea_create(btn);	// 花了一点时间想到还能这样
	lv_textarea_set_one_line(ta, true);
	lv_textarea_set_accepted_chars(ta, "0123456789");
	lv_textarea_set_max_length(ta, 5);
	lv_obj_set_width(ta, 100);
	lv_obj_align(ta, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_width(ta, 100);
	lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);


    btn1 = lv_list_add_btn(list1, NULL, "Current\nVoltage:");
	// 弹性布局(和对齐方式是两种不同的布局管理方法,在设置了弹性布局后,对象的位置和对齐方式将由弹性布局管理
	lv_obj_set_flex_flow(btn1, LV_FLEX_FLOW_ROW);

    lv_obj_t * label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "0 V");
    lv_obj_set_flex_grow(label1, 1);    // 设置标签成长比例为1,这样它们会在可用空间内平均分配

	// carefull
	lv_obj_set_flex_align(btn1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    btn2 = lv_list_add_btn(list1, NULL, "Current\nCurrent:");
	lv_obj_set_flex_flow(btn2, LV_FLEX_FLOW_ROW);
	lv_obj_t * label2 = lv_label_create(btn2);
	lv_label_set_text(label2, "0 A");
	lv_obj_set_flex_grow(label2, 1);
	lv_obj_set_flex_align(btn2, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    btn3 = lv_list_add_btn(list1, NULL, "Current\nPower:");
	lv_obj_set_flex_flow(btn3, LV_FLEX_FLOW_ROW);
	lv_obj_t * label3 = lv_label_create(btn3);
	lv_label_set_text(label3, "0 A");
	lv_obj_set_flex_grow(label3, 1);
	lv_obj_set_flex_align(btn3, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
 
	// 创建和设置group
	group1 = lv_group_create();
	my_gui_set_group(group1);

	// 创建group2,tabview按键回调
	group2 = lv_group_create();
	// my_gui_set_group(group2);
	lv_obj_add_event_cb(lv_tabview_get_tab_btns(tabview), my_gui_tv_cb, LV_EVENT_KEY, NULL);

     
#endif

#if 1
    // tabview中增加一个tab
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Charge");
    lv_obj_set_scrollbar_mode(tab2, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style2;    // 需要为static，不然不起作用
    lv_style_init(&style2);
    lv_style_set_radius(&style2, 0);	// tao:无圆角
    lv_style_set_bg_opa(&style2, LV_OPA_TRANSP); // 背景透明
    lv_style_set_border_width(&style2, 0);	// 无边框

    /*Add content to the tab2*/
    lv_obj_t * table = lv_table_create(tab2);
    lv_group_remove_obj(table); // 不分配group
	lv_obj_add_style(table, &style2, 0);

	// tao:设置表格的行数和列数
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
    // tabview中增加一个tab
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "Discharge");
    lv_obj_set_scrollbar_mode(tab3, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style3;    // 需要为static，不然不起作用
    lv_style_init(&style3);
    lv_style_set_radius(&style3, 0);	// tao:无圆角
    lv_style_set_bg_opa(&style3, LV_OPA_TRANSP); // 背景透明
    lv_style_set_border_width(&style3, 0);	// 无边框

    /*Add content to the tab2*/
	lv_obj_t * table3 = lv_table_create(tab3);
    lv_group_remove_obj(table3); // 不分配group
	lv_obj_add_style(table3, &style3, 0);

	// tao:设置表格的行数和列数
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

#include "ssd1306.h"
#include "../../mGUI/mgui.h"
#include "../../test/font_16x8.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/timer.h"

#include "build/quadrature_encoder.pio.h"

#define BUTTON_GPIO 15

// Base pin to connect the A phase of the encoder.
// The B phase must be connected to the next pin
#define ENCODER_FIRST_GPIO 18

const uint sm = 0;

static void button_gpio_init() {
    gpio_init(BUTTON_GPIO);
    gpio_set_dir(BUTTON_GPIO, GPIO_IN);
    // We are using the button to pull down to 0v when pressed, so ensure that when
    // unpressed, it uses internal pull ups. Otherwise when unpressed, the input will
    // be floating.
    gpio_pull_up(BUTTON_GPIO);
}

static int update_quadrature_encoder(PIO pio) {
    static int new_value = 0;
    static int old_value = 0;
    int delta = 0;

    // note: thanks to two's complement arithmetic delta will always
    // be correct even when new_value wraps around MAXINT / MININT
    new_value = quadrature_encoder_get_count(pio, sm);
    delta = new_value - old_value;
    old_value = new_value;

    printf("position %8d, delta %6d\n", new_value, delta);

    return delta;
}

static void update_selection(mgui_ui_group* group, int delta) {
    group->set_on_select_prev(delta < 0);
    group->set_on_select_next(delta > 0);
}

static void read_button(mgui_input_state* result) {
    result->type = mgui_input_type::Single;
    result->value_1 = !gpio_get(BUTTON_GPIO);
}

static void read_encoder(mgui_input_state* result) {
    result->type = mgui_input_type::Single;

    // set delta
    result->value_1 = update_quadrature_encoder(pio0);
}

static void clear(SSD1306* disp) {
    mgui gui(SSD1306_WIDTH, SSD1306_HEIGHT);
    disp->render(gui.lcd());
}

static void splash(SSD1306* disp) {
    mgui gui(SSD1306_WIDTH, SSD1306_HEIGHT);
    font_16x8 font;
    mgui_text text(&font, "mGUI Test", 16, 24);

    // show splash
    gui.add((mgui_object*)&text);
    gui.update_lcd();
    disp->render(gui.lcd());
    sleep_ms(5000);

    // clear display
    gui.clear();
    gui.update_lcd();
    disp->render(gui.lcd());
}

mgui test_circle() {
    mgui gui(SSD1306_WIDTH, SSD1306_HEIGHT);
    mgui_circle Circle;
    Circle.set_x(20);
    Circle.set_y(20);
    Circle.set_radius(10);
    gui.add((mgui_object*)&Circle);

    return gui;
}

static void menu_item_handler(const mgui_menu_item* sender, const mgui_input_state state[], mgui_string* current_group) {

}

static void menu_handler(mgui_menu* sender, const mgui_input_state state[], mgui_string *current_group) {
    sender->set_on_enter(state[0].value_1);

    int delta = state[1].value_1;
    sender->set_on_select_prev(delta < 0);
    sender->set_on_select_next(delta > 0);
}

static void menu_return_handler(const mgui_menu_item* sender, const mgui_input_state state[], mgui_string *current_group) {
    static int button_state = state[0].value_1;
    
    // 0 -> 1 edge
    if(button_state < state[0].value_1){
        // change gui group
        *current_group = "main";
    }

    button_state = state[0].value_1;
}

static void test_menu(mgui_multi *gui) {
    static font_16x8 font;

    static mgui_text text(&font, "Item 1");
    static mgui_menu_item item(&text);
    item.set_input_event_handler(&menu_item_handler);

    static mgui_text text2(&font, "Check");
    static mgui_menu_item item2(&text2);
    item2.set_check(false);

    static mgui_text text3(&font, "Menu");
    static mgui_menu_item item3(&text3);

    static mgui_menu item_menu3(SSD1306_WIDTH, SSD1306_HEIGHT);

    static mgui_text text3_1(&font, "Back");
    static mgui_menu_item item3_1(&text3_1);
    item3_1.set_return_menu(true);

    static mgui_text text3_2(&font, "Child 2");
    static mgui_menu_item item3_2(&text3_2);

    static mgui_menu_item item3_3;
    item_menu3.add(&item3_1);
    item_menu3.add(&item3_2);
    item_menu3.add(&item3_3);

    item3.set_menu(item_menu3.get_property());

    static mgui_text text4(&font, "1234567890abcdefghij");
    static mgui_menu_item item4(&text4);

    static mgui_menu_item item5;
    static mgui_text text5(&font, "Item 5");
    item5.set_text(&text5);

    static mgui_menu_item item6;
    static mgui_text text6(&font, "Item 6");
    item6.set_text(&text6);

    static mgui_menu_item item7;
    static mgui_text text7(&font, "Item 7");
    item7.set_text(&text7);

    static mgui_menu_item item8;
    static mgui_text text8(&font, "Return");
    item8.set_text(&text8);
    item8.set_input_event_handler(&menu_return_handler);

    static mgui_menu menu(SSD1306_WIDTH, SSD1306_HEIGHT);
    menu.set_input_event_handler(&menu_handler);
    menu.add(&item);
    menu.add(&item2);
    menu.add(&item3);
    menu.add(&item4);
    menu.add(&item5);
    menu.add(&item6);
    menu.add(&item7);
    menu.add(&item8);

    gui->add("menu", (mgui_object*)&menu);
}

static void test_text(mgui_multi *gui) {
    static font_16x8 font;

    static mgui_menu_item item8;
    static mgui_text text8(&font, "Return");
    item8.set_text(&text8);
    item8.set_input_event_handler(&menu_return_handler);

    static mgui_menu menu(SSD1306_WIDTH, SSD1306_HEIGHT);
    menu.set_input_event_handler(&menu_handler);
    menu.add(&item8);

    gui->add("text", (mgui_object*)&menu);

    static mgui_text long_text(&font, "This is long text sample.");
    long_text.set_x(0);
    long_text.set_y(16);
    long_text.set_view_height(font.font_height());
    long_text.set_view_width(SSD1306_WIDTH);
    long_text.set_move(true, 2);
    gui->add("text", (mgui_object*)&long_text);

    static mgui_text long_text2(&font, "This is long text sample.");
    long_text2.set_x(32);
    long_text2.set_y(32);
    long_text2.set_view_height(font.font_height());
    long_text2.set_view_width(SSD1306_WIDTH / 2);
    long_text2.set_move(true);
    gui->add("text", (mgui_object*)&long_text2);
}

static void button_group_handler(mgui_ui_group* sender, const mgui_input_state state[], mgui_string* current_group){
    sender->set_on_press(state[0].value_1);

    int delta = state[1].value_1;
    sender->set_on_select_prev(delta < 0);
    sender->set_on_select_next(delta > 0);
}

static void move_to_test_menu(const mgui_button* sender, const mgui_input_state state[], mgui_string* current_group){
    static int button_state = state[0].value_1;
    
    // 0 -> 1 edge
    if(button_state < state[0].value_1){
        // change gui group
        *current_group = "menu";
    }

    button_state = state[0].value_1;
}

static void move_to_test_text(const mgui_button* sender, const mgui_input_state state[], mgui_string* current_group){
    static int button_state = state[0].value_1;
    
    // 0 -> 1 edge
    if(button_state < state[0].value_1){
        // change gui group
        *current_group = "text";
    }

    button_state = state[0].value_1;
}

static void test_main(mgui_multi* gui) {
    static font_16x8 font;
    
    static mgui_button button_menu(10, 2);
    static mgui_text menu_text(&font, "menu");
    button_menu.set_text(&menu_text);
    button_menu.set_padding(4,0,4,0);
    button_menu.set_input_event_handler(&move_to_test_menu);

    static mgui_button button_status(64, 2);
    static mgui_text status_text(&font, "status");
    button_status.set_text(&status_text);
    button_status.set_padding(4,0,4,0);

    static mgui_button button_text(10, 24);
    static mgui_text text_text(&font, "texts");
    button_text.set_text(&text_text);
    button_text.set_padding(4,0,4,0);
    button_text.set_input_event_handler(&move_to_test_text);

    static mgui_ui_group group;
    group.add(&button_menu);
    group.add(&button_status);
    group.add(&button_text);
    group.set_input_event_handler(&button_group_handler);

    gui->add("main", (mgui_object*)&group);
}

int main()
{
    stdio_init_all();

    // we don't really need to keep the offset, as this program must be loaded
    // at offset 10000
    pio_add_program(pio0, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio0, sm, ENCODER_FIRST_GPIO, 10000);

    button_gpio_init();

    SSD1306 disp(SSD1306_WIDTH, SSD1306_NUM_PAGES);

    // clear screen
    clear(&disp);

    // show splash
    splash(&disp);

    mgui_multi gui(SSD1306_WIDTH, SSD1306_HEIGHT);

    // register input
    gui.input()->add(&read_button); // 0
    gui.input()->add(&read_encoder); // 1

    // register gui views
    test_menu(&gui);
    test_main(&gui);
    test_text(&gui);

    // select registered gui
    gui.select("main");

    while (true) {
        disp.render(gui.lcd());
        gui.update_lcd();
    }

    return 0;
}

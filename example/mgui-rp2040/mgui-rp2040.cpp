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

static int update_quadrature_encoder(PIO pio){
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

static void update_selection(mgui_ui_group *group, int delta) {
    group->set_on_select_prev(delta < 0);
    group->set_on_select_next(delta > 0);
}

void clear(SSD1306 *disp) {
    mgui gui(SSD1306_WIDTH, SSD1306_HEIGHT);
    disp->render(gui.lcd());
}

void splash(SSD1306 *disp) {
    mgui gui(SSD1306_WIDTH, SSD1306_HEIGHT);
    font_16x8 font;
    mgui_text text(&font, "mGUI Test", 16, 24);

    // show splash
    gui.add((mgui_object *)&text);
    gui.update_lcd();
    disp->render(gui.lcd());
    sleep_ms(5000);

    // clear display
    gui.clear();
    gui.update_lcd();
    disp->render(gui.lcd());
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

    mgui gui(SSD1306_WIDTH, SSD1306_HEIGHT);
    // mgui_circle Circle;
    // Circle.set_x(20);
    // Circle.set_y(20);
    // Circle.set_radius(10);
    // gui.add((mgui_object *)&Circle);
    font_16x8 font;

    mgui_text text(&font, "Item 1");
    mgui_menu_item item(&text);

    mgui_text text2(&font, "Check");
    mgui_menu_item item2(&text2);
    item2.set_check(false);

    mgui_text text3(&font, "Menu");
    mgui_menu_item item3(&text3);

    mgui_menu item_menu3(SSD1306_WIDTH, SSD1306_HEIGHT);

    mgui_text text3_1(&font, "Back");
    mgui_menu_item item3_1(&text3_1);
    item3_1.set_return_menu(true);

    mgui_text text3_2(&font, "Child 2");
    mgui_menu_item item3_2(&text3_2);
    
    mgui_menu_item item3_3;
    item_menu3.add(&item3_1);
    item_menu3.add(&item3_2);
    item_menu3.add(&item3_3);

    item3.set_menu(item_menu3.get_property());

    mgui_text text4(&font, "1234567890abcdefghij");
    mgui_menu_item item4(&text4);

    mgui_menu_item item5;
    mgui_text text5(&font, "Item 5");
    item5.set_text(&text5);

    mgui_menu_item item6;
    mgui_text text6(&font, "Item 6");
    item6.set_text(&text6);
    
    mgui_menu_item item7;
    mgui_text text7(&font, "Item 7");
    item7.set_text(&text7);

    mgui_menu menu(SSD1306_WIDTH, SSD1306_HEIGHT);
    menu.add(&item);
    menu.add(&item2);
    menu.add(&item3);
    menu.add(&item4);
    menu.add(&item5);
    menu.add(&item6);
    menu.add(&item7);

    gui.add((mgui_object *)&menu);
    gui.update_lcd();

    while(true){
        disp.render(gui.lcd());

        // set on encoder
        // button.set_on_press(update_quadrature_encoder(pio0) != 0);
        menu.set_on_enter(!gpio_get(BUTTON_GPIO));

        int delta = update_quadrature_encoder(pio0);
        menu.set_on_select_prev(delta < 0);
        menu.set_on_select_next(delta > 0);

        // update frame
        gui.update_lcd();
    }

// #endif
    return 0;
}

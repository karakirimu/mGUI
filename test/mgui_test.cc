#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../mGUI/mgui.h"
#include "font_16x8.h"

constexpr int WIDTH = 128;
constexpr int HEIGHT = 64;

constexpr int BUFFER_SIZE = (HEIGHT >> 3) * WIDTH;

int _byte_index(int x, int y) {
	return (y >> 3) * WIDTH + x;
}

static void debug_print(unsigned char *data) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        std::cout << "0x" << std::hex << (unsigned int)data[i] << ",";
    }
    std::cout << std::endl;
}

namespace Basic {

    TEST(String, init) {
        mgui_string str;
        EXPECT_TRUE(str == "");
    }

    TEST(String, basic) {
        mgui_string str("test");
        mgui_string str2 = "test2";

        EXPECT_TRUE(str == "test");
        EXPECT_FALSE(str == "t");

        EXPECT_EQ(str[0], 't');
        EXPECT_EQ(str[1], 'e');
        EXPECT_EQ(str[2], 's');
        EXPECT_EQ(str[3], 't');
        EXPECT_EQ(str[4], '\0');
        EXPECT_EQ(str.length(), 4);

        EXPECT_EQ(str2[0], 't');
        EXPECT_EQ(str2[1], 'e');
        EXPECT_EQ(str2[2], 's');
        EXPECT_EQ(str2[3], 't');
        EXPECT_EQ(str2[4], '2');
        EXPECT_EQ(str2[5], '\0');

        EXPECT_EQ(str2.length(), 5);

        EXPECT_FALSE(str == str2);
    }

    TEST(String, assign) {
        mgui_string str("test2");
        mgui_string copy = str;

        EXPECT_EQ(copy[0], 't');
        EXPECT_EQ(copy[1], 'e');
        EXPECT_EQ(copy[2], 's');
        EXPECT_EQ(copy[3], 't');
        EXPECT_EQ(copy[4], '2');
        EXPECT_EQ(copy[5], '\0');

        EXPECT_EQ(copy.length(), 5);

        EXPECT_TRUE(str == copy);
    }

    TEST(List, Order) {
        mgui_list<int> test;

        test.add(0);
        test.add(1);
        test.add(2);
        test.add(3);
        test.add(4);
        test.add(5);

        EXPECT_EQ(test.get(0), 0);
        EXPECT_EQ(test.get(1), 1);
        EXPECT_EQ(test.get(2), 2);
        EXPECT_EQ(test.get(3), 3);
        EXPECT_EQ(test.get(4), 4);
        EXPECT_EQ(test.get(5), 5);
    };

    TEST(List, Remove_Index) {
        mgui_list<int> test;

        test.add(0);
        test.add(1);
        test.add(2);
        test.add(3);
        test.add(4);
        test.add(5);

        EXPECT_EQ(test.get(0), 0);
        EXPECT_EQ(test.get(1), 1);
        EXPECT_EQ(test.get(2), 2);
        EXPECT_EQ(test.get(3), 3);
        EXPECT_EQ(test.get(4), 4);
        EXPECT_EQ(test.get(5), 5);

        // Remove first
        test.remove(0);
        EXPECT_EQ(test.get(0), 1);

        // Remove middle
        test.remove(2);
        EXPECT_EQ(test.get(0), 1);
        EXPECT_EQ(test.get(1), 3);

        // Remove last
        test.remove(5);
        EXPECT_EQ(test.get(0), 1);
        EXPECT_EQ(test.get(1), 3);
        EXPECT_EQ(test.get(2), 4);

        EXPECT_EQ(test.first()->obj, 1);
        EXPECT_EQ(test.last()->obj, 4);
    };

    TEST(Stack, basic) {
        mgui_stack<int> test;
        test.push(0);
        test.push(1);
        test.push(2);
        test.push(3);
        test.push(4);
        test.push(5);

        EXPECT_EQ(test.is_empty(), false);
        EXPECT_EQ(test.pop(), 5);
        EXPECT_EQ(test.pop(), 4);
        EXPECT_EQ(test.pop(), 3);
        EXPECT_EQ(test.pop(), 2);
        EXPECT_EQ(test.pop(), 1);
        EXPECT_EQ(test.pop(), 0);
        EXPECT_EQ(test.is_empty(), true);
    };

    TEST(Stack, object) {
        mgui_stack<int*> test;
        int data[] = { 0,1,2,3,4,5 };


        test.push(&data[0]);
        test.push(&data[1]);
        test.push(&data[2]);
        test.push(&data[3]);
        test.push(&data[4]);
        test.push(&data[5]);

        EXPECT_EQ(test.is_empty(), false);
        EXPECT_EQ(test.pop(), &data[5]);
        EXPECT_EQ(test.pop(), &data[4]);
        EXPECT_EQ(test.pop(), &data[3]);
        EXPECT_EQ(test.pop(), &data[2]);
        EXPECT_EQ(test.pop(), &data[1]);
        EXPECT_EQ(test.pop(), &data[0]);
        EXPECT_EQ(test.is_empty(), true);
    };

    TEST(Map, string) {
        mgui_string_map<char> test;
        test.insert("a", 'A');
        test.insert("ab", 'B');
        test.insert("abc", 'C');
        test.insert("abcd", 'D');

        EXPECT_EQ(*test.get("a"), 'A');
        EXPECT_EQ(*test.get("ab"), 'B');
        EXPECT_EQ(*test.get("abc"), 'C');
        EXPECT_EQ(*test.get("abcd"), 'D');

        test.remove("abc");

        EXPECT_EQ(*test.get("a"), 'A');
        EXPECT_EQ(*test.get("ab"), 'B');
        EXPECT_EQ(*test.get("abcd"), 'D');

        test.remove("a");

        EXPECT_EQ(*test.get("ab"), 'B');
        EXPECT_EQ(*test.get("abcd"), 'D');

        test.remove("abcd");

        EXPECT_EQ(*test.get("ab"), 'B');

        test.remove("ab");

        test.remove("a");

        // No item
        EXPECT_TRUE(test.get("a") == nullptr);
    };

}

typedef std::tuple<int, int, int> P_A3;
typedef std::tuple<int, int, int, int> P_A4;
typedef std::tuple<int, int, int, int, int> P_A5;
typedef std::tuple<int, int, int, int, int, int> P_A6;
typedef std::tuple<int, int, int, int, int, int, int> P_A7;

namespace DrawOnly {

    class DrawPixelTest :
        public testing::TestWithParam<P_A3> {};

    INSTANTIATE_TEST_SUITE_P(
        On,
        DrawPixelTest,
        testing::Values(
            P_A3{ 0, 0, 1 },
            P_A3{ 0, 1, 2 },
            P_A3{ 0, 2, 4 },
            P_A3{ 0, 3, 8 },
            P_A3{ 0, 4, 16 },
            P_A3{ 0, 5, 32 },
            P_A3{ 0, 6, 64 },
            P_A3{ 0, 7, 128 },
            P_A3{ 0, 8, 1 },
            P_A3{ 1, 0, 1 },
            P_A3{ 2, 0, 1 },
            P_A3{ 3, 0, 1 },
            P_A3{ 4, 0, 1 },
            P_A3{ 5, 0, 1 },
            P_A3{ 6, 0, 1 },
            P_A3{ 7, 0, 1 },
            P_A3{ 8, 0, 1 },
            P_A3{ 1, 1, 2 },
            P_A3{ 2, 2, 4 },
            P_A3{ 3, 3, 8 },
            P_A3{ 4, 4, 16 },
            P_A3{ 5, 5, 32 },
            P_A3{ 6, 6, 64 },
            P_A3{ 7, 7, 128 },
            P_A3{ 8, 8, 1 },
            P_A3{ 56, 56, 1 },
            P_A3{ 57, 57, 2 },
            P_A3{ 58, 58, 4 },
            P_A3{ 59, 59, 8 },
            P_A3{ 60, 60, 16 },
            P_A3{ 61, 61, 32 },
            P_A3{ 62, 62, 64 },
            P_A3{ 63, 63, 128 },
            P_A3{ 64, 0, 1 }
        )
    );

    TEST_P(DrawPixelTest, On) {
        mgui g(WIDTH, HEIGHT);
        int x = std::get<0>(GetParam());
        int y = std::get<1>(GetParam());
        int ex = std::get<2>(GetParam());

        mgui_pixel pixel;
        pixel.set_x(x);
        pixel.set_y(y);
        pixel.set_on(true);

        g.add((mgui_object*)&pixel);
        g.update_lcd();

        EXPECT_EQ(g.lcd()[_byte_index(x, y)], ex);

    };

    TEST_P(DrawPixelTest, Invert) {
        mgui g(WIDTH, HEIGHT);
        int x = std::get<0>(GetParam());
        int y = std::get<1>(GetParam());
        int ex = std::get<2>(GetParam());

        mgui_pixel pixel;
        pixel.set_x(x);
        pixel.set_y(y);
        pixel.set_invert(true);

        g.add((mgui_object*)&pixel);

        g.update_lcd();

        EXPECT_EQ(g.lcd()[_byte_index(x, y)], ex);

        pixel.set_invert(false);
        g.update_lcd();

        EXPECT_EQ(g.lcd()[_byte_index(x, y)], 0);

        // debug_print(g.lcd()); // OK
    };

    class DrawRectangleRoundedTest :
        public testing::TestWithParam<P_A5> {};

    TEST_P(DrawRectangleRoundedTest, On) {
        mgui g(WIDTH, HEIGHT);

        int x0 = std::get<0>(GetParam());
        int y0 = std::get<1>(GetParam());
        int x1 = std::get<2>(GetParam());
        int y1 = std::get<3>(GetParam());
        int r = std::get<4>(GetParam());

        mgui_rectangle rectangle;

        rectangle.set_x(x0);
        rectangle.set_y(y0);
        rectangle.set_width(x1 - x0);
        rectangle.set_height(y1 - y0);
        rectangle.set_radius(r);

        g.add((mgui_object*)&rectangle);
        g.update_lcd();
        // debug_print(g.lcd()); // OK
    };

    TEST_P(DrawRectangleRoundedTest, Fill) {
        mgui g(WIDTH, HEIGHT);
        int x0 = std::get<0>(GetParam());
        int y0 = std::get<1>(GetParam());
        int x1 = std::get<2>(GetParam());
        int y1 = std::get<3>(GetParam());
        int r = std::get<4>(GetParam());

        mgui_rectangle rectangle;

        rectangle.set_x(x0);
        rectangle.set_y(y0);
        rectangle.set_width(x1 - x0);
        rectangle.set_height(y1 - y0);
        rectangle.set_radius(r);
        rectangle.set_fill(true);

        g.add((mgui_object*)&rectangle);
        g.update_lcd();

        // debug_print(g.lcd()); // OK
    };

    INSTANTIATE_TEST_SUITE_P(
        On,
        DrawRectangleRoundedTest,
        testing::Values(
            P_A5{ 2, 2, 12, 12, 2 },
            P_A5{ 0, 0, 31, 31, 8 },
            P_A5{ 0, 48, 127, 63, 2 }
        )
    );

    class DrawCircleTest :
        public testing::TestWithParam<P_A3> {};

    TEST_P(DrawCircleTest, On) {
        mgui g(WIDTH, HEIGHT);
        int x0 = std::get<0>(GetParam());
        int y0 = std::get<1>(GetParam());
        int r = std::get<2>(GetParam());

        mgui_circle circle;
        circle.set_x(x0);
        circle.set_y(y0);
        circle.set_radius(r);

        g.add((mgui_object*)&circle);

        g.update_lcd();
        // debug_print(g.lcd()); // OK
    };

    TEST_P(DrawCircleTest, Fill) {
        mgui g(WIDTH, HEIGHT);
        int x0 = std::get<0>(GetParam());
        int y0 = std::get<1>(GetParam());
        int r = std::get<2>(GetParam());

        mgui_circle circle;
        circle.set_x(x0);
        circle.set_y(y0);
        circle.set_radius(r);
        circle.set_fill(true);

        g.add((mgui_object*)&circle);

        g.update_lcd();
        // debug_print(g.lcd()); // OK
    };

    INSTANTIATE_TEST_SUITE_P(
        On,
        DrawCircleTest,
        testing::Values(
            P_A3{ 8, 8, 6 },
            P_A3{ 32, 32, 20 }
        )
    );

    class DrawRectangleTest :
        public testing::TestWithParam<P_A5> {};

    TEST_P(DrawRectangleTest, On) {
        mgui g(WIDTH, HEIGHT);
        int x0 = std::get<0>(GetParam());
        int y0 = std::get<1>(GetParam());
        int x1 = std::get<2>(GetParam());
        int y1 = std::get<3>(GetParam());
        bool fill = std::get<4>(GetParam());

        mgui_rectangle rectangle;

        rectangle.set_x(x0);
        rectangle.set_y(y0);
        rectangle.set_width(x1 - x0);
        rectangle.set_height(y1 - y0);
        rectangle.set_fill(fill);

        g.add((mgui_object*)&rectangle);
        g.update_lcd();
        // debug_print(g.lcd()); // OK
    };

    INSTANTIATE_TEST_SUITE_P(
        On,
        DrawRectangleTest,
        testing::Values(
            P_A5{ 0, 0, 15, 15, true },
            P_A5{ 4, 4, 24, 24, false }
        )
    );

    class DrawTriangleTest :
        public testing::TestWithParam<P_A7> {};

    TEST_P(DrawTriangleTest, On) {
        mgui g(WIDTH, HEIGHT);
        int x0 = std::get<0>(GetParam());
        int y0 = std::get<1>(GetParam());
        int x1 = std::get<2>(GetParam());
        int y1 = std::get<3>(GetParam());
        int x2 = std::get<4>(GetParam());
        int y2 = std::get<5>(GetParam());
        bool invert = std::get<6>(GetParam());

        mgui_triangle triangle;

        triangle.set_x0(x0);
        triangle.set_y0(y0);
        triangle.set_x1(x1);
        triangle.set_y1(y1);
        triangle.set_x2(x2);
        triangle.set_y2(y2);
        triangle.set_invert(invert);

        g.add((mgui_object*)&triangle);
        g.update_lcd();
        //debug_print(g.lcd()); // OK
    };

    INSTANTIATE_TEST_SUITE_P(
        On,
        DrawTriangleTest,
        testing::Values(
            P_A7{ 0, 0, 9, 21, 21, 21, false },
            P_A7{ 4, 4, 24, 24, 16, 16, false },
            P_A7{ 16, 16, 8, 32, 24, 32, false }
        )
    );

    class DrawTextTest :
        public testing::TestWithParam<std::tuple<int, int, std::string>> {};

    TEST_P(DrawTextTest, On) {
        mgui g(WIDTH, HEIGHT);
        int x0 = std::get<0>(GetParam());
        int y0 = std::get<1>(GetParam());
        std::string textdata = std::get<2>(GetParam());

        font_16x8 prop;
        mgui_text text(&prop);

        text.set_x(x0);
        text.set_y(y0);
        text.set_text(textdata.c_str());

        g.add((mgui_object*)&text);
        g.update_lcd();
        // debug_print(g.lcd()); // OK
    };

    INSTANTIATE_TEST_SUITE_P(
        On,
        DrawTextTest,
        testing::Values(
            std::tuple<int, int, std::string>{ 0, 0, "Hello" },
            std::tuple<int, int, std::string>{ 10, 20, "World" },
            std::tuple<int, int, std::string>{ 1, 48, "World" },
            // Height is greater than screen_height 
            std::tuple<int, int, std::string>{ 1, 49, "x >" },
            // Width is greater than screen_width
            std::tuple<int, int, std::string>{ 1, 49, "123456789abcdef" }
    )
    );
}

namespace UserInterface {

    class ButtonTest :
        public testing::TestWithParam<std::tuple<int, int, std::string>> {};

    TEST_P(ButtonTest, On) {
        mgui g(WIDTH, HEIGHT);
        int x0 = std::get<0>(GetParam());
        int y0 = std::get<1>(GetParam());
        std::string textdata = std::get<2>(GetParam());

        font_16x8 prop;
        mgui_text text(&prop, textdata.c_str());
        mgui_button button(x0, y0);
        button.set_text(&text, 1, 2);
        button.set_radius(2);
        button.set_on_press(true);

        g.add((mgui_object*)&button);
        g.update_lcd();
        //debug_print(g.lcd()); // OK
    };

    TEST_P(ButtonTest, Off) {
        mgui g(WIDTH, HEIGHT);
        int x0 = std::get<0>(GetParam());
        int y0 = std::get<1>(GetParam());
        std::string textdata = std::get<2>(GetParam());

        font_16x8 prop;
        mgui_text text(&prop, textdata.c_str());
        mgui_button button(x0, y0);
        button.set_text(&text, 1, 2);
        button.set_radius(2);
        button.set_on_press(false);

        g.add((mgui_object*)&button);
        g.update_lcd();
        //debug_print(g.lcd()); // OK
    };

    INSTANTIATE_TEST_SUITE_P(
        On,
        ButtonTest,
        testing::Values(
            std::tuple<int, int, std::string>{ 0, 0, "Hello" },
            std::tuple<int, int, std::string>{ 5, 5, "Hello" },
            std::tuple<int, int, std::string>{ 10, 20, "W" }
    )
    );

    TEST(ButtonTest_NoText, On) {
        mgui g(WIDTH, HEIGHT);

        mgui_button button(40, 15);
        button.set_height(20);
        button.set_width(30);
        button.set_on_press(true);

        g.add((mgui_object*)&button);
        g.update_lcd();
        // debug_print(g.lcd()); // OK
    };

    TEST(ButtonTest_NoText, Off) {
        mgui g(WIDTH, HEIGHT);

        mgui_button button(40, 15);
        button.set_height(20);
        button.set_width(30);
        button.set_on_press(false);

        g.add((mgui_object*)&button);
        g.update_lcd();
        // debug_print(g.lcd()); // OK
    };

    TEST(ButtonTest_NoText, Multi_On) {
        mgui g(WIDTH, HEIGHT);

        mgui_button button(10, 10);
        button.set_height(10);
        button.set_width(10);
        button.set_on_press(true);
        g.add((mgui_object*)&button);

        mgui_button button2(20, 20);
        button2.set_height(10);
        button2.set_width(10);
        button2.set_on_press(true);
        g.add((mgui_object*)&button2);

        mgui_button button3(30, 30);
        button3.set_height(10);
        button3.set_width(10);
        button3.set_on_press(true);
        g.add((mgui_object*)&button3);

        g.update_lcd();
        //debug_print(g.lcd()); // OK
    };

    class MenuItemTest :
        public testing::TestWithParam<std::tuple<int, int, std::string>> {};

    TEST_P(MenuItemTest, Selected) {
        mgui g(WIDTH, HEIGHT);
        int index = std::get<0>(GetParam());
        int count = std::get<1>(GetParam());
        std::string textdata = std::get<2>(GetParam());

        font_16x8 prop;
        mgui_text text(&prop, textdata.c_str());
        mgui_menu_item menu_item;
        menu_item._set_draw_position(index, count, WIDTH, HEIGHT);
        menu_item.set_text(&text, 1, 2);
        menu_item.set_radius(2);
        menu_item.set_on_selected(true);

        g.add((mgui_object*)&menu_item);
        g.update_lcd();
        //debug_print(g.lcd()); // OK
    };

    INSTANTIATE_TEST_SUITE_P(
        Selected,
        MenuItemTest,
        testing::Values(
            std::tuple<int, int, std::string>{ 0, 4, "Hello" },
            std::tuple<int, int, std::string>{ 1, 4, "012345678" },
            std::tuple<int, int, std::string>{ 2, 4, "W" },
            std::tuple<int, int, std::string>{ 3, 4, "x >" }
    )
    );

    TEST(UiGroupTest, FourItem) {
        mgui g(WIDTH, HEIGHT);
        mgui_button button(10, 10);
        button.set_height(10);
        button.set_width(10);

        mgui_button button2(20, 20);
        button2.set_height(10);
        button2.set_width(10);

        mgui_button button3(30, 30);
        button3.set_height(10);
        button3.set_width(10);

        mgui_button button4(40, 40);
        button4.set_height(10);
        button4.set_width(10);

        mgui_ui_group group;
        group.add(&button);
        group.add(&button2);
        group.add(&button3);
        group.add(&button4);

        g.add((mgui_object*)&group);
        g.update_lcd();
        // debug_print(g.lcd()); // OK
    }

    TEST(UiGroupTest, FourItem_2) {
        mgui g(WIDTH, HEIGHT);
        mgui_button button(10, 10, 10, 10);
        mgui_button button2(20, 20, 10, 10);
        mgui_button button3(30, 30, 10, 10);
        mgui_button button4(40, 40, 10, 10);
        mgui_ui_group group;
        group.add(&button);
        group.add(&button2);
        group.add(&button3);
        group.add(&button4);

        g.add((mgui_object*)&group);
        g.update_lcd();
        // debug_print(g.lcd()); // OK
    }

    TEST(MenuTest, Nested) {
        mgui g(WIDTH, HEIGHT);

        mgui_menu_item item;

        mgui_menu_item item2;
        mgui_menu item2_menu(WIDTH, HEIGHT);
        mgui_menu_item item2_item;
        item2_menu.add(&item2_item);
        item2.set_menu(item2_menu.get_property());

        mgui_menu_item item3;
        mgui_menu_item item4;
        mgui_menu_item item5;

        mgui_menu menu(WIDTH, HEIGHT);
        menu.add(&item);
        menu.add(&item2);
        menu.add(&item3);
        menu.add(&item4);
        menu.add(&item5);

        g.add((mgui_object*)&menu);
        g.update_lcd();
        // debug_print(g.lcd()); // OK
    }

    TEST(MenuTest, AddressMove) {
        mgui g(WIDTH, HEIGHT);

        mgui_menu_item item;

        mgui_menu_item item2;
        mgui_menu item2_menu(WIDTH, HEIGHT);
        mgui_menu_item item2_item;
        item2_menu.add(&item2_item);
        item2.set_menu(item2_menu.get_property());

        mgui_menu_item item3;
        mgui_menu_item item4;
        mgui_menu_item item5;

        mgui_menu menu(WIDTH, HEIGHT);
        menu.add(&item);
        menu.add(&item2);
        menu.add(&item3);
        menu.add(&item4);
        menu.add(&item5);

        g.add((mgui_object*)&menu);
        g.update_lcd();

        // select item2
        menu.set_selected_index(1);
        EXPECT_EQ(menu.selected_index(), 1);

        // move current menu
        menu.set_on_enter(true);
        EXPECT_EQ(menu.selected_index(), 0);

        // revert menu
        menu.set_on_return(true);
        EXPECT_EQ(menu.selected_index(), 1);
    }

    /**
     * @brief
     * menu
     * + item_menu
     * ++ item_menu2
     *  ++ item_menu3
     *   ++ item_menu4
     *    ++ item_menu5
     *     ++ item5
     */
    TEST(MenuTest, MenuStack) {
        mgui g(WIDTH, HEIGHT);

        mgui_menu_item item;
        mgui_menu_item item2;
        mgui_menu_item item3;
        mgui_menu_item item4;
        mgui_menu_item item5;

        mgui_menu item_menu2(WIDTH, HEIGHT);
        mgui_menu item_menu3(WIDTH, HEIGHT);
        mgui_menu item_menu4(WIDTH, HEIGHT);
        mgui_menu item_menu5(WIDTH, HEIGHT);

        item_menu5.add(&item5);

        item4.set_menu(item_menu5.get_property());
        item_menu4.add(&item4);

        item3.set_menu(item_menu4.get_property());
        item_menu3.add(&item3);

        item2.set_menu(item_menu3.get_property());
        item_menu2.add(&item2);

        item.set_menu(item_menu2.get_property());

        mgui_menu menu(WIDTH, HEIGHT);
        menu.add(&item);

        g.add((mgui_object*)&menu);
        g.update_lcd();

        EXPECT_EQ(menu.get_selected_item(), &item);

        // enter menu
        menu.set_on_enter(true);
        menu.set_on_enter(false);
        EXPECT_EQ(menu.get_selected_item(), &item2);

        // enter menu
        menu.set_on_enter(true);
        menu.set_on_enter(false);
        EXPECT_EQ(menu.get_selected_item(), &item3);

        // enter menu
        menu.set_on_enter(true);
        menu.set_on_enter(false);
        EXPECT_EQ(menu.get_selected_item(), &item4);

        // enter menu
        menu.set_on_enter(true);
        menu.set_on_enter(false);
        EXPECT_EQ(menu.get_selected_item(), &item5);

        // revert menu
        menu.set_on_return(true);
        menu.set_on_return(false);
        EXPECT_EQ(menu.get_selected_item(), &item4);

        // revert menu
        menu.set_on_return(true);
        menu.set_on_return(false);
        EXPECT_EQ(menu.get_selected_item(), &item3);

        // revert menu
        menu.set_on_return(true);
        menu.set_on_return(false);
        EXPECT_EQ(menu.get_selected_item(), &item2);

        // revert menu
        menu.set_on_return(true);
        menu.set_on_return(false);
        EXPECT_EQ(menu.get_selected_item(), &item);
    }

    TEST(MenuTest, Check_Menu) {
        mgui g(WIDTH, HEIGHT);

        mgui_menu_item item;
        mgui_menu_item item2;
        item2.set_check(false);
        mgui_menu_item item3;
        mgui_menu_item item4;
        mgui_menu_item item5;

        mgui_menu menu(WIDTH, HEIGHT);
        menu.add(&item);
        menu.add(&item2);
        menu.add(&item3);
        menu.add(&item4);
        menu.add(&item5);

        g.add((mgui_object*)&menu);
        g.update_lcd();

        // select item2
        menu.set_selected_index(1);
        EXPECT_EQ(menu.selected_index(), 1);
        EXPECT_EQ(item2.checked(), false);
        g.update_lcd();

        // check item2
        menu.set_on_enter(true);
        g.update_lcd();
        EXPECT_EQ(item2.checked(), true);

        menu.set_on_enter(false);
        g.update_lcd();

        // uncheck item2
        menu.set_on_enter(true);
        g.update_lcd();
        EXPECT_EQ(item2.checked(), false);

        g.update_lcd();
        //debug_print(g.lcd()); // OK
    }

    TEST(MenuTest, Show_Only) {
        mgui g(WIDTH, HEIGHT);

        mgui_menu_item item;
        mgui_menu_item item2;
        item2.set_check(false);
        mgui_menu_item item3;
        mgui_menu_item item4;
        mgui_menu_item item5;

        mgui_menu menu(WIDTH, HEIGHT);
        menu.add(&item);
        menu.add(&item2);
        menu.add(&item3);
        menu.add(&item4);
        menu.add(&item5);

        g.add((mgui_object*)&menu);
        g.update_lcd();

        //debug_print(g.lcd()); // OK
    }

    TEST(MultiGuiTest, Register) {
        mgui_multi g(WIDTH, HEIGHT);

        mgui_menu_item item;

        mgui_menu_item item2;
        mgui_menu item2_menu(WIDTH, HEIGHT);
        mgui_menu_item item2_item;
        item2_menu.add(&item2_item);
        item2.set_menu(item2_menu.get_property());

        mgui_menu_item item3;
        mgui_menu_item item4;
        mgui_menu_item item5;

        mgui_menu menu(WIDTH, HEIGHT);
        menu.add(&item);
        menu.add(&item2);
        menu.add(&item3);
        menu.add(&item4);
        menu.add(&item5);

        g.add("menu", (mgui_object*)&menu);

        mgui_button button(10, 10, 10, 10);
        mgui_button button2(20, 20, 10, 10);
        mgui_button button3(30, 30, 10, 10);
        mgui_button button4(40, 40, 10, 10);
        mgui_ui_group group;
        group.add(&button);
        group.add(&button2);
        group.add(&button3);
        group.add(&button4);

        g.add("buttons", (mgui_object*)&group);
    
        g.update_lcd();
        // debug_print(g.lcd()); // OK
    }
}
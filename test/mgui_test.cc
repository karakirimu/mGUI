#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../mGUI/mgui.h"
#include "font_16x8.h"
#include <iostream>

constexpr int WIDTH = 128;
constexpr int HEIGHT = 64;

constexpr int BUFFER_SIZE = (HEIGHT >> 3) * WIDTH;

int _byte_index(int x, int y) {
	return (y >> 3) * WIDTH + x;
}

void debug_print(uint8_t *data) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        std::cout << "0x" << std::hex << (unsigned int)data[i] << ",";
    }
    std::cout << std::endl;
}

typedef std::tuple<int, int, int> P_A3;
typedef std::tuple<int, int, int, int> P_A4;
typedef std::tuple<int, int, int, int, int> P_A5;

class DrawPixelTest :
    public testing::TestWithParam<P_A3> {};

TEST_P(DrawPixelTest, On) {
  mgui g(WIDTH, HEIGHT);
  int x = std::get<0>(GetParam());
  int y = std::get<1>(GetParam());
  int ex = std::get<2>(GetParam());

  mgui_pixel pixel;
  pixel.set_x(x);
  pixel.set_y(y);
  pixel.set_on(true);

  g.add((mgui_object *)&pixel);
  g.display();

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

  g.add((mgui_object *)&pixel);

  g.display();

  EXPECT_EQ(g.lcd()[_byte_index(x, y)], ex);

  g.display();

  EXPECT_EQ(g.lcd()[_byte_index(x, y)], 0);

  // debug_print(g.lcd()); // OK
};

INSTANTIATE_TEST_SUITE_P(
  On,
  DrawPixelTest,
  testing::Values(
    P_A3{0, 0, 128},
    P_A3{0, 1, 64},
    P_A3{0, 2, 32},
    P_A3{0, 3, 16},
    P_A3{0, 4, 8},
    P_A3{0, 5, 4},
    P_A3{0, 6, 2},
    P_A3{0, 7, 1},
    P_A3{0, 8, 128},
    P_A3{1, 0, 128},
    P_A3{2, 0, 128},
    P_A3{3, 0, 128},
    P_A3{4, 0, 128},
    P_A3{5, 0, 128},
    P_A3{6, 0, 128},
    P_A3{7, 0, 128},
    P_A3{8, 0, 128},
    P_A3{1, 1, 64},
    P_A3{2, 2, 32},
    P_A3{3, 3, 16},
    P_A3{4, 4, 8},
    P_A3{5, 5, 4},
    P_A3{6, 6, 2},
    P_A3{7, 7, 1},
    P_A3{8, 8, 128},
    P_A3{56, 56, 128},
    P_A3{57, 57, 64},
    P_A3{58, 58, 32},
    P_A3{59, 59, 16},
    P_A3{60, 60, 8},
    P_A3{61, 61, 4},
    P_A3{62, 62, 2},
    P_A3{63, 63, 1},
    P_A3{64, 0, 128}
  )
);

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
  g.display();
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
  g.display();

  // debug_print(g.lcd()); // OK
};

INSTANTIATE_TEST_SUITE_P(
  On,
  DrawRectangleRoundedTest,
  testing::Values(
    P_A5{2, 2, 12, 12, 2},
    P_A5{0, 0, 31, 31, 8}
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

    g.add((mgui_object *)&circle);

    g.display();
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

    g.add((mgui_object *)&circle);

    g.display();
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
    g.display();
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

class DrawTextTest :
    public testing::TestWithParam<std::tuple<int, int, std::string>> {};

TEST_P(DrawTextTest, On) {
    mgui g(WIDTH, HEIGHT);
    int x0 = std::get<0>(GetParam());
    int y0 = std::get<1>(GetParam());
    std::string textdata = std::get<2>(GetParam());

    font_16x8 prop;
    mgui_text text;

    text.set_x(x0);
    text.set_y(y0);
    text.set_font(&prop);
    text.set_text(textdata.c_str());

    g.add((mgui_object *)&text);
    g.display();
    // debug_print(g.lcd()); // OK
};

INSTANTIATE_TEST_SUITE_P(
    On,
    DrawTextTest,
    testing::Values(
        std::tuple<int, int, std::string>{ 0, 0, "Hello" },
        std::tuple<int, int, std::string>{ 10, 20, "World" }
    )
);

class ButtonTest :
    public testing::TestWithParam<std::tuple<int, int, std::string>> {};

static bool onFunc() {
    return true;
};

TEST_P(ButtonTest, On) {
    mgui g(WIDTH, HEIGHT);
    int x0 = std::get<0>(GetParam());
    int y0 = std::get<1>(GetParam());
    std::string textdata = std::get<2>(GetParam());

    font_16x8 prop;
    mgui_button button(x0, y0);
    button.set_text(textdata.c_str(), &prop, 1, 2);
    button.set_radius(2);
    button.set_on_press(&onFunc);
    
    g.add((mgui_object*)&button);
    g.display();
    // debug_print(g.lcd()); // OK
};

static bool offFunc() {
    return false;
};

TEST_P(ButtonTest, Off) {
    mgui g(WIDTH, HEIGHT);
    int x0 = std::get<0>(GetParam());
    int y0 = std::get<1>(GetParam());
    std::string textdata = std::get<2>(GetParam());

    font_16x8 prop;
    mgui_button button(x0, y0);
    button.set_text(textdata.c_str(), &prop, 1, 2);
    button.set_radius(2);
    button.set_on_press(&offFunc);

    g.add((mgui_object*)&button);
    g.display();
    // debug_print(g.lcd()); // OK
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

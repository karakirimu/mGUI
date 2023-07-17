#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../mGUI/mgui.h"
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
  mgui_draw g(WIDTH, HEIGHT);
  int x = std::get<0>(GetParam());
  int y = std::get<1>(GetParam());
  int ex = std::get<2>(GetParam());

  g.draw_pixel(x, y, true);

  EXPECT_EQ(g.lcd()[_byte_index(x, y)], ex);

  // debug_print(g.lcd());
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
  mgui_draw g(WIDTH, HEIGHT);
  int x0 = std::get<0>(GetParam());
  int y0 = std::get<1>(GetParam());
  int x1 = std::get<2>(GetParam());
  int y1 = std::get<3>(GetParam());
  int r = std::get<4>(GetParam());

  g.draw_rectangle_rounded(x0, y0, x1, y1, r);
  // debug_print(g.lcd()); // OK
};

TEST_P(DrawRectangleRoundedTest, Fill) {
  mgui_draw g(WIDTH, HEIGHT);
  int x0 = std::get<0>(GetParam());
  int y0 = std::get<1>(GetParam());
  int x1 = std::get<2>(GetParam());
  int y1 = std::get<3>(GetParam());
  int r = std::get<4>(GetParam());

  g.draw_rectangle_rounded(x0, y0, x1, y1, r, true);
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
    mgui_draw g(WIDTH, HEIGHT);
    int x0 = std::get<0>(GetParam());
    int y0 = std::get<1>(GetParam());
    int r = std::get<2>(GetParam());

    g.draw_circle(x0, y0, r);
    // debug_print(g.lcd()); // OK
};

TEST_P(DrawCircleTest, Fill) {
    mgui_draw g(WIDTH, HEIGHT);
    int x0 = std::get<0>(GetParam());
    int y0 = std::get<1>(GetParam());
    int r = std::get<2>(GetParam());

    g.draw_circle(x0, y0, r, true);
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
    mgui_draw g(WIDTH, HEIGHT);
    int x0 = std::get<0>(GetParam());
    int y0 = std::get<1>(GetParam());
    int x1 = std::get<2>(GetParam());
    int y1 = std::get<3>(GetParam());
    bool fill = std::get<4>(GetParam());

    g.draw_rectangle(x0, y0, x1, y1, fill);
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
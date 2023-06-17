#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../mGUI/mgui.h"

constexpr int WIDTH = 128;
constexpr int HEIGHT = 64;

int _byte_index(int x, int y) {
	return (y >> 3) * WIDTH + x;
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

  g.draw_pixel(x, y, true);

  EXPECT_EQ(g.lcd()[_byte_index(x, y)], ex);
};

INSTANTIATE_TEST_SUITE_P(
  On,
  DrawPixelTest,
  testing::Values(
    P_A3{0, 0, 1},
    P_A3{0, 1, 2},
    P_A3{0, 2, 4},
    P_A3{0, 3, 8},
    P_A3{0, 4, 16},
    P_A3{0, 5, 32},
    P_A3{0, 6, 64},
    P_A3{0, 7, 128},
    P_A3{0, 8, 1},
    P_A3{1, 0, 1},
    P_A3{2, 0, 1},
    P_A3{3, 0, 1},
    P_A3{4, 0, 1},
    P_A3{5, 0, 1},
    P_A3{6, 0, 1},
    P_A3{7, 0, 1},
    P_A3{8, 0, 1}
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

  g.draw_rectangle_rounded(x0, y0, x1, y1, r);

  EXPECT_EQ(g.lcd()[_byte_index(x, y)], 0);
};

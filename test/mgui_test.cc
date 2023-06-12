#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../mGUI/mgui.h"

constexpr int WIDTH = 128;
constexpr int HEIGHT = 64;

int _byte_index(int x, int y) {
	return (y >> 3) * WIDTH + x;
}

typedef std::tuple<int, int, int> P_POS;

class DrawPixelTest :
    public testing::TestWithParam<P_POS> {};

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
    P_POS{0, 0, 1},
    P_POS{0, 1, 2},
    P_POS{0, 2, 4},
    P_POS{0, 3, 8},
    P_POS{0, 4, 16},
    P_POS{0, 5, 32},
    P_POS{0, 6, 64},
    P_POS{0, 7, 128},
    P_POS{0, 8, 1},
    P_POS{1, 0, 1},
    P_POS{2, 0, 1},
    P_POS{3, 0, 1},
    P_POS{4, 0, 1},
    P_POS{5, 0, 1},
    P_POS{6, 0, 1},
    P_POS{7, 0, 1},
    P_POS{8, 0, 1}
  )
);

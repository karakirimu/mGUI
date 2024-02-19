/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

/* Example code to talk to an SSD1306-based OLED display

   The SSD1306 is an OLED/PLED driver chip, capable of driving displays up to
   128x64 pixels.

   NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefore I2C) cannot be used at 5v.

   You will need to use a level shifter on the I2C lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on display
   board
   GPIO PICO_DEFAULT_I2C_SCL_PIN (on Pico this is GP5 (pin 7)) -> SCL on
   display board
   3.3v (pin 36) -> VCC on display board
   GND (pin 38)  -> GND on display board
*/

namespace Commands {
    #define SSD1306_I2C_ADDR            _u(0x3C)

    // commands (see datasheet)
    #define SSD1306_SET_MEM_MODE        _u(0x20)
    #define SSD1306_SET_COL_ADDR        _u(0x21)
    #define SSD1306_SET_PAGE_ADDR       _u(0x22)
    #define SSD1306_SET_HORIZ_SCROLL    _u(0x26)
    #define SSD1306_SET_SCROLL          _u(0x2E)

    #define SSD1306_SET_DISP_START_LINE _u(0x40)

    #define SSD1306_SET_CONTRAST        _u(0x81)
    #define SSD1306_SET_CHARGE_PUMP     _u(0x8D)

    #define SSD1306_SET_SEG_REMAP       _u(0xA0)
    #define SSD1306_SET_ENTIRE_ON       _u(0xA4)
    #define SSD1306_SET_ALL_ON          _u(0xA5)
    #define SSD1306_SET_NORM_DISP       _u(0xA6)
    #define SSD1306_SET_INV_DISP        _u(0xA7)
    #define SSD1306_SET_MUX_RATIO       _u(0xA8)
    #define SSD1306_SET_DISP            _u(0xAE)
    #define SSD1306_SET_COM_OUT_DIR     _u(0xC0)
    #define SSD1306_SET_COM_OUT_DIR_FLIP _u(0xC0)

    #define SSD1306_SET_DISP_OFFSET     _u(0xD3)
    #define SSD1306_SET_DISP_CLK_DIV    _u(0xD5)
    #define SSD1306_SET_PRECHARGE       _u(0xD9)
    #define SSD1306_SET_COM_PIN_CFG     _u(0xDA)
    #define SSD1306_SET_VCOM_DESEL      _u(0xDB)

    #define SSD1306_WRITE_MODE         _u(0xFE)
    #define SSD1306_READ_MODE          _u(0xFF)
}

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 20
#define I2C_SCL 21

// 400 is usual, but often these can be overclocked to improve display response.
// Tested at 1000 on both 32 and 84 pixel height devices and it worked.
//#define SSD1306_I2C_CLK             400
#define SSD1306_I2C_CLK             1000

// Define the size of the display we have attached. This can vary, make sure you
// have the right size defined or the output will look rather odd!
// Code has been tested on 128x32 and 128x64 OLED displays
#define SSD1306_HEIGHT              64
#define SSD1306_WIDTH               128

#define SSD1306_PAGE_HEIGHT         _u(8)
#define SSD1306_NUM_PAGES           (SSD1306_HEIGHT / SSD1306_PAGE_HEIGHT)
#define SSD1306_BUF_LEN             (SSD1306_NUM_PAGES * SSD1306_WIDTH)

struct render_area {
    uint8_t start_col;
    uint8_t end_col;
    uint8_t start_page;
    uint8_t end_page;

    int buflen;
};

class SSD1306 {
public:
    SSD1306();
    ~SSD1306(){}

    // inline uint8_t *GetBuffer() {return draw_buf;}
    void init();
    void scroll(bool on);
    void invert_display(bool on);
    // void renderAll();
    void render(uint8_t *buf, struct render_area *area);

    void SSD1306_send_cmd(uint8_t cmd);

    void calc_render_area_buflen(struct render_area *area);

private:
    // void FillReversedCache();
    uint8_t reverse(uint8_t b);

    void SSD1306_send_cmd_list(uint8_t *buf, int num);
    void SSD1306_send_buf(uint8_t buf[], int buflen);

    // uint8_t draw_buf[SSD1306_BUF_LEN];
    // struct render_area frame_area;
    // uint8_t reversed[sizeof(font)];
};
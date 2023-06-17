/**
 * @file mgui.h
 * @author karakirimu
 * @brief Simple monochrome GUI operation program
 * @version 0.1
 * @date 2023-06-10
 * 
 * @copyright Copyright (c) 2023 karakirimu
 * 
 */

// typedefs
typedef unsigned char uint8_t;

// enums
enum mgui_line_dir {
    Left,
    Down
};

class mgui {
public:
    mgui(const uint8_t width, const uint8_t height){
        lcd_width = width;
        lcd_height = height;
        int size = width * (height >> 3);
        lcd_buffer = new uint8_t[size];
        memset(lcd_buffer, 0, size);
    }
    ~mgui(){
        delete[] lcd_buffer;
    }

    void draw_circle(int x0, int y0, int r) {
        int x = r;
        int y = 0;
        int f = -(r << 1) + 3;

        while (x >= y)
        {
            draw_pixel(x0 + x, y0 + y, true);
            draw_pixel(x0 - x, y0 + y, true);
            draw_pixel(x0 + x, y0 - y, true);
            draw_pixel(x0 - x, y0 - y, true);
            draw_pixel(x0 + y, y0 + x, true);
            draw_pixel(x0 - y, y0 + x, true);
            draw_pixel(x0 + y, y0 - x, true);
            draw_pixel(x0 - y, y0 - x, true);
            if (f >= 0)
            {
                x--;
                f -= (x << 2);
            }
            y++;
            f += (y << 2) + 2;
        }
    }

    void draw_rectangle_rounded(int x0, int y0, int x1, int y1, int r){
        int x = r;
        int y = 0;
        int f = -(r << 1) + 3;

        int px0 = x0 + r;
        int px1 = x1 - r;
        int py0 = y0 + r;
        int py1 = y1 - r;

        while (x >= y)
        {
            // 1
            draw_pixel(px1 + x, py0 + y, true);
            draw_pixel(px1 - x, py0 - y, true);
            
            // 2
            draw_pixel(px1 - x, py1 + y, true);
            draw_pixel(px1 + x, py1 - y, true);
            
            // 3
            draw_pixel(px0 + y, py1 + x, true);
            draw_pixel(px0 - y, py1 - x, true);

            // 4
            draw_pixel(px0 - y, py0 + x, true);
            draw_pixel(px0 + y, py0 - x, true);
            if (f >= 0)
            {
                x--;
                f -= (x << 2);
            }
            y++;
            f += (y << 2) + 2;
        }

        for (int x = px0; x <=px1 ; x++) {
            draw_pixel(x, y0, true);
            draw_pixel(x, y1, true);
        }

        for(int y = py0; y <= py1; y++) {
            draw_pixel(x0, y, true);
            draw_pixel(x1, y, true);
        }
    }

    void draw_rectangle(int x0, int y0, int x1, int y1, bool fill = false){
        if(fill){
            for(int x = x0; x <= x1; x++){
                for(int y = y0; y <= y1; y++){
                    draw_pixel(x, y, true);
                }
            }
            return;
        }

        for (int x = x0; x <=x1 ; x++) {
            draw_pixel(x, y0, true);
            draw_pixel(x, y1, true);
        }

        for(int y = y0; y <= y1; y++) {
            draw_pixel(x0, y, true);
            draw_pixel(x1, y, true);
        }
    }

    void draw_line(int x0, int y0, int x1, int y1, bool on){
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int dx = sx * (x1 - x0);
        int dy = -sy * (y1 - y0);
        int err = dx + dy;
        int e2;

        while (x0 != x1 || y0 != y1)
        {
            draw_pixel(x0, y0, on);

            e2 = 2 * err;

            if (e2 >= dy)
            {
                err += dy;
                x0 += sx;
            } else {
                err += dx;
                y0 += sy;
            }
        }
    }

    void draw_line_straight(int x0, int y0, int length, bool on, mgui_line_dir direction) {

        if (direction == mgui_line_dir::Left) {
            int xlast = x0 + length;
            for (int x = x0; x < xlast; x++) {
                draw_pixel(x, y0, on);
            }
            return;
        }

        int ylast = y0 + length;
        for (int y = y0; y < ylast; y++) {
            draw_pixel(x0, y, on);
        }
    }

    void draw_pixel(int x, int y, bool on){
        // Calculate the byte index.
        int byte_idx = (y >> 3) * lcd_width + x;
        uint8_t bit_idx = (1 << (y & 7));

        // Set or clear the bit at the specified index.
        lcd_buffer[byte_idx] 
            = on ? (lcd_buffer[byte_idx] | bit_idx)
                     : (lcd_buffer[byte_idx] & ~bit_idx);
    }

    uint8_t * lcd() { return lcd_buffer; }

private:
    // inline int abs(int x) { return x > 0 ? x : -x; }

    uint8_t *lcd_buffer;
    int lcd_width;
    int lcd_height;
};
/**
 * @file mgui.h
 * @author karakirimu
 * @brief Simple monochrome GUI operation program
 * @version 0.1
 * @date 2023-07-16
 * 
 * @copyright Copyright (c) 2023 karakirimu
 * 
 */
#ifndef _MGUI_H_
#define _MGUI_H_

// typedefs
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

// enums
enum mgui_draw_line_dir {
    Left,
    Down
};

enum mgui_draw_type {
    Rectangle,
    Circle,
    Pixel,
    Text,
    Button,
    MenuItem
};

class mgui_draw {
public:

    /**
     * @brief Construct a new mgui draw object
     * 
     * @param width Target LCD width
     * @param height Target LCD height
     */
    mgui_draw(const uint8_t width, const uint8_t height, uint8_t *buffer){
        lcd_width = width;
        lcd_height = height;
        lcd_buffer = buffer;
    }

    /**
     * @brief Destroy the mgui draw object
     */
    ~mgui_draw(){}

    /**
     * @brief Draw circle
     * 
     * @param x0 center point of X
     * @param y0 center point of Y
     * @param r  radius
     * @param fill If true, set fill circle
     */
    void draw_circle(int x0, int y0, int r, bool fill = false) {

        if(fill){
            draw_circle_fill(x0, y0, r);
            return;
        }

        int x = r;
        int y = 0;
        int f = -(r << 1) + 3;

        while (x >= y)
        {
            // 1
            draw_pixel(x0 + x, y0 + y, true);
            draw_pixel(x0 + y, y0 - x, true);

            // 2
            draw_pixel(x0 + x, y0 - y, true);
            draw_pixel(x0 + y, y0 + x, true);

            // 3
            draw_pixel(x0 - x, y0 + y, true);
            draw_pixel(x0 - y, y0 + x, true);
            
            // 4
            draw_pixel(x0 - x, y0 - y, true);
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

    void draw_rectangle_rounded(int x0, int y0, int x1, int y1, int r, bool fill = false){

        if(fill){
            draw_rectangle_rounded_fill(x0, y0, x1, y1, r);
            return;
        }

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
            draw_pixel(px1 + x, py0 - y, true);
            draw_pixel(px1 + y, py0 - x, true);
            
            // 2
            draw_pixel(px1 + x, py1 + y, true);
            draw_pixel(px1 + y, py1 + x, true);
            
            // 3
            draw_pixel(px0 - x, py1 + y, true);
            draw_pixel(px0 - y, py1 + x, true);

            // 4
            draw_pixel(px0 - x, py0 - y, true);
            draw_pixel(px0 - y, py0 - x, true);
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
            draw_rectangle_fill(x0, y0, x1, y1);
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

    void draw_line_straight(int x0, int y0, int length, bool on, mgui_draw_line_dir direction) {

        if (direction == mgui_draw_line_dir::Left) {
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
        uint8_t bit_idx = (1 << (7 - y % 8));

        // Set or clear the bit at the specified index.
        lcd_buffer[byte_idx] 
            = on ? (lcd_buffer[byte_idx] | bit_idx)
                     : (lcd_buffer[byte_idx] & ~bit_idx);
    }

    /**
     * @brief Invert specified position.
     * 
     * @param x position of x
     * @param y position of y
     */
    void draw_pixel_invert(int x, int y){
        // Calculate the byte index.
        int byte_idx = (y >> 3) * lcd_width + x;
        uint8_t bit_idx = (1 << (7 - y % 8));

        // invert the bit at the specified index.
        lcd_buffer[byte_idx] ^= bit_idx;
    }

    /**
     * @brief Set the value for vertical 8px.
     * 
     * @param x top left point of X
     * @param y top left point of Y
     * @param value byte value to set
     */
    //void draw_virtical_8px(int x, int y, uint8_t value) {
    //    int byte_idx = (y >> 3) * lcd_width + x;
    //    lcd_buffer[byte_idx] = value;
    //}

    //void draw_virtical_1px(int x, int y, uint8_t value) {
    //    // Calculate the byte index.
    //    int byte_idx = (y >> 3) * lcd_width + x;
    //    uint8_t bit_idx = (1 << (7 - y % 8));
    //    bool on = (value & bit_idx) & bit_idx;

    //    // Set or clear the bit at the specified index.
    //    lcd_buffer[byte_idx] 
    //        = on ? (lcd_buffer[byte_idx] | bit_idx)
    //                 : (lcd_buffer[byte_idx] & ~bit_idx);
    //}

    /**
     * @brief It returns generated buffer.
     * 
     * @return uint8_t* 
     */
    uint8_t * lcd() { return lcd_buffer; }

private:
    // inline int abs(int x) { return x > 0 ? x : -x; }

    inline void draw_circle_fill(int x0, int y0, int r) {
        int x = r;
        int y = 0;
        int f = -(r << 1) + 3;

        while (x >= y)
        {

            for(int xd = 0; xd < x; xd++){
                // 1
                draw_pixel(x0 + xd, y0 + y, true);
                draw_pixel(x0 + y, y0 - xd, true);

                // 2
                draw_pixel(x0 + xd, y0 - y, true);
                draw_pixel(x0 + y, y0 + xd, true);

                // 3
                draw_pixel(x0 - xd, y0 + y, true);
                draw_pixel(x0 - y, y0 + xd, true);
                
                // 4
                draw_pixel(x0 - xd, y0 - y, true);
                draw_pixel(x0 - y, y0 - xd, true);
            }
            
            if (f >= 0)
            {
                x--;
                f -= (x << 2);
            }
            y++;
            f += (y << 2) + 2;
        }
    }

    inline void draw_rectangle_rounded_fill(int x0, int y0, int x1, int y1, int r){
        int x = r;
        int y = 0;
        int f = -(r << 1) + 3;

        int px0 = x0 + r;
        int px1 = x1 - r;
        int py0 = y0 + r;
        int py1 = y1 - r;

        while (x >= y)
        {
            for(int xd = 0; xd <= x; xd++){
                // 1
                draw_pixel(px1 + xd, py0 - y, true);
                draw_pixel(px1 + y, py0 - xd, true);
                
                // 2
                draw_pixel(px1 + xd, py1 + y, true);
                draw_pixel(px1 + y, py1 + xd, true);
                
                // 3
                draw_pixel(px0 - xd, py1 + y, true);
                draw_pixel(px0 - y, py1 + xd, true);

                // 4
                draw_pixel(px0 - xd, py0 - y, true);
                draw_pixel(px0 - y, py0 - xd, true);
            }

            if (f >= 0)
            {
                x--;
                f -= (x << 2);
            }
            y++;
            f += (y << 2) + 2;
        }

        // fill side
        for(int dx = 0; dx <= r; dx++){
            for(int dy = py0; dy <= py1; dy++){
                draw_pixel(x0 + dx, dy, true);
                draw_pixel(px1 + dx, dy, true);
            }
        }

        draw_rectangle_fill(px0, y0, px1, y1);
    }

    inline void draw_rectangle_fill(int x0, int y0, int x1, int y1){
        for(int x = x0; x <= x1; x++){
            for(int y = y0; y <= y1; y++){
                draw_pixel(x, y, true);
            }
        }
    }

    uint8_t *lcd_buffer;
    int lcd_width;
    int lcd_height;
};

class mgui_object {
public:
    virtual mgui_draw_type type() const = 0;
    virtual void update(mgui_draw* draw) = 0;
};

struct mgui_list_node {
  mgui_object* obj;
  mgui_list_node* next;
};

class mgui_list {
 public:
  mgui_list() {
    head = nullptr;
    counter = 0;
  }

  ~mgui_list() {
    while(counter > 0){
        remove(head->obj);
    }
  }

  void add(mgui_object* item) {
    mgui_list_node* node = new mgui_list_node();
    node->obj = item;
    node->next = head;
    head = node;
    counter++;
  }

  mgui_object* get(int index) {
    mgui_list_node* node = head;
    for (int i = 0; i < index; i++) {
      node = node->next;
    }
    return node->obj;
  }

  void remove(mgui_object* item) {
    mgui_list_node* current_node = head;
    mgui_list_node* prev_node = nullptr;
    
    while (current_node != nullptr)
    {
        if(current_node->obj == item){
            break;
        }
        prev_node = current_node;
        current_node = current_node->next;
    }

    if(current_node == nullptr){
        return;
    }
    
    if(prev_node == nullptr){
        head = current_node->next;
    }else{
        prev_node->next = current_node->next;
    }
    delete current_node;
    counter--;
  }

  inline int count() { return counter; }
  inline mgui_list_node* first() { return head; }

 private:
  mgui_list_node* head;
  int counter;
};

class mgui {
public:
    /**
     * @brief Construct a new mgui draw object
     *
     * @param width Target LCD width
     * @param height Target LCD height
     */
    mgui(const uint8_t width, const uint8_t height) {
        lcd_width = width;
        lcd_height = height;
        int size = width * (height >> 3);
        lcd_buffer = new uint8_t[size];
        memset(lcd_buffer, 0, size);
        draw = new mgui_draw(width, height, lcd_buffer);
        list = new mgui_list();
    }

    ~mgui() {
        delete draw;
        delete list;
        delete[] lcd_buffer;
    }

    inline void add(mgui_object *item){
        list->add(item);
    }

    inline void remove(mgui_object *item){
        list->remove(item);
    }

    inline void display() {
        mgui_list_node* node = list->first();

        while(node != nullptr){
            node->obj->update(draw);
            node = node->next;
        }
    }

    inline uint8_t *lcd() { return lcd_buffer; }

private:
    mgui_draw* draw;
    mgui_list* list;
    uint8_t* lcd_buffer;
    int lcd_width;
    int lcd_height;
};

class mgui_base_property {
public:
    mgui_base_property() {
        x_ = 0;
        y_ = 0;
        width_ = 0;
        height_ = 0;
    };

    mgui_base_property(const uint16_t& width_, const uint16_t& height_, const uint16_t& x_, const uint16_t& y_)
        : width_(width_), height_(height_), x_(x_), y_(y_) {}

    bool operator==(const mgui_base_property& other) const
    {
        return width_ == other.width_ && height_ == other.height_ && x_ == other.x_ && y_ == other.y_;
    }

    inline uint16_t width() const { return width_; }
    inline void set_width(uint16_t width) { width_ = width; }

    inline uint16_t height() const { return height_; }
    inline void set_height(uint16_t height) { height_ = height; }

    inline uint16_t x() const { return x_; }
    inline void set_x(uint16_t x) { x_ = x; }

    inline uint16_t y() const { return y_; }
    inline void set_y(uint16_t y) { y_ = y; }

private:
    uint16_t width_;
    uint16_t height_;
    uint16_t x_;
    uint16_t y_;
};

class mgui_padding_property {
public:
    mgui_padding_property() {
        down_ = 0;
        left_ = 0;
        up_ = 0;
        right_ = 0;
    }

    mgui_padding_property(const uint16_t& left_, const uint16_t& up_, const uint16_t& right_, const uint16_t& down_)
        : left_(left_), up_(up_), right_(right_), down_(down_)
    {
    }

    bool operator==(const mgui_padding_property& other) const
    {
        return left_ == other.left_ && up_ == other.up_ && right_ == other.right_ && down_ == other.down_;
    }

    inline uint16_t left() const { return left_; }
    inline void set_left(uint16_t left) { left_ = left; }

    inline uint16_t up() const { return up_; }
    inline void set_up(uint16_t up) { up_ = up; }

    inline uint16_t right() const { return right_; }
    inline void set_right(uint16_t right) { right_ = right; }

    inline uint16_t down() const { return down_; }
    inline void set_down(uint16_t down) { down_ = down; }

private:
    uint16_t left_;
    uint16_t up_;
    uint16_t right_;
    uint16_t down_;

};

class mgui_font_property {
public:
    inline uint16_t font_width() const { return font_width_; }
    inline void set_font_width(uint16_t font_width) { font_width_ = font_width; }

    inline uint16_t font_height() const { return font_height_; }
    inline void set_font_height(uint16_t font_height) { font_height_ = font_height; }

    inline uint8_t* resource() const { return resource_; }
    inline void set_resource(uint8_t* resource) { resource_ = resource; }

    virtual int search(const char *c) = 0;

private:
    uint16_t font_width_;
    uint16_t font_height_;
    uint8_t* resource_;
};

/**
 * @brief It draws pixel at specified position.
 * 
 */
class mgui_pixel : mgui_object {
public:
    mgui_pixel() {
        x_ = 0;
        y_ = 0;
        on_ = 0;
        invert_ = 0;
    }
    virtual ~mgui_pixel(){}

    mgui_draw_type type() const { return mgui_draw_type::Pixel; }

    inline uint16_t x() const { return x_; }
    inline void set_x(uint16_t x) { x_ = x; }

    inline uint16_t y() const { return y_; }
    inline void set_y(uint16_t y) { y_ = y; }

    inline uint8_t on() const { return on_; }
    inline void set_on(uint8_t on) { on_ = on; }

    inline uint8_t invert() const { return invert_; }
    inline void set_invert(uint8_t invert) { invert_ = invert; }

    void update(mgui_draw* draw) {
        if(invert_) {
            draw->draw_pixel_invert(x_, y_);
            return;
        }

        draw->draw_pixel(x_, y_, on_);
    }

private:
    uint16_t x_;
    uint16_t y_;
    uint8_t on_;
    uint8_t invert_;
};

/**
 * @brief It draws circle.
 * 
 */
class mgui_circle : mgui_object {
public:
    mgui_circle() {
        x_ = 0;
        y_ = 0;
        r_ = 0;
        fill_ = 0;
    }
    virtual ~mgui_circle(){}

    mgui_draw_type type() const { return mgui_draw_type::Circle; }

    inline uint16_t x() const { return x_; }
    inline void set_x(uint16_t x) { x_ = x; }

    inline uint16_t y() const { return y_; }
    inline void set_y(uint16_t y) { y_ = y; }

    inline uint16_t radius() const { return r_; }
    inline void set_radius(uint16_t r) { r_ = r; }

    inline uint8_t fill() const { return fill_; }
    inline void set_fill(uint8_t fill) { fill_ = fill; }

    void update(mgui_draw* draw) {
        draw->draw_circle(x_, y_, r_, fill_);
    }

private:
    uint16_t x_;
    uint16_t y_;
    uint16_t r_;
    uint8_t fill_;
};

/**
 * @brief It draws rectangle.
 * 
 */
class mgui_rectangle : public mgui_base_property, mgui_object {
public:
    mgui_rectangle() {
        r_ = 0;
        fill_ = 0;
    }
    virtual ~mgui_rectangle() {}

    mgui_draw_type type() const { return mgui_draw_type::Rectangle; }

    inline uint16_t radius() const { return r_; }
    inline void set_radius(uint16_t r) { r_ = r; }

    inline uint8_t fill() const { return fill_; }
    inline void set_fill(uint8_t fill) { fill_ = fill; }

    void update(mgui_draw* draw) {

        if(r_ > 0){
            draw->draw_rectangle_rounded(
                x(), y(), x() + width(), y() + height(), r_, fill_);
            return;
        }

        draw->draw_rectangle(x(), y(), x() + width(), y() + height(), fill_);
    }

private:
    uint16_t r_;
    uint8_t fill_;
};

/**
 * @brief It draws text.
 */
class mgui_text : public mgui_base_property, mgui_object {
public:
    mgui_text() {
        text_ = nullptr;
        text_index_ = nullptr;
        invert_ = false;
    }

    ~mgui_text() {
        reset_text_();
    }

    mgui_draw_type type() const { return mgui_draw_type::Text; }
    
    void update(mgui_draw* draw) {
        for(int i = 0; i < text_length; i++) {
            int x0 = x() + font()->font_width() * i;
            draw_char(draw, x0, y(), text_index_[i]);
        }
    }

    inline char* text() const { return text_; }
    inline void set_text(const char* text) {
        reset_text_();
        text_length = strlen(text);
        text_ = new char[text_length + 1];
        text_index_ = new int[text_length];
        memcpy(text_, text, text_length);
        text_[text_length] = '\0';

        for(int i = 0; i < text_length; i++){
            char c = *(text + i);
            text_index_[i] = font()->search(&c);
        }

        set_width(font()->font_width() * text_length);
        set_height(font()->font_height());
    }

    inline mgui_font_property* font() const { return font_; }
    inline void set_font(mgui_font_property* font) { font_ = font; }

    inline uint8_t invert() const { return invert_; }
    inline void set_invert(uint8_t invert) { invert_ = invert; }

private:
    inline void draw_char(mgui_draw* draw, int x, int y, int index) {               
        //if(font()->font_height() % 8 == 0
        //    && x % 8 == 0
        //    && y % 8 == 0){
        //    int vh = font()->font_height() / 8;
        //    for(int y0 = 0; y0 < vh; y0++) {
        //        int vy = y0 * 8;
        //        for(int x0 = 0; x0 < font()->font_width(); x0++) {
        //            int pos = index + vy + x0;
        //            uint8_t value = (invert_)? ~(font()->resource()[pos]) : font()->resource()[pos];
        //            draw->draw_virtical_8px(x+x0, y + vy, value);
        //        }
        //    }
        //    return;
        //}

        for(int y1 = 0; y1 < font()->font_height(); y1++) {
            for(int x1 = 0; x1 < font()->font_width(); x1++) {
                int pos = index + y1 / 8 * font()->font_width() + x1;
                bool checkbit = check_bit_on(x1, y1, font()->resource()[pos]);
                if (checkbit) {
                    draw->draw_pixel(x + x1, y + y1, (invert_)? !checkbit : checkbit);
                }
            }
        }
    }

    bool check_bit_on(int x, int y, uint8_t value) {
        uint8_t bit_idx = (1 << (7 - y % 8));
        bool on = (value & bit_idx) & bit_idx;
        return on;
    }

    inline void reset_text_() {
        if (text_) {
            delete[] text_index_;
            delete[] text_;
            text_index_ = nullptr;
            text_ = nullptr;
            text_length = 0;
        }
    }

    inline int strlen(const char* str) {
        int len = 0;
        while (*str != '\0') {
            len++;
            str++;
        }
        return len;
    }

    mgui_font_property* font_;
    uint8_t invert_;
    char* text_;
    int* text_index_;
    int text_length;
};

// class mgui_menu_item : mgui_draw, mgui_base_property, mgui_object {
// public:
//     void menu_item(bool select){

//     }
// };

 class mgui_button : mgui_base_property, mgui_object {
 public:
     mgui_button(uint16_t x, uint16_t y) {
         set_x(x);
         set_y(y);
         text_rel_x_ = 0;
         text_rel_y_ = 0;
     };
     ~mgui_button() {
         on_press_ = nullptr;
     };

     mgui_draw_type type() const { return mgui_draw_type::Button; };

     void update(mgui_draw* draw) {
         uint8_t press = (*on_press_)();
         rect_.set_fill(press);
         text_.set_invert(press);
         rect_.update(draw);
         text_.update(draw);
     };

     inline void set_on_press(bool (*on_press)()) { on_press_ = on_press; };

     inline uint16_t radius() const { return rect_.radius(); }
     inline void set_radius(uint16_t radius) {
         rect_.set_radius(radius);
     }

     inline char* text() const { return text_.text(); }
     inline void set_text(const char* text, mgui_font_property *font, uint16_t text_rel_x = 0, uint16_t text_rel_y = 0) {
         text_.set_font(font);
         text_.set_text(text);
         text_rel_x_ = text_rel_x;
         text_rel_y_ = text_rel_y;
         update_property();
     }

     inline void set_padding(uint16_t left, uint16_t up, uint16_t right, uint16_t down) {
         padding_.set_left(left);
         padding_.set_up(up);
         padding_.set_right(right);
         padding_.set_down(down);
         update_property();
     }
     
     inline mgui_padding_property padding() {
         return padding_;
     }

 private:
     inline void update_property() {
         uint16_t w = text_.width() + padding_.left() + padding_.right();
         uint16_t h = text_.height() + padding_.up() + padding_.down();

         uint16_t rx = x();
         uint16_t ry = y();
         rect_.set_x(rx);
         rect_.set_y(ry);
         rect_.set_width(w);
         rect_.set_height(h);

         text_.set_x(rx + text_rel_x_ + padding_.left());
         text_.set_y(ry + text_rel_y_ + padding_.up());
     }

     mgui_padding_property padding_;
     bool (*on_press_)();
     mgui_rectangle rect_;
     mgui_text text_;
     uint16_t text_rel_x_;
     uint16_t text_rel_y_;

 };

#endif _MGUI_H_
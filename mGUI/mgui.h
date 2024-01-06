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
#ifndef MGUI_H
#define MGUI_H

// typedefs
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

// enums
enum mgui_draw_line_dir {
    Left,
    Down
};

enum mgui_object_type {
    Rectangle,
    Circle,
    Triangle,
    Pixel,
    Text,
    Button,
    MenuItem,
    Menu,
    UiGroup
};

/**
 * @brief 
 * Set a pixel, line, or shape at an arbitrary position in the preallocated lcd buffer array
 */
class mgui_draw {
public:

    /**
     * @brief Construct a new mgui draw object
     * 
     * @param width Target LCD width
     * @param height Target LCD height
     */
    explicit mgui_draw(const uint8_t width, const uint8_t height, uint8_t *buffer){
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

    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool invert = false) {
        
        draw_line(x0, y0, x1, y1, !invert);
        draw_line(x0, y0, x2, y2, !invert);
        draw_line(x1, y1, x2, y2, !invert);
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

    /**
     * @brief Sets the color at the specified position
     * 
     * @param x x position
     * @param y y position
     * @param on 
     * If true, the bit at the position corresponding to (x,y) is set to 1;
     * if false, it is set to 0
     */
    void draw_pixel(int x, int y, bool on){
        // Calculate the byte index.
        int byte_idx = (y >> 3) * lcd_width + x;
        uint8_t bit_idx = (1 << (y % 8));

        // Set or clear the bit at the specified index.
        lcd_buffer[byte_idx] 
            = on ? (lcd_buffer[byte_idx] | bit_idx)
                     : (lcd_buffer[byte_idx] & ~bit_idx);
    }

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

    /**
     * @brief Draw a filled rounded corner rectangle
     * 
     * @param x0 x position at upper left
     * @param y0 y position at upper left
     * @param x1 x position at bottom right
     * @param y1 y position of bottom right
     * @param r rounded corner radius
     */
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

    /**
     * @brief Draw a filled rectangle
     * 
     * @param x0 x position at upper left
     * @param y0 y position at upper left
     * @param x1 x position at bottom right
     * @param y1 y position at bottom right
     */
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
    virtual mgui_object_type type() const = 0;
    virtual void update(mgui_draw* draw) = 0;
};

template <typename O>
struct mgui_list_node {
  O obj;
  mgui_list_node* next;
};

template <typename T>
class mgui_list {
 public:
  mgui_list() {
    head = nullptr;
    tail = nullptr;
    counter = 0;
  }

  mgui_list(const mgui_list& other) {
      head = nullptr;
      tail = nullptr;
      counter = 0;

      for (mgui_list_node<T>* node = other.head; node != nullptr; node = node->next) {
          add(node->obj);
      }
  }

  mgui_list& operator=(const mgui_list& other) {
      if (this != &other) {
          clear();
          for (mgui_list_node<T>* node = other.head; node != nullptr; node = node->next) {
              add(node->obj);
          }
      }
      return *this;
  }

  mgui_list(mgui_list&& other) noexcept {
      head = other.head;
      tail = other.tail;
      counter = other.counter;

      other.head = nullptr;
      other.tail = nullptr;
      other.counter = 0;
  }

  mgui_list& operator=(mgui_list&& other) noexcept {
      if (this != &other) {
          clear();

          head = other.head;
          tail = other.tail;
          counter = other.counter;

          other.head = nullptr;
          other.tail = nullptr;
          other.counter = 0;
      }
      return *this;
  }

  ~mgui_list() {
    clear();
  }

  void add(const T &item) {
    mgui_list_node<T>* node = new mgui_list_node<T>();
    node->obj = item;
    node->next = nullptr;

    if(head == nullptr){
        head = node;
    }else{
        tail->next = node;
    }
    tail = node;
    counter++;
  }

  const T get(const int index) {
    mgui_list_node<T>* node = head;
    for (int i = 0; i < index; i++) {
      node = node->next;
    }
    return node->obj;
  }

  void remove(const T &item) {
    mgui_list_node<T>* current_node = head;
    mgui_list_node<T>* prev_node = nullptr;
    
    while (current_node != nullptr)
    {
        if(current_node->obj == item){
            break;
        }
        prev_node = current_node;
        current_node = current_node->next;
    }

    if(current_node == nullptr){
        // No item found
        return;
    }
    
    if(prev_node == nullptr){
        // First item removed
        head = current_node->next;

        if (head == nullptr) {
            // No item listed
            tail = nullptr;
        }

    }else{
        // Other
        prev_node->next = current_node->next;

        if(current_node->next == nullptr){
            // Last item removed
            tail = prev_node;
        }
    }
    delete current_node;
    counter--;
  }

  void clear() {
    while(counter > 0){
        remove(head->obj);
    }
  }

  inline int count() const { return counter; }
  inline mgui_list_node<T>* first() { return head; }
  inline mgui_list_node<T>* last() { return tail; }

 private:
  mgui_list_node<T>* head;
  mgui_list_node<T>* tail;
  int counter;
};


template <typename T>
class mgui_stack {
public:
    mgui_stack() {
        head_ = nullptr;
    }

    ~mgui_stack() {
        while (head_ != nullptr) {
            pop();
        }
    }

    void push(const T &item) {
        mgui_list_node<T>* node = new mgui_list_node<T>();
        node->obj = item;
        node->next = head_;
        head_ = node;
    }

    T pop() {
        mgui_list_node<T>* node = head_;
        head_ = node->next;

        return static_cast<T&&>(node->obj);
    }

    inline bool is_empty() const { return head_ == nullptr; }

private:
    mgui_list_node<T>* head_;
};

class mgui {
public:
    /**
     * @brief Construct a new mgui draw object
     *
     * @param width Target LCD width
     * @param height Target LCD height
     */
    explicit mgui(const uint8_t width, const uint8_t height) {
        lcd_width = width;
        lcd_height = height;
        buffer_size = width * (height >> 3);
        lcd_buffer = new uint8_t[buffer_size]();
        memset(lcd_buffer, 0, buffer_size);
        draw = new mgui_draw(width, height, lcd_buffer);
        list = new mgui_list<mgui_object*>();
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

    inline void clear(){
        list->clear();
    }

    inline void update_lcd() {
        mgui_list_node<mgui_object*>* node = list->first();

        // clear buffer
        memset(lcd_buffer, 0, buffer_size);

        // set settings
        while(node != nullptr){
            node->obj->update(draw);
            node = node->next;
        }
    }

    inline uint8_t *lcd() { return lcd_buffer; }

private:
    mgui_draw* draw;
    mgui_list<mgui_object*>* list;
    uint8_t* lcd_buffer;
    int lcd_width;
    int lcd_height;
    int buffer_size;
};

class mgui_base_property {
public:
    /**
     * @brief Construct a new mgui base property object
     * The arguments of this constructor default to 0,
     * since it is not necessary to set the entire argument
     * on the part of the inherited class.
     * 
     * @param x LCD X position
     * @param y LCD Y position
     * @param width Width of objects that inherit this property
     * @param height Height of objects that inherit this property
     */
    mgui_base_property(uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0) {
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;
    };

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
    mgui_font_property() {
        font_width_ = 0;
        font_height_ = 0;
        resource_ = nullptr;
    }

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

    mgui_object_type type() const { return mgui_object_type::Pixel; }

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
            draw->draw_pixel(x_, y_, !on_);
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

    mgui_object_type type() const { return mgui_object_type::Circle; }

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

    mgui_object_type type() const { return mgui_object_type::Rectangle; }

    inline uint16_t radius() const { return r_; }
    inline void set_radius(uint16_t r) { r_ = r; }

    inline uint8_t fill() const { return fill_; }
    inline void set_fill(uint8_t fill) { fill_ = fill; }

    void update(mgui_draw* draw) {

        if(r_ > 0){
            draw->draw_rectangle_rounded(
                x(), y(), x() + width() - 1, y() + height() - 1, r_, fill_);
            return;
        }

        draw->draw_rectangle(x(), y(), x() + width() - 1, y() + height() - 1, fill_);
    }

private:
    uint16_t r_;
    uint8_t fill_;
};

/**
 * @brief It draws triangle.
 *
 */
class mgui_triangle : mgui_object {
public:
    mgui_triangle() {
        x0_ = 0;
        y0_ = 0;
        x1_ = 0;
        y1_ = 0;
        x2_ = 0;
        y2_ = 0;
        invert_ = 0;
    }
    virtual ~mgui_triangle() {}

    mgui_object_type type() const { return mgui_object_type::Triangle; }

    inline uint16_t x0() const { return x0_; }
    inline void set_x0(uint16_t x0) { x0_ = x0; }

    inline uint16_t y0() const { return y0_; }
    inline void set_y0(uint16_t y0) { y0_ = y0; }

    inline uint16_t x1() const { return x1_; }
    inline void set_x1(uint16_t x1) { x1_ = x1; }

    inline uint16_t y1() const { return y1_; }
    inline void set_y1(uint16_t y1) { y1_ = y1; }

    inline uint16_t x2() const { return x2_; }
    inline void set_x2(uint16_t x2) { x2_ = x2; }

    inline uint16_t y2() const { return y2_; }
    inline void set_y2(uint16_t y2) { y2_ = y2; }

    inline uint8_t invert() const { return invert_; }
    inline void set_invert(uint8_t invert) { invert_ = invert; }

    void update(mgui_draw* draw) {
        draw->draw_triangle(x0_, y0_, x1_, y1_, x2_, y2_, invert_);
    }

private:
    uint16_t x0_;
    uint16_t y0_;
    uint16_t x1_;
    uint16_t y1_;
    uint16_t x2_;
    uint16_t y2_;
    uint8_t invert_;
};

/**
 * @brief It draws text.
 */
class mgui_text : public mgui_base_property, mgui_object {
public:
    /**
     * @brief Construct a new mgui text object
     * 
     * @param font The inherited class of mgui_font_property
     * @param text Text to display
     * @param x X position in the upper left corner where mgui_font_property resource is displayed
     * @param y Y position in the upper left corner where mgui_font_property resource is displayed
     */
    mgui_text(mgui_font_property *font, const char* text = nullptr, uint16_t x = 0, uint16_t y = 0) {
        font_ = font;
        text_ = nullptr;
        text_index_ = nullptr;
        text_length = 0;

        if(text) {
            set_text(text);
        }
        set_x(x);
        set_y(y);
        invert_ = false;
    }

    ~mgui_text() {
        reset_text_();
    }

    mgui_object_type type() const { return mgui_object_type::Text; }
    
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

/**
 * @brief 
 * UI that requires operation such as buttons and menus are always objects that
 * must be inherited. 
 */
class mgui_core_ui : public mgui_object {
public:
    mgui_core_ui() {
        on_press_ = false;
        on_selected_ = false;
    }
    virtual ~mgui_core_ui() {
        on_press_ = false;
        on_selected_ = false;
    }

    /**
     * @brief
     * Set the press state of an element
     * 
     * @param on_press If true, the state is pressed; if false, the state is not pressed
     */
    inline void set_on_press(bool on_press) { on_press_ = on_press; }

    /**
     * @brief
     * Get the pressed state of an element
     * 
     * @return true Pressed
     * @return false Not pressed
     */
    inline bool get_on_press() const { return on_press_; }

    /**
     * @brief
     * Set the selection state of an element
     * 
     * @param on_selected If true, selected; if false, not selected
     */
    inline void set_on_selected(bool on_selected) { on_selected_ = on_selected; }

    /**
     * @brief 
     * Get the selection state of an element
     * 
     * @return true Selected
     * @return false Not selected
     */
    inline bool get_on_selected() const { return on_selected_; }

private:
    bool on_press_;
    bool on_selected_;
};

class mgui_button : public mgui_core_ui {
 public:

     /**
      * @brief Construct a new button object
      * 
      * @param x X position in the upper left corner
      * @param y Y position in the upper left corner
      * @param width Width of button (ignored if text is set)
      * @param height Height of button (ignored if text is set)
      */
     explicit mgui_button(uint16_t x, uint16_t y, uint16_t width = 0, uint16_t height = 0)
         : mgui_core_ui() {
         rect_.set_x(x);
         rect_.set_y(y);
         rect_.set_width(width);
         rect_.set_height(height);
         text_rel_x_ = 0;
         text_rel_y_ = 0;
         text_ = nullptr;

     };
     ~mgui_button(){};

     mgui_object_type type() const { return mgui_object_type::Button; };

     void update(mgui_draw* draw) {
        bool is_filled = get_on_selected()? !get_on_press() : get_on_press();

        rect_.set_fill(is_filled);
        rect_.update(draw);

        if (text_) {
            text_->set_invert(is_filled);
            text_->update(draw);
        }
     };

     inline mgui_text* text() const { return text_; }
     inline void set_text(mgui_text* text, uint16_t text_rel_x = 0, uint16_t text_rel_y = 0) {
         text_ = text;
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
     inline mgui_padding_property padding() const { return padding_; }

     inline uint16_t width() const { return rect_.width(); }
     inline void set_width(uint16_t width) { rect_.set_width(width); }

     inline uint16_t height() const { return rect_.height(); }
     inline void set_height(uint16_t height) { rect_.set_height(height); }

     inline uint16_t radius() const { return rect_.radius(); }
     inline void set_radius(uint16_t r) { rect_.set_radius(r); }

 private:
     inline void update_property() {
         uint16_t rect_w = text_->width() + padding_.left() + padding_.right();
         uint16_t rect_h = text_->height() + padding_.up() + padding_.down();

         uint16_t rx = rect_.x();
         uint16_t ry = rect_.y();
         rect_.set_x(rx);
         rect_.set_y(ry);
         rect_.set_width(rect_w == 0 ? rect_.width() : rect_w);
         rect_.set_height(rect_h == 0 ? rect_.height() : rect_h);

         text_->set_x(rx + text_rel_x_ + padding_.left());
         text_->set_y(ry + text_rel_y_ + padding_.up());
     }

     mgui_padding_property padding_;
     mgui_text *text_;
     uint16_t text_rel_x_;
     uint16_t text_rel_y_;
     mgui_rectangle rect_;
 };

/**
 * @brief 
 * Attribute information shared to create a hierarchical menu structure
 */
class mgui_menu_property {
public:
    mgui_menu_property() {
        selected_index_ = 0;
    }
    virtual ~mgui_menu_property(){}

    inline mgui_menu_property* get_property() { return this; }

    mgui_list<mgui_core_ui*> menu_item_;
    uint16_t selected_index_;
};

class mgui_menu_item : public mgui_core_ui {
public:
    explicit mgui_menu_item(uint16_t x = 0, uint16_t y = 0, uint16_t w = 0, uint16_t h = 0)
        : mgui_core_ui() {
        rect_.set_x(x);
        rect_.set_y(y);
        rect_.set_width(w);
        rect_.set_height(h);
        rect_.set_fill(true);
        text_ = nullptr;
        text_rel_x_ = 0;
        text_rel_y_ = 0;
        child_menu_ = nullptr;
    }
    ~mgui_menu_item(){}

    mgui_object_type type() const { return mgui_object_type::MenuItem; }

    void update(mgui_draw* draw) {
        bool is_filled = get_on_selected()? !get_on_press() : get_on_press();

        rect_.set_fill(is_filled);
        rect_.update(draw);
 
        if (text_) {
            text_->set_invert(is_filled);
            text_->update(draw);
        }

        if(child_menu_) {
            // draw triangle
        }
    }

    inline uint16_t radius() const { return rect_.radius(); }
    inline void set_radius(uint16_t radius) { rect_.set_radius(radius); }

    inline uint16_t width() const { return rect_.width(); }
    inline void set_width(uint16_t width) { rect_.set_width(width); }
    
    inline uint16_t height() const { return rect_.height(); }
    inline void set_height(uint16_t height) { rect_.set_height(height); }
    
    inline uint16_t x() const { return rect_.x(); }
    inline void set_x(uint16_t x) { rect_.set_x(x); }
    
    inline uint16_t y() const { return rect_.y(); }
    inline void set_y(uint16_t y) { rect_.set_y(y); }

    inline mgui_text* text() const { return text_; }
    inline void set_text(mgui_text* text, uint16_t text_rel_x = 0, uint16_t text_rel_y = 0) {
        text_ = text;
        text_->set_x(rect_.x());
        text_->set_y(rect_.y());
        text_rel_x_ = text_rel_x;
        text_rel_y_ = text_rel_y;
        update_property();
    }

    inline void set_menu(mgui_menu_property* menu) { child_menu_ = menu; }
    inline mgui_menu_property* menu() { return child_menu_; }

private:
    inline void update_property() {
        text_->set_x(text_->x() + text_rel_x_);
        text_->set_y(text_->y() + text_rel_y_);
    }

    mgui_menu_property* child_menu_;
    mgui_rectangle rect_;
    mgui_text *text_;
    uint16_t text_rel_x_;
    uint16_t text_rel_y_;
};

class mgui_menu : mgui_object {
public:
    mgui_menu(){
        moved_from_ = new mgui_stack<mgui_menu_property>();
    }
    ~mgui_menu(){
        delete moved_from_;
    }

    mgui_object_type type() const { return mgui_object_type::Menu; }
    void update(mgui_draw* draw) {
        mgui_list_node<mgui_core_ui*>* node = p.menu_item_.first();
        while (node != nullptr) {
            node->obj->update(draw);
            node = node->next;
        }
    }

    inline void add(mgui_core_ui* item) {
        if (item->type() == mgui_object_type::MenuItem) {
            p.menu_item_.add(item);
        }
    }

    inline void remove(mgui_core_ui* item) {
        if (item->type() == mgui_object_type::MenuItem) {
            p.menu_item_.remove(item);
        }
    }

    inline uint16_t selected_index() const { return p.selected_index_; }
    inline void set_selected_index(uint16_t index_) { p.selected_index_ = index_; }

    inline mgui_menu_property* get_property() { return p.get_property(); }

    inline mgui_menu_item* get_selected_item() {
        return static_cast<mgui_menu_item*>(p.menu_item_.get(p.selected_index_));
    }

    /**
    * @brief Return specified menu list.
    * @param on_enter The callback, this returns some button pressed.
    */
    inline void set_on_return(bool on_return) {
        if (on_return && moved_from_->is_empty() == false) {
            p = static_cast<mgui_menu_property&&>(moved_from_->pop());
        }
    }
     
    /**
    * @brief Specified menu item entered.
    * @param on_enter The callback, this returns some button pressed.
    */
    inline void set_on_enter(bool on_enter) {
        p.menu_item_.get(p.selected_index_)->set_on_press(on_enter);
        mgui_menu_property *menu = get_selected_item()->menu();

        if (menu) {
            // set own copy
            moved_from_->push(p);

            // update
            p.menu_item_ = menu->menu_item_;
            p.selected_index_ = menu->selected_index_;
        }
    }

    /**
    * @brief
    * Change the element selection to one after the other. If there is no element
    * later than the currently selected element, nothing is done.
    *
    * @param on_select_next
    * If true, set the flag to set the element one element later; if false, do nothing.
    */
    inline void set_on_select_next(bool on_select_next) {
        if (!on_select_next) {
            return;
        }

        if (p.selected_index_ < p.menu_item_.count() - 1) {
            p.menu_item_.get(p.selected_index_)->set_on_selected(false);
            p.menu_item_.get(p.selected_index_)->set_on_press(false);
            p.selected_index_++;
            p.menu_item_.get(p.selected_index_)->set_on_selected(true);
        }
    }

    /**
    * @brief
    * Changes the selection of an element to the one before it. If no element
    * precedes the currently selected element, nothing is done.
    *
    * @param on_select_prev
    * If true, set the flag to set the element to one previous element; if false, do nothing.
    */
    inline void set_on_select_prev(bool on_select_prev) {
        if (!on_select_prev) {
            return;
        }

        if (p.selected_index_ > 0) {
            p.menu_item_.get(p.selected_index_)->set_on_selected(false);
            p.menu_item_.get(p.selected_index_)->set_on_press(false);
            p.selected_index_--;
            p.menu_item_.get(p.selected_index_)->set_on_selected(true);
        }
    }

private:
    mgui_menu_property p;
    mgui_stack<mgui_menu_property> *moved_from_;
};

/**
* @brief
* - This class is used to assist in changing and confirming the selected button
*   on the screen in situations where there are few physical buttons relative to
*   the number of buttons on the screen. 
* 
* - If multiple buttons can be operated simultaneously, such as on a touch panel,
*   this class is not necessary, and it is more appropriate to set input operations
*   directly on mgui_button, etc.
* 
* - The order in which UIs are moved is the order in which they are assigned to
*   the add function.
* 
* @remarks
* - An example of using this class is when there are five on-screen buttons,
*   one physical button, and one rotary encoder, and the rotary encoder is
*   used to change the button selection, and the physical button is used to
*   confirm the selection.
*/
class mgui_ui_group : mgui_object {
public:
    mgui_ui_group() {
        list = new mgui_list<mgui_core_ui*>();
        selected_index_ = 0;
    }
    ~mgui_ui_group() {
        delete list;
    }

    mgui_object_type type() const { return mgui_object_type::UiGroup; }

    void update(mgui_draw* draw) {
        mgui_list_node<mgui_core_ui*>* node = list->first();
        while (node != nullptr) {
            node->obj->update(draw);
            node = node->next;
        }
    }

    /**
    * @brief
    * Register the UI to be operated within this group
    * @param item Elements to be added
    */
    inline void add(mgui_core_ui* item) { 
    list->add(item);
    reset_selection();
    }

    /**
    * @brief
    * Unregister the UI to be operated within this group
    * @param item Elements to be removed
    */
    inline void remove(mgui_core_ui* item) {
        list->remove(item);
        reset_selection();
    }

    inline void set_selected_index(uint16_t index_) { selected_index_ = index_; }
    inline uint16_t get_selected_index() const { return selected_index_; }

    /**
    * @brief
    * Change the element selection to one after the other. If there is no element
    * later than the currently selected element, nothing is done.
    * 
    * @param on_select_next 
    * If true, set the flag to set the element one element later; if false, do nothing.
    */
    inline void set_on_select_next(bool on_select_next) {
        if (!on_select_next) {
            return;
        }

        if (selected_index_ < list->count() - 1) {
            ((mgui_core_ui*)list->get(selected_index_))->set_on_selected(false);
            ((mgui_core_ui*)list->get(selected_index_))->set_on_press(false);
            selected_index_++;
            ((mgui_core_ui*)list->get(selected_index_))->set_on_selected(true);
        }
    }

    /**
    * @brief
    * Changes the selection of an element to the one before it. If no element
    * precedes the currently selected element, nothing is done.
    * 
    * @param on_select_prev
    * If true, set the flag to set the element to one previous element; if false, do nothing.
    */
    inline void set_on_select_prev(bool on_select_prev) { 
        if (!on_select_prev) {
            return;
        }

        if (selected_index_ > 0) {
            ((mgui_core_ui*)list->get(selected_index_))->set_on_selected(false);
            ((mgui_core_ui*)list->get(selected_index_))->set_on_press(false);
            selected_index_--;
            ((mgui_core_ui*)list->get(selected_index_))->set_on_selected(true);
        }
    }

    /**
    * @brief 
    * Set the press state of the currently selected element
    * @param on_press If true, the state is pressed; if false, the state is not pressed
    */
    inline void set_on_press(bool on_press) {
    ((mgui_core_ui*)list->get(selected_index_))->set_on_press(on_press);
    }

    /**
    * @brief 
    * Get the pressed state of the currently selected element
    * @return true Pressed
    * @return false Not pressed
    */
    inline bool get_on_press() const { 
    return ((mgui_core_ui*)list->get(selected_index_))->get_on_press();
    }

private:
     
    /**
    * @brief 
    * Resets the selection state and press state of all elements and sets the selection
    * index to 0.
    */
    inline void reset_selection() {
        selected_index_ = 0;
        mgui_list_node<mgui_core_ui*>* node = list->first();
        if(node == nullptr){
        return;
        }

        node->obj->set_on_press(false);
        node->obj->set_on_selected(true);
        node = node->next;

        while (node != nullptr) {
            node->obj->set_on_press(false);
            node->obj->set_on_selected(false);
            node = node->next;
        } 
    }
    mgui_list<mgui_core_ui*>* list;
    uint16_t selected_index_;
};

#endif // MGUI_H
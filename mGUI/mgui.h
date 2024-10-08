﻿/**
 * @file mgui.h
 * @author karakirimu
 * @brief Simple monochrome GUI operation program
 * @version 0.1
 * @date 2024-07-26
 * 
 * @copyright Copyright (c) 2024 karakirimu
 * 
 */
#ifndef MGUI_H
#define MGUI_H

// typedefs
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

// prototype declare
class mgui_menu_item;

/**
 * @brief The size of the hash table used in the mgui_string_map class.
 *
 * This constant specifies the number of buckets in the hash table used by the
 * mgui_string_map class to store key-value pairs. Increasing this value can
 * improve the performance of the hash table, but it may also increase memory
 * usage.
 */
constexpr int HASH_TABLE_SIZE = 20;

/**
 * @brief Enumeration representing the direction for drawing a straight line.
 *
 * This enumeration is used as a parameter of the drawing function to specify the
 * direction of the straight line to be drawn.
 */
enum mgui_draw_line_dir {
    /**
     * @brief Draw a straight line from left to right
     */
    Left,
    /**
     * @brief Draw a straight line from top to bottom
     */
    Down
};

/**
 * @brief List of object types that use drawing functions
 *
 * its corresponding drawing function.
 */
enum mgui_object_type {
    /**
     * @brief Rectangle drawing function
     */
    Rectangle,
    /**
     * @brief Circle drawing function
     */
    Circle,
    /**
     * @brief Triangle drawing function
     */
    Triangle,
    /**
     * @brief Pixel drawing function
     */
    Pixel,
    /**
     * @brief Line drawing function
     */
    Line,
    /**
     * @brief Text drawing function
     */
    Text,
    /**
     * @brief Image drawing function
     */
    Image,
    /**
     * @brief Button drawing function
     */
    Button,
    /**
     * @brief Vertical scrollbar drawing function
     */
    VerticalScroll,
    /**
     * @brief Menu item drawing function
     */
    MenuItem,
    /**
     * @brief Menu drawing function
     */
    Menu,
    /**
     * @brief UI group drawing function
     */
    UiGroup
};

/**
 * @brief
 * Types of parameters entered using GPIO or other communication methods
 */
enum mgui_input_type {
    /**
     * @brief 
     * Push switches, variable resistors, rotary encoders, etc. 
     * that can represent input states within a single int value.
     */
    Single,
};

/**
 * @brief
 * Structure for storing the result of input value acquisition 
 * and passing it to the drawing section.
 */
struct mgui_input_state {

    /**
     * @brief Type of input set up
     */
    mgui_input_type type;

    /**
     * @brief Current input value
     */
    int value_1;
};

/**
 * @brief
 * A simple node structure for the mgui_list class.
 */
template <typename O>
struct mgui_list_node {
    /**
     * @brief
     * Object of any type to be stored in the list.
     */
    O obj;
    /**
     * @brief
     * Pointer to the next element in the list.
     */
    mgui_list_node* next = nullptr;
};

/**
 * @brief
 * A simple list class that can be used for various object types.
 * It is created to avoid using standard functions.
 */
template <typename T>
class mgui_list {
public:
    /**
     * @brief Constructor for the mgui_list class.
     * Initializes the head, tail, and counter members to nullptr and 0 respectively.
     */
    mgui_list() {
        head = nullptr;
        tail = nullptr;
        counter = 0;
    }

    /**
     * @brief Copy constructor for the mgui_list class.
     *
     * This constructor makes a copy of the elements in the list by iterating
     * over the list and calling the add function for each element.
     *
     * @param other The mgui_list object to be copied.
     */
    mgui_list(const mgui_list& other) {
        head = nullptr;
        tail = nullptr;
        counter = 0;

        for (mgui_list_node<T>* node = other.head; node != nullptr; node = node->next) {
            add(node->obj);
        }
    }

    /**
     * @brief Move constructor for the mgui_list class.
     *
     * This constructor moves the elements from the other list to this list.
     *
     * @param other The mgui_list object to move from.
     */
    mgui_list(mgui_list&& other) noexcept {
        // Move the elements from the other list to this list
        head = other.head;
        tail = other.tail;
        counter = other.counter;

        // Reset the other list
        other.head = nullptr;
        other.tail = nullptr;
        other.counter = 0;
    }

    /**
     * @brief Destructor for the mgui_list class.
     *
     * Destroys all elements in the list and cleans up the memory.
     */
    ~mgui_list() {
        clear();
    }

    /**
     * @brief Copy assignment operator.
     * 
     * @param other The mgui_list object to be copied.
     * @return A reference to the current mgui_list object.
     */
    mgui_list& operator=(const mgui_list& other) {
        if (this != &other) {
            clear();
            for (mgui_list_node<T>* node = other.head; node != nullptr; node = node->next) {
                add(node->obj);
            }
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * 
     * @param other The mgui_list object to move from.
     * @return A reference to the current mgui_list object.
     */
    mgui_list& operator=(mgui_list&& other) noexcept {
        if (this != &other) {
            // Clear current list
            clear();

            // Move elements from other list
            head = other.head;
            tail = other.tail;
            counter = other.counter;

            // Reset other list
            other.head = nullptr;
            other.tail = nullptr;
            other.counter = 0;
        }
        return *this;
    }

    /**
     * @brief Appends a new item to the end of a linked list.
     *
     * @param item A new item to append. The objects being set must be comparable.
     */
    void add(const T& item) {
        mgui_list_node<T>* node = new mgui_list_node<T>();
        node->obj = item;
        node->next = nullptr;

        if (head == nullptr) {
            head = node;
        }
        else {
            tail->next = node;
        }
        tail = node;
        counter++;
    }

    /**
     * @brief Get the selected index object
     *
     * @param index selected index
     * @return const T selected object or nullptr
     */
    const T get(const int index) {
        mgui_list_node<T>* node = head;
        for (int i = 0; i < index; i++) {
            node = node->next;
        }
        return node->obj;
    }

    /**
     * @brief Get the list node object. Use when you want to scan from a specific index.
     * @param index selected index
     * @return mgui_list_node<T>* List node for selected index or nullptr.
     */
    mgui_list_node<T>* get_node(const int index) {
        mgui_list_node<T>* node = head;
        for (int i = 0; i < index; i++) {
            node = node->next;
        }
        return node;
    }

    /**
     * @brief deletes the specified object
     *
     * @param item Comparable objects
     */
    void remove(const T& item) {
        mgui_list_node<T>* current_node = head;
        mgui_list_node<T>* prev_node = nullptr;

        while (current_node != nullptr)
        {
            if (current_node->obj == item) {
                break;
            }
            prev_node = current_node;
            current_node = current_node->next;
        }

        if (current_node == nullptr) {
            // No item found
            return;
        }

        if (prev_node == nullptr) {
            // First item removed
            head = current_node->next;

            if (head == nullptr) {
                // No item listed
                tail = nullptr;
            }

        }
        else {
            // Other
            prev_node->next = current_node->next;

            if (current_node->next == nullptr) {
                // Last item removed
                tail = prev_node;
            }
        }
        delete current_node;
        counter--;
    }

    /**
     * @brief Deletes all item.
     */
    void clear() {
        while (counter > 0) {
            remove(head->obj);
        }
    }

    /**
     * @brief Get the item count
     *
     * @return int item count
     */
    inline int count() const { return counter; }

    /**
     * @brief It returns first node
     *
     * @return mgui_list_node<T>* first list node
     */
    inline mgui_list_node<T>* first() { return head; }

    /**
     * @brief It returns last node
     *
     * @return mgui_list_node<T>* last list node
     */
    inline mgui_list_node<T>* last() { return tail; }

private:
    mgui_list_node<T>* head;
    mgui_list_node<T>* tail;
    int counter;
};

template <typename T>
/**
 * @brief
 * A simple stack class that can be used for various object types.
 * It is created to avoid using standard functions.
 */
class mgui_stack {
public:
    /**
     * @brief Constructor for mgui_stack
     *
     * Initializes the head of the stack to nullptr.
     */
    mgui_stack() { 
        // Initialize the head of the stack to nullptr
        head_ = nullptr;
    }

    /**
     * @brief Destructor for mgui_stack
     *
     * The destructor removes all the elements from the stack and deletes them.
     */
    virtual ~mgui_stack() {
        // Keep removing elements from the stack until it is empty
        while (head_ != nullptr) {
            pop();
        }
    }

    /**
     * @brief Add item to the end of stack.
     *
     * @param item object
     */
    void push(const T& item) {
        mgui_list_node<T>* node = new mgui_list_node<T>();
        node->obj = item;
        node->next = head_;
        head_ = node;
    }

    /**
     * @brief Take the element from the beginning and move the element reference
     *
     * @return T first object reference
     */
    T pop() {
        mgui_list_node<T>* node = head_;
        head_ = node->next;

        return static_cast<T&&>(node->obj);
    }

    /**
     * @brief Check if number of elements is 0
     *
     * @return true It has no elements
     * @return false One or more elements exist.
     */
    inline bool is_empty() const { return head_ == nullptr; }

private:
    mgui_list_node<T>* head_;
};

/**
 * @brief
 * Basic string management class.
 * Created to avoid implementing standard functions.
 */
class mgui_string {
public:
    mgui_string() {
        str_length_ = 0;
        str_ = nullptr;
    }

    /**
     * @brief Constructs a `mgui_string` object from a C-style string.
     * @param str the C-style string to initialize the `mgui_string` object with
     */
    mgui_string(const char* str) {
        build(str);
    }

    /**
     * @brief Copy constructor
     * @param other The object to copy from
     */
    mgui_string(const mgui_string& other) {
        build(other.c_str());
    }

    /**
     * @brief Destructor. Deletes the C-style string.
     */
    ~mgui_string() {
        clear();
    }

    /**
     * @brief
     * Subscript operator.
     *
     * Returns the character at the specified index in the string.
     * If the index is out of range, returns null character.
     *
     * @param index the index of the character to retrieve
     * @return the character at the specified index, or null character if index is out of range
     */
    const char operator[](int index) {
        if (strlen(str_) != str_length_) {
            return '\0';
        }

        return str_[index];
    }

    /**
     * @brief
     * Assignment operator.
     *
     * Copies a C-style string to the `mgui_string` object.
     *
     * @param str the C-style string to copy
     * @return a reference to the `mgui_string` object
     */
    mgui_string operator=(const char* str) {
        if (str != nullptr) {
            clear();
            build(str);
        }
        return *this;
    }

    /**
     * @brief
     * Assignment operator.
     *
     * Copies the content of another `mgui_string` object to this object.
     *
     * @param other the `mgui_string` object to copy from
     * @return a reference to this `mgui_string` object
     */
    mgui_string operator=(const mgui_string& other) noexcept {
        // If the current string is not empty, clear it
        if (this->str_ != nullptr) {
            clear();
        }
        
        // If the current object is not the same as the other object, copy the content
        if (this != &other) {
            build(other.c_str());
        }
        
        return *this;
    }

    /**
     * @brief
     * Equality operator.
     *
     * Compares the content of this `mgui_string` object with another `mgui_string` object.
     *
     * @param str the `mgui_string` object to compare with
     * @return `true` if the content of the two `mgui_string` objects are equal, `false` otherwise
     */
    bool operator==(const mgui_string& str) const {
        // If the lengths of the two strings are not equal, they are not equal
        if (str.str_length_ != str_length_) {
            return false;
        }

        // Compare the content of the two strings using memcmp
        return memcmp(str_, str.str_, str_length_) == 0;
    }

    /**
     * @brief
     * Equality operator.
     *
     * Compares the content of this `mgui_string` object with a C-style string.
     *
     * @param str the C-style string to compare with
     * @return `true` if the content of the `mgui_string` object and the C-style string are equal, `false` otherwise
     */
    bool operator==(const char* str) const {
        // If the lengths of the string and the `mgui_string` object are not equal, they are not equal
        if (strlen(str) != str_length_) {
            return false;
        }

        // Compare the content of the string and the `mgui_string` object using memcmp
        return memcmp(str_, str, str_length_) == 0;
    }

    /**
     * @brief Returns the first pointer of the string.
     *
     * @return const char*
     */
    const char* c_str() const { return str_; }

    /**
     * @brief returns the number of characters set.
     *
     * @return int
     */
    int length() const { return str_length_; }

private:

    /**
     * @brief
     * Get the number of characters received, allocate memory and copy them there.
     *
     * @param str char[]
     */
    inline void build(const char* str) {
        str_length_ = strlen(str);
        str_ = new char[str_length_ + 1];
        memcpy(str_, str, str_length_);
        str_[str_length_] = '\0';
    }

    /**
     * @brief
     * Initialize string settings and memory release.
     */
    inline void clear() {
        str_length_ = 0;
        if (str_) {
            delete[] str_;
            str_ = nullptr;
        }
    }

    /**
     * @brief
     * Count the number of characters
     *
     * @param str the first pointer of string.
     * @return Number of characters counted.
     */
    inline int strlen(const char* str) const {
        int len = 0;
        while (*str != '\0') {
            len++;
            str++;
        }
        return len;
    }

    char* str_;
    int str_length_;
};

template <typename K, typename V>
/**
 * @brief
 * A simple struct containing key and value
 */
class mgui_pair {
public:
    bool operator==(const mgui_pair& pair) {
        return (key == pair.key && value == pair.value);
    }

    K key;
    V value;
};

template <typename V>
/**
 * @brief
 * A map with built-in strings as keys.
 * The main purpose was to manage the created mgui objects by selecting them.
 * Originally, I was planning to create a general map, but since it was
 * necessary to divide the template into strings and strings,
 * I decided not to create one.
 */
class mgui_string_map {
public:
    /**
     * @brief Constructor.
     *
     * Initializes the map by calling the clear method.
     */
    mgui_string_map() {
        // Call clear method to initialize the map
        clear();
    }

    /**
     * @brief Destructor.
     *
     * Frees all allocated memory.
     */
    ~mgui_string_map() {
        // Free all allocated memory
        clear();
    }

    /**
     * @brief
     * Adds the specified key and value.
     * If the key exists, the value will be overwritten.
     *
     * @param key key string
     * @param value Object associated with key
     */
    void insert(const mgui_string key, V value) {
        unsigned long index = djb2_hash(key.c_str());

        mgui_pair<mgui_string, V*> pair = { key, new V(value) };

        const int count = table[index].count();
        mgui_list_node<mgui_pair<mgui_string, V*>>* node = table[index].first();
        for (int i = 0; i < count; i++) {
            if (node == nullptr) {
                break;
            }

            if (node->obj.key == key) {
                // overwrite member
                node->obj.value = pair.value;
                return;
            }

            node = node->next;
        }

        table[index].add(pair);
        counter_++;
    }

    /**
     * @brief Get the element corresponding to the set key.
     *
     * @param key Key corresponding to the value to retrieve.
     * @return V* A pointer to the element corresponding to the assigned key. If it does not exist, return nullptr.
     */
    V* get(const mgui_string key) {
        unsigned long index = djb2_hash(key.c_str());
        const int count = table[index].count();

        mgui_list_node<mgui_pair<mgui_string, V*>>* node = table[index].first();
        for (int i = 0; i < count; i++) {
            if (node == nullptr) {
                break;
            }

            if (node->obj.key == key) {
                return node->obj.value;
            }

            node = node->next;
        }

        return nullptr;
    }

    /**
     * @brief Delete the item that exists with the set key.
     *
     * @param key key string
     */
    void remove(mgui_string key) {
        unsigned long index = djb2_hash(key.c_str());
        const int count = table[index].count();

        mgui_list_node<mgui_pair<mgui_string, V*>>* node = table[index].first();
        for (int i = 0; i < count; i++) {
            if (node == nullptr) {
                break;
            }

            if (node->obj.key == key) {
                table[index].remove(node->obj);
                counter_--;
                break;
            }

            node = node->next;
        }
    }

    /**
     * @brief
     * Delete all contents of map
     */
    void clear() {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            table[i].clear();
        }
        counter_ = 0;
    }

    /**
     * @brief Get the item count
     *
     * @return int item count
     */
    inline int count() const { return counter_; }

private:

    /**
     * @brief DJB2 hash calculation
     *
     * @param data Value to calculate hash
     * @return unsigned long calculation result divided by HASH_TABLE_SIZE
     */
    inline unsigned long djb2_hash(const char* data) {
        unsigned long hash = 5381;
        int c;

        while ((c = *data++)) {
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return hash % HASH_TABLE_SIZE;
    }

    mgui_list<mgui_pair<mgui_string, V*>> table[HASH_TABLE_SIZE];
    int counter_;
};

/**
 * @brief A class representing an image.
 */
class mgui_image_property {
public:
    /**
     * @brief Construct a new mgui_image object
     *
     * @param width The width of the image
     * @param height The height of the image
     * @param resource A pointer to the image data
     */
    explicit mgui_image_property(const uint16_t width, const uint16_t height, const uint8_t* resource) {
        image_width_ = width;
        image_height_ = height;
        resource_ = resource;
    }

    /**
     * @brief Destroy the mgui_image object
     */
    ~mgui_image_property() = default;

    /**
     * @brief Get the width of the image
     *
     * @return The width of the image
     */
    inline uint16_t width() const { return image_width_; }

    /**
     * @brief Get the height of the image
     *
     * @return The height of the image
     */
    inline uint16_t height() const { return image_height_; }

    /**
     * @brief Get a pointer to the image data
     *
     * @return A pointer to the image data
     */
    inline const uint8_t* resource() const { return resource_; }

private:
    uint16_t image_width_;
    uint16_t image_height_;
    const uint8_t* resource_;
};

/**
 * @brief
 * This class is a common class that manages font resources (buffers)
 * with fixed width and height.
 * When using this class, define a class that inherits from this class
 * and set the value of each attribute.
 */
class mgui_font : public mgui_image_property {
public:

    /**
     * @brief Construct a new font property object
     *
     * @param w Font width to be set for the resource
     * @param h Font height to be set for the resource
     * @param resource Array of font data with specified width and height
     */
    explicit mgui_font(const uint16_t w, const uint16_t h, const uint8_t* resource)
     : mgui_image_property(w, h, resource) {};
    virtual ~mgui_font() {}

    /**
     * @brief
     * Function to retrieve the first array position of the string resource corresponding to the set character
     * @param c Character to search
     * @return
     * Position of the first array of font resources corresponding to the character to be searched for.
     */
    virtual int search(const char* c) = 0;
};

/**
 * @brief
 * This class is a common class that manages font resources (buffers)
 * with fixed width and height.
 * When using this class, define a class that inherits from this class
 * and set the value of each attribute.
 */
class mgui_font_w : public mgui_image_property {
public:

    /**
     * @brief Construct a new font property object
     *
     * @param w Font width to be set for the resource
     * @param h Font height to be set for the resource
     * @param resource Array of font data with specified width and height
     */
    explicit mgui_font_w(const uint16_t w, const uint16_t h, const uint8_t* resource)
     : mgui_image_property(w, h, resource) {};
    virtual ~mgui_font_w() {}

    /**
     * @brief
     * Function to retrieve the first array position of the string resource corresponding to the set character
     * @param c Character to search
     * @return
     * Position of the first array of font resources corresponding to the character to be searched for.
     */
    virtual int search(const wchar_t* c) = 0;
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
     * @param buffer Buffer to reflect drawing results
     */
    explicit mgui_draw(const uint16_t width, const uint16_t height, uint8_t *buffer){
        lcd_width_ = width;
        lcd_height_ = height;
        lcd_buffer_ = buffer;
    }

    /**
     * @brief Destroy the mgui draw object
     */
    ~mgui_draw(){}

    /**
     * @brief Drawing circle
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

    /**
     * @brief Drawing rounded rectangle
     * 
     * @param x0 x position at upper left
     * @param y0 y position at upper left
     * @param x1 x position at bottom right
     * @param y1 y position at bottom right
     * @param r rounded corner radius
     * @param fill If you want to fill the figure, set true
     * @param on if true, set 1; if false, set 0
     */
    void draw_rectangle_rounded(int x0, int y0, int x1, int y1, int r, bool fill = false, bool on = true){
        
        if(fill){
            draw_rectangle_rounded_fill(x0, y0, x1, y1, r, on);
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
            draw_pixel(px1 + x, py0 - y, on);
            draw_pixel(px1 + y, py0 - x, on);
            
            // 2
            draw_pixel(px1 + x, py1 + y, on);
            draw_pixel(px1 + y, py1 + x, on);
            
            // 3
            draw_pixel(px0 - x, py1 + y, on);
            draw_pixel(px0 - y, py1 + x, on);

            // 4
            draw_pixel(px0 - x, py0 - y, on);
            draw_pixel(px0 - y, py0 - x, on);
            if (f >= 0)
            {
                x--;
                f -= (x << 2);
            }
            y++;
            f += (y << 2) + 2;
        }

        for (int x = px0; x <=px1 ; x++) {
            draw_pixel(x, y0, on);
            draw_pixel(x, y1, on);
        }

        for(int y = py0; y <= py1; y++) {
            draw_pixel(x0, y, on);
            draw_pixel(x1, y, on);
        }
    }

    /**
     * @brief Drawing rectangle
     * 
     * @param x0 x position at upper left
     * @param y0 y position at upper left
     * @param x1 x position at bottom right
     * @param y1 y position at bottom right
     * @param fill If you want to fill the figure, set true
     * @param on if true, set 1; if false, set 0
     */
    void draw_rectangle(int x0, int y0, int x1, int y1, bool fill = false, bool on = true){
        if(fill){
            draw_rectangle_fill(x0, y0, x1, y1, on);
            return;
        }

        for (int x = x0; x <=x1 ; x++) {
            draw_pixel(x, y0, on);
            draw_pixel(x, y1, on);
        }

        for(int y = y0; y <= y1; y++) {
            draw_pixel(x0, y, on);
            draw_pixel(x1, y, on);
        }
    }

    /**
     * @brief Drawing triangle
     * 
     * @param x0 x position 0
     * @param y0 y position 0
     * @param x1 x position 1
     * @param y1 y position 1
     * @param x2 x position 2
     * @param y2 y position 2
     * @param invert if true, set 0; if false, set 1
     */
    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, bool invert = false) {
        
        draw_line(x0, y0, x1, y1, !invert);
        draw_line(x0, y0, x2, y2, !invert);
        draw_line(x1, y1, x2, y2, !invert);
    }

    /**
     * @brief Drawing straight line
     * 
     * @param x0 x position at start
     * @param y0 y position at start
     * @param x1 x position at end
     * @param y1 y position at end
     * @param on if true, set 1; if false, set 0
     */
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

    /**
     * @brief Drawing straight line other than diagonals
     * 
     * @param x0 x position at start
     * @param y0 y position at start
     * @param length straight line length
     * @param on if true, set 1; if false, set 0
     * @param direction Direction for drawing a straight line
     */
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
     * The point outside the set height and width are ignored.
     * 
     * @param x x position
     * @param y y position
     * @param on 
     * If true, the bit at the position corresponding to (x,y) is set to 1;
     * if false, it is set to 0
     */
    void draw_pixel(int x, int y, bool on){
        if (x < lcd_width_ && y < lcd_height_) {
            // Calculate the byte index.
            int byte_idx = (y >> 3) * lcd_width_ + x;
            uint8_t bit_idx = (1 << (y % 8));

            // Set or clear the bit at the specified index.
            lcd_buffer_[byte_idx]
                = on ? (lcd_buffer_[byte_idx] | bit_idx)
                        : (lcd_buffer_[byte_idx] & ~bit_idx);
        }
    }

    /**
     * @brief 
     * Draw an arbitrary character from mgui_font resource
     * 
     * @param font 
     * @param x x position in the upper left corner of the text display
     * @param y y position in the upper left corner of the text display
     * @param index  Starting position of font resource loading (returned value of mgui_font::search)
     * @param invert If true, invert buffer color of font resource
     * @param font_start_x Starting pixel position x-coordinate of font resource setting to be displayed
     * @param font_start_y Starting pixel position y-coordinate of font resource setting to be displayed
     * @param font_end_x 
     * End pixel position x-coordinate of the font resource setting to be displayed.
     * (If 0, the size of the font resource setting is applied.)
     * @param font_end_y
     * End pixel position y-coordinate of the font resource setting to be displayed. 
     * (If 0, the size of the font resource setting is applied.)
     */
    inline void draw_char(const mgui_font *font,
                          const int& x,
                          const int& y,
                          const int& index,
                          bool invert = false,
                          const int& font_start_x = 0,
                          const int& font_start_y = 0,
                          const int& font_end_x = 0,
                          const int& font_end_y = 0) {
        
        int x_end = ((font_end_x == 0)? font->width() : font_end_x);
        int y_end = ((font_end_y == 0)? font->height() : font_end_y);

        for (int y1 = font_start_y; y1 < y_end; y1++) {
            for (int x1 = font_start_x; x1 < x_end; x1++) {
                int pos = index + y1 / 8 * font->width() + x1;
                bool checkbit = check_bit_on(x1, y1, font->resource()[pos]);
                if (checkbit) {
                    draw_pixel(x + x1, y + y1, (invert) ? !checkbit : checkbit);
                }
            }
        }
    }

    /**
     * @brief Draw an image on the LCD buffer.
     * 
     * @param image A pointer to the image property object.
     * @param x The x-coordinate of the upper left corner of the image.
     * @param y The y-coordinate of the upper left corner of the image.
     * @param invert If true, the buffer color of the image will be inverted. Default is false.
     */
    inline void draw_image(const mgui_image_property *image,
                          const int& x,
                          const int& y,
                          bool invert = false) {
        
        // Iterate over the image height
        for (int y1 = 0; y1 < image->height(); y1++) {
            // Iterate over the image width
            for (int x1 = 0; x1 < image->width(); x1++) {
                // Calculate the position in the image resource
                int pos = y1 / 8 * image->width() + x1;
                // Check if the bit at the current position is set
                bool checkbit = check_bit_on(x1, y1, image->resource()[pos]);
                // Draw the pixel on the LCD buffer if the bit is set
                if (checkbit) {
                    draw_pixel(x + x1, y + y1, (invert) ? !checkbit : checkbit);
                }
            }
        }
    }

    /**
     * @brief It returns generated buffer.
     * 
     * @return uint8_t* 
     */
    uint8_t * lcd() { return lcd_buffer_; }

private:
    
    /**
     * @brief drawing filled circle
     *
     * @param x0 center point of X
     * @param y0 center point of Y
     * @param r  radius
     */
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
    inline void draw_rectangle_rounded_fill(int x0, int y0, int x1, int y1, int r, bool on){
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
                draw_pixel(px1 + xd, py0 - y, on);
                draw_pixel(px1 + y, py0 - xd, on);
                
                // 2
                draw_pixel(px1 + xd, py1 + y, on);
                draw_pixel(px1 + y, py1 + xd, on);
                
                // 3
                draw_pixel(px0 - xd, py1 + y, on);
                draw_pixel(px0 - y, py1 + xd, on);

                // 4
                draw_pixel(px0 - xd, py0 - y, on);
                draw_pixel(px0 - y, py0 - xd, on);
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
                draw_pixel(x0 + dx, dy, on);
                draw_pixel(px1 + dx, dy, on);
            }
        }

        draw_rectangle_fill(px0, y0, px1, y1, on);
    }

    /**
     * @brief Draw a filled rectangle
     * 
     * @param x0 x position at upper left
     * @param y0 y position at upper left
     * @param x1 x position at bottom right
     * @param y1 y position at bottom right
     * @param on if true, set 1; if false, set 0
     */
    inline void draw_rectangle_fill(int x0, int y0, int x1, int y1, bool on){
        for(int x = x0; x <= x1; x++){
            for(int y = y0; y <= y1; y++){
                draw_pixel(x, y, on);
            }
        }
    }

    /**
     * @brief
     * Check whether the data for the specified image display position is turned on.
     * TODO: Excessive idx calculation
     * 
     * @param x x position of image
     * @param y y position of image
     * @param value a value containing the bits of the specified image
    */
    inline bool check_bit_on(int x, int y, uint8_t value) {
       uint8_t bit_idx = (1 << (7 - y % 8));
       bool on = (value & bit_idx) & bit_idx;
       return on;
    }

    uint8_t *lcd_buffer_;
    int lcd_width_;
    int lcd_height_;
};

/**
 * @brief 
 * Common objects to reflect the set state
 */
class mgui_object {
public:
    /**
     * @brief Specify a unique object name in the inherited class
     * @return Name of object registered in enum
     */
    virtual mgui_object_type type() const = 0;

    /**
     * @brief 
     * The inherited class configures the drawing operations required by that object.
     * This is executed every time the mgi::update_lcd() function is called.
     * 
     * @param draw Objects that provide a method for drawing
     * @param input_state Current input state
     * @param current_group The name of the group that the object is currently being drawn in
     */
    virtual void update(mgui_draw* draw, mgui_input_state *input_state, mgui_string* current_group) = 0;
};

/**
 * @brief
 * - Register and update device inputs together.
 */
class mgui_input {
public:
    mgui_input() {
        input_data_ = nullptr;
    }
    ~mgui_input() {
        clear_data();
    }

    /**
     * @brief Add callbacks to update input status.
     * 
     * @param input_read_function 
     * A Function to execute a read process of SDK-specific inputs using GPIO or
     * other communication methods and store a value reflecting the result of
     * the read process in the argument mgui_input_state.
     */
    inline void add(void (*input_read_function)(mgui_input_state *result)) {
        function_list_.add(input_read_function);
        clear_data();
        input_data_ = new mgui_input_state[function_list_.count()];
    }

    /**
     * @brief Deletes a callback that has been set.
     * 
     * @param index 
     * Setup number of the callback. Since callbacks are stored in the order 
     * in which add is executed, the appropriate index depends on the order 
     * in which the callbacks are set. The setting order starts from 0.
     */
    inline void remove(int index) {

        mgui_list_node<void (*)(mgui_input_state* result)>* node = function_list_.first();

        for (int i = 0; i < index; i++) {
            node = node->next;
        }

        function_list_.remove(node->obj);
        clear_data();
        input_data_ = new mgui_input_state[function_list_.count()];
    }

    /**
     * @brief 
     * All registered callbacks are executed. mgui_input_type is initially set to Single.
     */
    inline void update() {
        mgui_list_node<void (*)(mgui_input_state* result)>* node = function_list_.first();

        int counter = 0;
        while (node != nullptr) {
            node->obj(&input_data_[counter]);
            node = node->next;
            counter++;
        }
    }

    mgui_input_state* get_input_result() { return input_data_; }

private:
    /**
     * @brief Deletes all dynamically allocated memory used by the input_data_.
     */
    void clear_data() {
        if (input_data_) {
            delete[] input_data_;
            input_data_ = nullptr;
        }
    }

    mgui_list<void (*)(mgui_input_state* result)> function_list_;
    mgui_input_state *input_data_;
};


/**
 * @brief 
 * Adds/deletes/updates drawing objects and provides drawing status.
 */
class mgui {
public:
    /**
     * @brief Construct a new mgui draw object
     *
     * @param width Target screen width
     * @param height Target screen height
     */
    explicit mgui(const uint8_t width, const uint8_t height) {
        buffer_size = width * (height >> 3);
        lcd_buffer = new uint8_t[buffer_size]();
        memset(lcd_buffer, 0, buffer_size);
        draw_ = new mgui_draw(width, height, lcd_buffer);
        input_ = nullptr;
    }

    ~mgui() {
        delete draw_;
        delete[] lcd_buffer;
    }

    inline bool operator==(mgui& gui) {
        if (buffer_size != gui.buffer_size) return false;
        if (list.count() != gui.list.count()) return false;
        int counter = list.count();
        for (int i = 0; i < counter; i++) {
            if (list.get(i)->type() != gui.list.get(i)->type()) {
                return false;
            }
        }

        return true;
    }

    inline void set_input(mgui_input* input){
        input_ = input;
    }

    inline void add(mgui_object *item){
        list.add(item);
    }

    inline void remove(mgui_object *item){
        list.remove(item);
    }

    inline void clear(){
        list.clear();
    }

    /**
     * @brief 
     * Update screen drawing
     */
    inline void update_lcd() {
        // update input state
        mgui_input_state* state = nullptr;
        if(input_ != nullptr){
            input_->update();
            state = input_->get_input_result();
        }

        mgui_list_node<mgui_object*>* node = list.first();

        // clear buffer
        memset(lcd_buffer, 0, buffer_size);

        // set settings
        while(node != nullptr){
            node->obj->update(draw_, state, nullptr);
            node = node->next;
        }
    }

    /**
     * @brief Get the buffer for the set screen size
     * 
     * @return uint8_t* A pointer to a screen buffer.
     */
    inline uint8_t *lcd() { return lcd_buffer; }

private:
    mgui_draw* draw_;
    mgui_input* input_;
    mgui_list<mgui_object*> list;
    uint8_t* lcd_buffer;
    int buffer_size;
};

/**
 * @brief
 * Adds/deletes/updates drawing objects and provides drawing status.
 */
class mgui_multi {
public:
    /**
     * @brief Construct a new mgui draw object
     *
     * @param width Target screen width
     * @param height Target screen height
     */
    explicit mgui_multi(const uint8_t width, const uint8_t height) {
        buffer_size = width * (height >> 3);

        lcd_buffer = new uint8_t[buffer_size]();
        memset(lcd_buffer, 0, buffer_size);
        
        draw_ = new mgui_draw(width, height, lcd_buffer);
    }

    ~mgui_multi() {
        delete draw_;
        delete[] lcd_buffer;
    }

    inline void add(const char *group_name, mgui_object* item) {
        mgui_list<mgui_object*>* list =  map.get(group_name);
        if (list == nullptr) {
            mgui_list<mgui_object*> new_list;
            new_list.add(item);
            map.insert(group_name, new_list);
            selected_ = group_name;
            return;
        }

        list->add(item);
    }

    inline void remove(const char* group_name, mgui_object* item) {
        mgui_list<mgui_object*>* list = map.get(group_name);
        if (list != nullptr) {
            list->remove(item);
        }
    }

    inline void clear(const char* group_name) {
        mgui_list<mgui_object*>* list = map.get(group_name);
        if (list != nullptr) {
            list->clear();
            map.remove(group_name);
        }
    }

    inline bool select(const char* group_name) {
        mgui_list<mgui_object*>* list = map.get(group_name);
        if (list != nullptr) {
            selected_ = group_name;
            return true;
        }

        return false;
    }

    /**
     * @brief
     * Update screen drawing
     */
    inline void update_lcd() {
        // update input state
        mgui_input_state* state = nullptr;
        input_.update();
        state = input_.get_input_result();

        mgui_list<mgui_object*>* list = map.get(selected_);
        if (list != nullptr) {
            mgui_list_node<mgui_object*>* node = list->first();

            // clear buffer
            memset(lcd_buffer, 0, buffer_size);

            // set settings
            while (node != nullptr) {
                node->obj->update(draw_, state, &selected_);
                node = node->next;
            }
        }
    }

    /**
     * @brief Get the buffer for the set screen size
     *
     * @return uint8_t* A pointer to a screen buffer.
     */
    inline uint8_t* lcd() { return lcd_buffer; }

    inline mgui_input* input() { return &input_; }

private:
    mgui_draw* draw_;
    mgui_input input_;
    mgui_string_map<mgui_list<mgui_object*>> map;
    uint8_t* lcd_buffer;
    int buffer_size;
    mgui_string selected_;
};

class mgui_padding_property {
public:
    /**
     * @brief Construct a new mgui_padding_property object with default padding values.
     *
     * Sets the left, up, right and down padding values to 0.
     */
    mgui_padding_property() {
        // Set the default down padding value to 0.
        down_ = 0;

        // Set the default left padding value to 0.
        left_ = 0;

        // Set the default up padding value to 0.
        up_ = 0;

        // Set the default right padding value to 0.
        right_ = 0;
    }

    /**
     * @brief Overload of the equality operator for mgui_padding_property objects.
     *
     * This function checks if the left, up, right and down padding values of two
     * mgui_padding_property objects are equal.
     *
     * @param other The mgui_padding_property object to compare against.
     * @return true If all padding values are equal.
     * @return false If any of the padding values are not equal.
     */
    bool operator==(const mgui_padding_property& other) const
    {
        return left_ == other.left_ && up_ == other.up_ && right_ == other.right_ && down_ == other.down_;
    }

    /**
     * @brief Overload of the assignment operator for mgui_padding_property objects.
     *
     * This function copies the values of the left, up, right and down padding properties
     * from the provided object to the current object.
     *
     * @param other The mgui_padding_property object to copy from.
     * @return Reference to the current object.
     */
    mgui_padding_property operator=(const mgui_padding_property& other) noexcept {
        if (this != &other) {
            this->left_ = other.left_;
            this->up_ = other.up_;
            this->right_ = other.right_;
            this->down_ = other.down_;
        }
        return *this;
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

/**
 * @brief The mgui_text_property class represents a property of text with a font.
 * This class is used to define a text with a font in mGUI.
 */
class mgui_text_property {
public:
    /**
     * @brief Construct a new mgui text property
     *
     * @param font The inherited class of mgui_font_property
     * @param text Text to display
     */
    explicit mgui_text_property(mgui_font* font, const char* text = nullptr) {
        font_ = font;
        text_ = nullptr;
        text_index_ = nullptr;
        text_length = 0;

        if (text) {
            set_text(text);
        }
    }

    /**
     * @brief Destructor of mgui_text_property.
     *
     * This destructor is responsible for releasing the memory allocated for the text.
     */
    virtual ~mgui_text_property() {
        // Call the clear function to release the allocated memory
        clear();
    }

    mgui_text_property operator=(const mgui_text_property& other) noexcept {
        if (this != &other) {
            this->clear();
            this->font_ = other.font_;
            this->text_ = other.text_;
            this->text_index_ = other.text_index_;
            this->text_length = other.text_length;
        }
        return *this;
    }

    bool operator==(const mgui_text_property& other) {
        return this->font_ == other.font_
            && this->text_ == other.text_
            && this->text_index_ == other.text_index_
            && this->text_length == other.text_length;
    }

    inline char* get_text() const { return text_; }
    inline void set_text(const char* text) {
        clear();
        text_length = strlen(text);
        text_ = new char[text_length + 1];
        text_index_ = new int[text_length];
        memcpy(text_, text, text_length);
        text_[text_length] = '\0';

        for (int i = 0; i < text_length; i++) {
            char c = *(text + i);
            text_index_[i] = font_->search(&c);
        }
    }
    inline int get_text_index(const int index) const { return text_index_[index]; }
    inline int get_text_length() const { return text_length; }
    inline mgui_font* get_font() const { return font_; }

private:
    inline void clear() {
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

    mgui_font* font_;
    char* text_;
    int* text_index_;
    int text_length;
};

/**
 * @brief It draws pixel at specified position.
 * 
 */
class mgui_pixel : mgui_object {
public:
    explicit mgui_pixel(
        const uint16_t& x = 0,
        const uint16_t& y = 0,
        const bool& on = false,
        const bool& invert = false) {
        x_ = x;
        y_ = y;
        on_ = on;
        invert_ = invert;
    }
    virtual ~mgui_pixel(){}

    mgui_pixel operator=(const mgui_pixel& other) noexcept {
        if (this != &other) {
            this->x_ = other.x_;
            this->y_ = other.y_;
            this->on_ = other.on_;
            this->invert_ = other.invert_;
        }
        return *this;
    }

    mgui_object_type type() const { return mgui_object_type::Pixel; }

    inline uint16_t x() const { return x_; }
    inline void set_x(uint16_t x) { x_ = x; }

    inline uint16_t y() const { return y_; }
    inline void set_y(uint16_t y) { y_ = y; }

    inline bool on() const { return on_; }
    inline void set_on(bool on) { on_ = on; }

    inline bool invert() const { return invert_; }
    inline void set_invert(bool invert) { invert_ = invert; }

    void update(mgui_draw* draw, mgui_input_state*, mgui_string*) {
        if(invert_) {
            draw->draw_pixel(x_, y_, !on_);
            return;
        }

        draw->draw_pixel(x_, y_, on_);
    }

private:
    uint16_t x_;
    uint16_t y_;
    bool on_;
    bool invert_;
};

/**
 * @brief It draws straight line.
 *
 */
class mgui_line : mgui_object {
public:
    mgui_line() {
        x0_ = 0;
        y0_ = 0;
        x1_ = 0;
        y1_ = 0;
        invert_ = 0;
    }
    virtual ~mgui_line() {}

    mgui_line operator=(const mgui_line& other) noexcept {
        if (this != &other) {
            this->x0_ = other.x0_;
            this->y0_ = other.y0_;
            this->x1_ = other.x1_;
            this->y1_ = other.y1_;
            this->invert_ = other.invert_;
        }
        return *this;
    }

    mgui_object_type type() const { return mgui_object_type::Line; }

    inline uint16_t x0() const { return x0_; }
    inline void set_x0(uint16_t x0) { x0_ = x0; }

    inline uint16_t y0() const { return y0_; }
    inline void set_y0(uint16_t y0) { y0_ = y0; }

    inline uint16_t x1() const { return x1_; }
    inline void set_x1(uint16_t x1) { x1_ = x1; }

    inline uint16_t y1() const { return y1_; }
    inline void set_y1(uint16_t y1) { y1_ = y1; }

    inline uint8_t invert() const { return invert_; }
    inline void set_invert(uint8_t invert) { invert_ = invert; }

    void update(mgui_draw* draw, mgui_input_state*, mgui_string*) {
        draw->draw_line(x0_, y0_, x1_, y1_, !invert_);
    }

private:
    uint16_t x0_;
    uint16_t y0_;
    uint16_t x1_;
    uint16_t y1_;
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

    mgui_circle operator=(const mgui_circle& other) noexcept {
        if (this != &other) {
            this->x_ = other.x_;
            this->y_ = other.y_;
            this->r_ = other.r_;
            this->fill_ = other.fill_;
        }
        return *this;
    }

    mgui_object_type type() const { return mgui_object_type::Circle; }

    inline uint16_t x() const { return x_; }
    inline void set_x(uint16_t x) { x_ = x; }

    inline uint16_t y() const { return y_; }
    inline void set_y(uint16_t y) { y_ = y; }

    inline uint16_t radius() const { return r_; }
    inline void set_radius(uint16_t r) { r_ = r; }

    inline uint8_t fill() const { return fill_; }
    inline void set_fill(uint8_t fill) { fill_ = fill; }

    void update(mgui_draw* draw, mgui_input_state*, mgui_string*) {
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
class mgui_rectangle : mgui_object {
public:
    mgui_rectangle() {
        x_ = 0;
        y_ = 0;
        width_ = 0;
        height_ = 0;
        r_ = 0;
        fill_ = false;
        invert_ = false;
    }
    virtual ~mgui_rectangle() {}

    mgui_rectangle operator=(const mgui_rectangle& other) noexcept {
        if (this != &other) {
            this->x_ = other.x_;
            this->y_ = other.y_;
            this->width_ = other.width_;
            this->height_ = other.height_;
            this->r_ = other.r_;
            this->fill_ = other.fill_;
            this->invert_ = other.invert_;
        }
        return *this;
    }

    mgui_object_type type() const { return mgui_object_type::Rectangle; }

    void update(mgui_draw* draw, mgui_input_state*, mgui_string*) {

        if (r_ > 0) {
            draw->draw_rectangle_rounded(
                x_, y_, x_ + width_ - 1, y_ + height_ - 1, r_, fill_, !invert_);
            return;
        }

        draw->draw_rectangle(x_, y_, x_ + width_ - 1, y_ + height_ - 1, fill_, !invert_);
    }

    inline uint16_t radius() const { return r_; }
    inline void set_radius(uint16_t r) { r_ = r; }

    inline bool fill() const { return fill_; }
    inline void set_fill(bool fill) { fill_ = fill; }

    inline uint16_t width() const { return width_; }
    inline void set_width(uint16_t width) { width_ = width; }

    inline uint16_t height() const { return height_; }
    inline void set_height(uint16_t height) { height_ = height; }

    inline uint16_t x() const { return x_; }
    inline void set_x(uint16_t x) { x_ = x; }

    inline uint16_t y() const { return y_; }
    inline void set_y(uint16_t y) { y_ = y; }

    inline bool invert() const { return invert_; }
    inline void set_invert(bool invert) { invert_ = invert; }

private:
    uint16_t width_;
    uint16_t height_;
    uint16_t x_;
    uint16_t y_;
    uint16_t r_;
    bool fill_;
    bool invert_;
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

    mgui_triangle operator=(const mgui_triangle& other) noexcept {
        if (this != &other) {
            this->x0_ = other.x0_;
            this->y0_ = other.y0_;
            this->x1_ = other.x1_;
            this->y1_ = other.y1_;
            this->x2_ = other.x2_;
            this->y2_ = other.y2_;
            this->invert_ = other.invert_;
        }
        return *this;
    }

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

    void update(mgui_draw* draw, mgui_input_state*, mgui_string*) {
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
 * @brief It draws image.
 */
class mgui_image : mgui_object {
public:
    /**
     * @brief Construct a new mgui text object
     * 
     * @param font The inherited class of mgui_font_property
     * @param text Text to display
     * @param x X position in the upper left corner where mgui_font_property resource is displayed
     * @param y Y position in the upper left corner where mgui_font_property resource is displayed
     */
    explicit mgui_image(mgui_image_property *image, uint16_t x = 0, uint16_t y = 0) {
        x_ = x;
        y_ = y;
        invert_ = false;
        image_property_ = image;
    }

    virtual ~mgui_image() {}

    mgui_image operator=(const mgui_image& other) noexcept {
        if (this != &other) {
            this->x_ = other.x_;
            this->y_ = other.y_;
            this->invert_ = other.invert_;
            this->image_property_ = other.image_property_;
        }
        return *this;
    }

    bool operator==(const mgui_image& other) {
        return this->x_ == other.x_
            && this->y_ == other.y_
            && this->invert_ == other.invert_
            && this->image_property_ == other.image_property_;
    }

    mgui_object_type type() const { return mgui_object_type::Image; }
    
    void update(mgui_draw* draw, mgui_input_state*, mgui_string*) {
        draw->draw_image(image_property_, x_, y_, invert_);
    }

    inline uint16_t width() const { return image_property_->width(); }
    inline uint16_t height() const { return image_property_->height(); }

    inline uint16_t x() const { return x_; }
    inline void set_x(uint16_t x) { x_ = x; }

    inline uint16_t y() const { return y_; }
    inline void set_y(uint16_t y) { y_ = y; }

    inline bool invert() const { return invert_; }
    inline void set_invert(bool invert) { invert_ = invert; }

private:

    uint16_t x_;
    uint16_t y_;
    bool invert_;
    mgui_image_property *image_property_;
};

/**
 * @brief The mgui_text class is a UI object that displays text.
 * 
 * This class is used to display text on the screen.
 * The text is displayed using the mgui_font_property class.
 * The text can be displayed at a specified position on the screen. 
 * The position is specified by the x and y coordinates.
 */
class mgui_text : mgui_object {
public:
    /**
     * @brief Construct a new mgui text object
     * 
     * @param font The inherited class of mgui_font_property
     * @param text Text to display
     * @param x X position in the upper left corner where mgui_font_property resource is displayed
     * @param y Y position in the upper left corner where mgui_font_property resource is displayed
     */
    explicit mgui_text(mgui_font *font, const char* text = nullptr, uint16_t x = 0, uint16_t y = 0) {
        text_property_ = new mgui_text_property(font, text);
        set_x(x);
        set_y(y);
        if (text) {
            set_text(text);
        }
        view_width_ = 0;
        view_height_ = 0;
        move_ = false;
        moved_amount_of_movement_ = 0;
        moved_per_frame_ = 0;
        frame_counter_ = 0;
        invert_ = false;
        moved_x_counter_ = 0;
    }

    ~mgui_text() {
        delete text_property_;
    }

    mgui_text operator=(const mgui_text& other) noexcept {
        if (this != &other) {
            this->x_ = other.x_;
            this->y_ = other.y_;
            this->text_width_ = other.text_width_;
            this->text_height_ = other.text_height_;
            this->view_width_ = other.view_width_;
            this->view_height_ = other.view_height_;
            this->moved_amount_of_movement_ = other.moved_amount_of_movement_;
            this->moved_per_frame_ = other.moved_per_frame_;
            this->invert_ = other.invert_;
            this->frame_counter_ = other.frame_counter_;
            this->text_property_ = other.text_property_;
        }
        return *this;
    }

    bool operator==(const mgui_text& other) {
        return this->x_ == other.x_
            && this->y_ == other.y_
            && this->text_width_ == other.text_width_
            && this->text_height_ == other.text_height_
            && this->view_width_ == other.view_width_
            && this->view_height_ == other.view_height_
            && this->moved_amount_of_movement_ == other.moved_amount_of_movement_
            && this->moved_per_frame_ == other.moved_per_frame_
            && this->invert_ == other.invert_
            && this->text_property_ == other.text_property_;
    }

    mgui_object_type type() const { return mgui_object_type::Text; }
    
    void update(mgui_draw* draw, mgui_input_state*, mgui_string*) {

        if(move_ && 0 < view_width_ && view_width_ < text_width_) {
            
            int first_char = moved_x_counter_ / font()->width();
            int first_pos = moved_x_counter_ % font()->width();
            int end_char = view_width_ / font()->width() + first_char;
            int end_pos = view_width_ % font()->width();

            for(int i = first_char; i < end_char; i++) {
                int x0 = x_ + font()->width() * (i - first_char);
                if(i == first_char){
                    // first character
                    draw->draw_char(font(), x0, y_, text_property_->get_text_index(i), invert_, first_pos);
                } else if(i == end_char) {
                    // last character
                    draw->draw_char(font(), x0, y_, text_property_->get_text_index(i), invert_, 0, 0, end_pos);

                } else {
                    draw->draw_char(font(), x0, y_, text_property_->get_text_index(i), invert_);
                }
            }

            if(frame_counter_ == moved_per_frame_){
                moved_x_counter_+=moved_amount_of_movement_;            
                if((text_width_- moved_x_counter_) < view_width_) {
                    moved_x_counter_ = 0;
                }
                frame_counter_ = 0;
            }

            frame_counter_++;

        } else {
            int view_length = view_width_ / font()->width();
            if(view_length > 0){
                view_length = text_property_->get_text_length() < view_length ? text_property_->get_text_length() : view_length;
            }else{
                view_length = text_property_->get_text_length();
            }

            for(int i = 0; i < view_length; i++) {
                int x0 = x_ + font()->width() * i;
                draw->draw_char(font(), x0, y_, text_property_->get_text_index(i), invert_);
            }
        }
    }

    inline char* text() const { return text_property_->get_text(); }
    inline void set_text(const char* text) {
        text_property_->set_text(text);
        text_width_ = font()->width() * text_property_->get_text_length();
        text_height_ = font()->height();
    }

    inline int text_length() { return text_property_->get_text_length(); }

    inline uint16_t text_width() const { return text_width_; }
    inline uint16_t text_height() const { return text_height_; }

    inline uint16_t view_width() const { return view_width_; }
    inline void set_view_width(uint16_t view_width) { 
        view_width_ = view_width;
    }

    inline uint16_t view_height() const { return view_height_; }
    inline void set_view_height(uint16_t view_height) { 
        view_height_ = view_height;
    }

    inline uint16_t x() const { return x_; }
    inline void set_x(uint16_t x) { x_ = x; }

    inline uint16_t y() const { return y_; }
    inline void set_y(uint16_t y) { y_ = y; }

    inline mgui_font* font() const { return text_property_->get_font(); }

    inline bool invert() const { return invert_; }
    inline void set_invert(bool invert) { invert_ = invert; }

    inline bool move() const { return move_; }

    inline void set_move(bool move, uint8_t per_frame = 1, uint8_t amount_of_movement = 1) {  
        move_ = move;
        moved_per_frame_ = per_frame;
        moved_amount_of_movement_ = amount_of_movement;
        if(!move) moved_x_counter_ = 0;
    }

private:
    uint16_t moved_x_counter_;

    uint16_t text_width_;
    uint16_t text_height_;
    uint16_t view_width_;
    uint16_t view_height_;
    uint16_t x_;
    uint16_t y_;
    bool invert_;
    bool move_;
    uint8_t moved_per_frame_;
    uint8_t moved_amount_of_movement_;
    int frame_counter_;
    mgui_text_property *text_property_;
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
         input_event_callback_ = nullptr;

     };
     virtual ~mgui_button(){};

     mgui_button operator=(const mgui_button& other) noexcept {
        if (this != &other) {
            this->text_ = other.text_;
            this->text_rel_x_ = other.text_rel_x_;
            this->text_rel_y_ = other.text_rel_y_;
            this->padding_ = other.padding_;
            this->rect_ = other.rect_;
            this->input_event_callback_ = other.input_event_callback_;
        }
        return *this;
    }

     mgui_object_type type() const { return mgui_object_type::Button; };

     void update(mgui_draw* draw, mgui_input_state *input, mgui_string* current_group) {
         bool is_filled = get_on_selected()? !get_on_press() : get_on_press();
         
         if (input_event_callback_ && get_on_selected()) {
             input_event_callback_(this, input, current_group);
         }
                  
         rect_.set_fill(is_filled);
         rect_.update(draw, input, current_group);
         
         if (text_) {
             text_->set_invert(is_filled);
             text_->update(draw, input, current_group);
         }
     };

     inline mgui_text* text() const { return text_; }
     inline void set_text(mgui_text* text, uint16_t text_rel_x = 0, uint16_t text_rel_y = 0) {
         text_ = text;
         text_rel_x_ = text_rel_x;
         text_rel_y_ = text_rel_y;
         update_property();
     }

     /**
      * @brief Set the input event handler object
      * 
      * @param event_callback 
      * Implement functions to change the state of the gui and operate other non-gui functions
      * using functions set in mgui_button using the value of mgui_input_state
      * (the result of input reading set in mgui_input). 
      * The set function is called each time before drawing is updated.
      */
     inline void set_input_event_handler(
         void (*event_callback)(const mgui_button* sender, const mgui_input_state state[], mgui_string* current_group)) {
         input_event_callback_ = event_callback;
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
     inline void set_width(uint16_t width) {
         rect_.set_width(width);
         if (text_) {
             text_->set_view_width(width);
             update_property();
         }
     }

     inline uint16_t height() const { return rect_.height(); }
     inline void set_height(uint16_t height) {
         rect_.set_height(height);
         if (text_) {
             text_->set_view_height(height);
             update_property();
         }
     }

     inline uint16_t radius() const { return rect_.radius(); }
     inline void set_radius(uint16_t r) { rect_.set_radius(r); }

 private:
     inline void update_property() {
         uint16_t rect_w = text_->view_width() > 0 ? text_->view_width() : text_->text_width();
         uint16_t rect_h = text_->view_height() > 0 ? text_->view_height() : text_->text_height();

         rect_w += padding_.left() + padding_.right();
         rect_h += padding_.up() + padding_.down();

         uint16_t rx = rect_.x();
         uint16_t ry = rect_.y();
         rect_.set_x(rx);
         rect_.set_y(ry);
         rect_.set_width(rect_w == 0 ? rect_.width() : rect_w);
         rect_.set_height(rect_h == 0 ? rect_.height() : rect_h);

         text_->set_x(rx + text_rel_x_ + padding_.left());
         text_->set_y(ry + text_rel_y_ + padding_.up());
     }

     void (*input_event_callback_)(const mgui_button* sender, const mgui_input_state state[], mgui_string* current_group);
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

    mgui_list<mgui_menu_item*> menu_item_;
    uint16_t selected_index_;
};

/**
 * @brief Type of menu item
 */
enum class mgui_menu_item_type {
    /** 
     * @brief No operation
     */
    None,
    /** 
     * @brief Check box
     */
    Check,
    /** 
     * @brief Menu item with a child menu
     */
    Menu,
    /** 
     * @brief Item to return to the parent menu
     */
    ReturnToParent
};

class mgui_menu_item : public mgui_core_ui {
public:
    explicit mgui_menu_item(mgui_text* text = nullptr) : mgui_core_ui() {
        text_ = text;
        text_rel_x_ = 0;
        text_rel_y_ = 0;
        child_menu_ = nullptr;
        is_checked_ = false;
        is_return_menu_ = false;
        previous_on_press_ = false;
        item_type_ = mgui_menu_item_type::None;
        rect_ = mgui_rectangle();
        input_event_callback_ = nullptr;
    }

    virtual ~mgui_menu_item(){}

    mgui_menu_item operator=(const mgui_menu_item& other) noexcept {
        if (this != &other) {
            this->text_ = other.text_;
            this->text_rel_x_ = other.text_rel_x_;
            this->text_rel_y_ = other.text_rel_y_;
            this->child_menu_ = other.child_menu_;
            this->is_checked_ = other.is_checked_;
            this->is_return_menu_ = other.is_return_menu_;
            this->previous_on_press_ = other.previous_on_press_;
            this->item_type_ = other.item_type_;
            this->rect_ = other.rect_;
            this->input_event_callback_ = other.input_event_callback_;
            this->check_rect_outer = other.check_rect_outer;
            this->check_rect_inner = other.check_rect_inner;
            this->menu_right_arrow_up = other.menu_right_arrow_up;
            this->menu_right_arrow_down = other.menu_right_arrow_down;
            this->menu_left_arrow_up = other.menu_left_arrow_up;
            this->menu_left_arrow_down = other.menu_left_arrow_down;
        }
        return *this;
    }

    mgui_object_type type() const { return mgui_object_type::MenuItem; }

    mgui_menu_item_type item_type() const { return item_type_; }

    void update(mgui_draw* draw, mgui_input_state* input, mgui_string* current_group) {
        if (input_event_callback_) {
            input_event_callback_(this, input, current_group);
        }

        bool focus = get_on_selected()? !get_on_press() : get_on_press();

        if (focus) {
            rect_.update(draw, input, current_group);
        }
 
        if (text_) {
            text_->set_invert(focus);
            text_->set_move(focus);
            text_->update(draw, input, current_group);
        }

        switch (item_type_) {
        case mgui_menu_item_type::Check:
            if(!previous_on_press_ && get_on_press()){
                is_checked_ = !is_checked_;
            }
            previous_on_press_ = get_on_press();
            draw_check_box(draw, input, current_group, focus);
            break;
        case mgui_menu_item_type::Menu:
            draw_menu_guide(draw, input, current_group, focus);
            break;
        case mgui_menu_item_type::ReturnToParent:
            draw_return_menu_guide(draw, input, current_group, focus);
            break;
        default:
            break;
        }
    }

    inline uint16_t radius() const { return rect_.radius(); }
    inline void set_radius(uint16_t radius) { rect_.set_radius(radius); }

    /**
     * @brief
     * Calculate and set the menu display position based on the number of displays on
     * the screen and the item setting index.
     * 
     * @remarks
     * This function is automatically assigned by mgui_menu and is not used directly
     * 
     * @param index Setting display position in item
     * @param item_view_count number of items on screen
     * @param screen_width
     * @param screen_height
     */
    inline void _set_draw_position(uint16_t index, uint16_t item_view_count, uint16_t screen_width, uint16_t screen_height) {
        uint16_t h = screen_height / item_view_count;
        rect_.set_x(0);
        rect_.set_y(h * index);
        rect_.set_width(screen_width);
        rect_.set_height(h);
        rect_.set_fill(true);

        // check box
        check_rect_outer.set_height(h - 2);
        check_rect_outer.set_width(h - 2);
        check_rect_outer.set_x(screen_width - h + 1);
        check_rect_outer.set_y(h * index + 1);

        check_rect_inner.set_height(h - 6);
        check_rect_inner.set_width(h - 6);
        check_rect_inner.set_x(screen_width - h + 3);
        check_rect_inner.set_y(h * index + 3);
        check_rect_inner.set_fill(true);

        // menu
        constexpr int margin = 5;
        menu_right_arrow_up.set_x0(screen_width - h + margin);
        menu_right_arrow_up.set_y0(h * index + margin);
        menu_right_arrow_up.set_x1(screen_width - margin);
        menu_right_arrow_up.set_y1(h * index + (h >> 1) + 1);

        menu_right_arrow_down.set_x0(screen_width - h + margin);
        menu_right_arrow_down.set_y0(h * (index + 1) - margin);
        menu_right_arrow_down.set_x1(screen_width - margin);
        menu_right_arrow_down.set_y1(h * index + (h >> 1));

        // return_menu_
        menu_left_arrow_up.set_x0(h - margin);
        menu_left_arrow_up.set_y0(h * index + margin);
        menu_left_arrow_up.set_x1(margin);
        menu_left_arrow_up.set_y1(h * index + (h >> 1) + 1);

        menu_left_arrow_down.set_x0(h - margin);
        menu_left_arrow_down.set_y0(h * (index + 1) - margin);
        menu_left_arrow_down.set_x1(margin);
        menu_left_arrow_down.set_y1(h * index + (h >> 1));

        if (!text_) {
            return;
        }

        if (item_type_ == mgui_menu_item_type::ReturnToParent) {
            text_->set_x(h + text_rel_x_);
        }
        else {
            text_->set_x(text_rel_x_);
        }
        text_->set_y(rect_.y() + text_rel_y_);
        text_->set_view_width(screen_width);
    }

    inline mgui_text* text() const { return text_; }
    inline void set_text(mgui_text* text, uint16_t text_rel_x = 0, uint16_t text_rel_y = 0) {
        text_ = text;
        text_rel_x_ = text_rel_x;
        text_rel_y_ = text_rel_y;
    }

    /**
     * @brief Set the input event handler object
     *
     * @param event_callback
     * - Implement functions to change the state of the gui and operate other non-gui functions
     * using functions set in mgui_menu_item using the value of mgui_input_state
     * (the result of input reading set in mgui_input).
     * The set function is called each time before drawing is updated.
     */
    inline void set_input_event_handler(
        void (*event_callback)(const mgui_menu_item* sender, const mgui_input_state state[], mgui_string* current_group)) {
        input_event_callback_ = event_callback;
    }

    inline void set_menu(mgui_menu_property* menu) { 
        child_menu_ = menu;
        item_type_ = mgui_menu_item_type::Menu;
    }
    inline mgui_menu_property* menu() { return child_menu_; }

    inline void set_return_menu(bool init_value) {
        is_return_menu_ = init_value;
        item_type_ = mgui_menu_item_type::ReturnToParent;
    }
    inline bool return_menu() const { return is_return_menu_; }

    inline void set_check(bool init_value) { 
        is_checked_ = init_value;
        item_type_ = mgui_menu_item_type::Check;
    }
    inline bool checked() const { return is_checked_; }

private:

    inline void draw_check_box(mgui_draw* draw, mgui_input_state *input, mgui_string* current_group, bool invert) {
        check_rect_outer.set_invert(invert);
        check_rect_outer.update(draw, input, current_group);
        if (is_checked_) {
            check_rect_inner.set_invert(invert);
            check_rect_inner.update(draw, input, current_group);
        }
    }

    inline void draw_menu_guide(mgui_draw* draw, mgui_input_state* input, mgui_string* current_group, bool invert) {
        if (child_menu_) {
            menu_right_arrow_up.set_invert(invert);
            menu_right_arrow_down.set_invert(invert);
            menu_right_arrow_up.update(draw, input, current_group);
            menu_right_arrow_down.update(draw, input, current_group);
        }
    }

    inline void draw_return_menu_guide(mgui_draw* draw, mgui_input_state* input, mgui_string* current_group, bool invert) {
        if (is_return_menu_) {
            menu_left_arrow_up.set_invert(invert);
            menu_left_arrow_down.set_invert(invert);
            menu_left_arrow_up.update(draw, input, current_group);
            menu_left_arrow_down.update(draw, input, current_group);
        }
    }

    void (*input_event_callback_)(const mgui_menu_item* sender, const mgui_input_state state[], mgui_string* current_group);
    bool previous_on_press_;
    bool is_checked_;
    bool is_return_menu_;
    mgui_menu_item_type item_type_;
    mgui_menu_property* child_menu_;
    mgui_rectangle rect_;

    mgui_rectangle check_rect_outer;
    mgui_rectangle check_rect_inner;

    mgui_line menu_right_arrow_up;
    mgui_line menu_right_arrow_down;

    mgui_line menu_left_arrow_up;
    mgui_line menu_left_arrow_down;
    
    mgui_text *text_;
    uint16_t text_rel_x_;
    uint16_t text_rel_y_;
};

class mgui_menu : mgui_object {
public:

    /**
     * @brief Constructor.
     * @param width Target screen width
     * @param height Target screen height
     * @param item_view_count Count of items in each view
     */
    explicit mgui_menu(const uint16_t width, const uint16_t height, const uint16_t item_view_count = 4){
        moved_from_ = new mgui_stack<mgui_menu_property>();
        window_width_ = width;
        window_height_ = height;
        item_first_node_ = nullptr;
        item_view_count_ = item_view_count;
        on_return_ = false;
        on_enter_ = false;
        input_event_callback_ = nullptr;
    }

    /**
     * @brief Destructor.
     */
    ~mgui_menu(){
        delete moved_from_;
    }

    /**
     * @brief Copy constructor.
     */
    mgui_menu operator=(const mgui_menu& other) noexcept {
        if (this != &other) {
            this->moved_from_ = other.moved_from_;
            this->window_width_ = other.window_width_;
            this->window_height_ = other.window_height_;
            this->item_first_node_ = other.item_first_node_;
            this->item_view_count_ = other.item_view_count_;
            this->on_return_ = other.on_return_;
            this->on_enter_ = other.on_enter_;
            this->input_event_callback_ = other.input_event_callback_;
        }
        return *this;
    }

    /**
     * @brief Returns the type of the object.
     */
    mgui_object_type type() const { return mgui_object_type::Menu; }

    /**
     * @brief Updates the menu items by calling the input event callback
     *  and updating each menu item's draw position.
     * @param draw pointer to the draw object
     * @param input pointer to the input state object
     * @param current_group pointer to the current group string
     */
    void update(mgui_draw* draw, mgui_input_state* input, mgui_string* current_group) {
        if (input_event_callback_) {
            input_event_callback_(this, input, current_group);
        }

        mgui_list_node<mgui_menu_item*>* node = item_first_node_;
        for (int i = 0; i < item_view_count_; i++) {
            if (node == nullptr) {
                break;
            }
            node->obj->_set_draw_position(i, item_view_count_, window_width_, window_height_);
            node->obj->update(draw, input, current_group);
            node = node->next;
        }
    }

    /**
     * @brief Add a menu item to the menu and set it as selected if it's the only item.
     * @param item pointer to the menu item to add
     */
    inline void add(mgui_menu_item* item) {
        p.menu_item_.add(item);

        if(p.menu_item_.count() == 1){
            set_selected_index(0);
            item->set_on_selected(true);
        }
    }

	/**
	 * @brief Remove a menu item from the menu
	 * @param item pointer to the menu item to remove
	 */
    inline void remove(mgui_menu_item* item) {
        if(item_first_node_!= nullptr && item_first_node_->obj == item){
            item_first_node_ = nullptr;
        }

        p.menu_item_.remove(item);

        if(p.menu_item_.count() == 0) {
            item_first_node_ = nullptr;
        }
    }

    /**
     * @brief Return the index of the currently selected item.
     */
    inline uint16_t selected_index() const { return p.selected_index_; }
    
    /**
     * @brief Set the index of the currently selected item.
     * @param index_ index of the selected item.
     */
    inline void set_selected_index(uint16_t index_){
        p.selected_index_ = index_;
        int first = ((index_ + 1 - item_view_count_) > 0)? index_ + 1 - item_view_count_ : 0;

        if (first < p.menu_item_.count()) {
            item_first_node_ = p.menu_item_.get_node(first);
        }
        else {
            item_first_node_ = nullptr;
        }
    }

    /**
     * @brief Return the property of the menu.
     */
    inline mgui_menu_property* get_property() { return p.get_property(); }

    /**
     * @brief Return the currently selected menu item.
     */
    inline mgui_menu_item* get_selected_item() {
        return static_cast<mgui_menu_item*>(p.menu_item_.get(p.selected_index_));
    }

    /**
    * @brief Return specified menu list.
    * @param on_return 
    */
    inline void set_on_return(bool on_return) {
        if(on_return_ == on_return){
            return;
        }

        on_return_ = on_return;

        if (on_return && moved_from_->is_empty() == false) {
            p = static_cast<mgui_menu_property&&>(moved_from_->pop());
        }
    }
     
    /**
    * @brief Specified menu item entered.
    * @param on_enter 
    */
    inline void set_on_enter(bool on_enter) {
        if(on_enter_ == on_enter){
            return;
        }

        on_enter_ = on_enter;

        mgui_menu_item *item = p.menu_item_.get(p.selected_index_);

        // Return from item
        if (on_enter
         && item->item_type() == mgui_menu_item_type::ReturnToParent
         && moved_from_->is_empty() == false) {
            p = static_cast<mgui_menu_property&&>(moved_from_->pop());
            return;
        }

        item->set_on_press(on_enter);
        mgui_menu_property *menu = get_selected_item()->menu();
        if (on_enter && menu) {
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
            set_selected_index(p.selected_index_ + 1);
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
            set_selected_index(p.selected_index_ - 1);
            p.menu_item_.get(p.selected_index_)->set_on_selected(true);
        }
    }

    /**
    * @brief Set the input event handler object
    * 
    * @param event_callback 
    * Implement functions to change the state of the gui and operate other non-gui functions
    * using functions set in mgui_menu using the value of mgui_input_state
    * (the result of input reading set in mgui_input). 
    * The set function is called each time before drawing is updated.
    */
    inline void set_input_event_handler(
        void (*event_callback)(mgui_menu* sender, const mgui_input_state state[], mgui_string* current_group)) {
        input_event_callback_ = event_callback;
    }

    inline uint16_t item_view_count() const { return item_view_count_; }
    inline void set_item_view_count(uint16_t item_view_count) { item_view_count_ = item_view_count; }

    inline int menu_item_count() const { return p.menu_item_.count(); }

    inline uint16_t width() const { return window_width_; }
    inline void set_width(uint16_t width) { window_width_ = width; }

    inline uint16_t height() const { return window_height_; }
    inline void set_height(uint16_t height) { window_height_ = height; }

private:
    void (*input_event_callback_)(mgui_menu* sender, const mgui_input_state state[], mgui_string* current_group);
    bool on_return_;
    bool on_enter_;

    mgui_list_node<mgui_menu_item*>* item_first_node_;
    uint16_t item_view_count_;
    uint16_t window_height_;
    uint16_t window_width_;
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
        input_event_callback_ = nullptr;
    }
    ~mgui_ui_group() {
        delete list;
    }

    mgui_object_type type() const { return mgui_object_type::UiGroup; }

     /**
      * @brief Set the input event handler object
      * 
      * @param event_callback 
      * Implement functions to change the state of the gui and operate other non-gui functions
      * using functions set in mgui_menu using the value of mgui_input_state
      * (the result of input reading set in mgui_input). 
      * The set function is called each time before drawing is updated.
      */
     inline void set_input_event_handler(
         void (*event_callback)(mgui_ui_group* sender, const mgui_input_state state[], mgui_string* current_group)) {
         input_event_callback_ = event_callback;
     }

    /**
     * @brief
     * Updates the UI group by calling the input event callback and updating each UI object.
     * @param draw Pointer to the draw object.
     * @param input Pointer to the input state object.
     * @param current_group Pointer to the current group string.
     */
    void update(mgui_draw* draw, mgui_input_state* input, mgui_string* current_group) {
        if (input_event_callback_) {
            input_event_callback_(this, input, current_group);
        }

        mgui_list_node<mgui_core_ui*>* node = list->first();
        while (node != nullptr) {
            node->obj->update(draw, input, current_group);
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

    void (*input_event_callback_)(mgui_ui_group* sender, const mgui_input_state state[], mgui_string* current_group);

    mgui_list<mgui_core_ui*>* list;
    uint16_t selected_index_;
};

class mgui_vertical_scrollbar : public mgui_object {
public:
    mgui_vertical_scrollbar() {
        current_index_ = 0;
        count_ = 0;
        full_cursor_height_ = 0;
        cursor_.set_fill(true);
        input_event_callback_ = nullptr;
    }

    /**
     * Constructs a new mgui_vertical_scrollbar object with the specified x, y, width, height, and count.
     *
     * @param x The x-coordinate of the scrollbar.
     * @param y The y-coordinate of the scrollbar.
     * @param width The width of the scrollbar.
     * @param height The height of the scrollbar.
     * @param count The number of items to scroll.
     */
    explicit mgui_vertical_scrollbar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int count)
        : mgui_vertical_scrollbar(){
        set_x(x);
        set_y(y);
        set_width(width);
        set_height(height);
        set_count(count);
    }

    ~mgui_vertical_scrollbar() {}

    mgui_object_type type() const { return mgui_object_type::VerticalScroll; };

    /**
     * @brief Set the input event handler object
     *
     * @param event_callback
     * Implement functions to change the state of the gui and operate other non-gui functions
     * using functions set in mgui_menu using the value of mgui_input_state
     * (the result of input reading set in mgui_input).
     * The set function is called each time before drawing is updated.
     */
    inline void set_input_event_handler(
        void (*event_callback)(mgui_vertical_scrollbar* sender, const mgui_input_state state[], mgui_string* current_group)) {
        input_event_callback_ = event_callback;
    }

   /**
    * @brief Updates the state of the vertical scrollbar.
    * 1. If the count is less than 1, the function returns early.
    * 2. If an input event callback is set, it is called with the current object, input state, and current group.
    *
    * @param draw Pointer to the draw object used for rendering.
    * @param input Pointer to the input state object containing input data.
    * @param current_group Pointer to the current group string.
    */
    void update(mgui_draw* draw, mgui_input_state* input, mgui_string* current_group) {
        if (count_ < 1) {
            return;
        }

        if (input_event_callback_) {
            input_event_callback_(this, input, current_group);
        }

        frame_.update(draw, input, current_group);
        uint16_t y = 2 + full_cursor_height_ * current_index_ / count_;
        cursor_.set_y(y);
        cursor_.update(draw, input, current_group);
    }

    /**
     * @brief Sets the count of items and updates the height of the cursor accordingly.
     * @param count The new count of items.
     */
    inline void set_count(const int count) {
        count_ = count;
        full_cursor_height_ = frame_.height() - 4;
        uint16_t cursor_height_ = full_cursor_height_ / count;
        cursor_height_ = cursor_height_ > 0 ? cursor_height_ : full_cursor_height_;
        cursor_.set_height(cursor_height_);
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
        if (current_index_ < (count_ - 1)) {
            current_index_++;
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

        if (current_index_ > 0) {
            current_index_--;
        }
    }
    inline uint16_t current_index() const { return current_index_; }

    inline uint16_t radius() const { return frame_.radius(); }
    
    /**
     * @brief Sets the radius of the object.
     * @param r The new radius value.
     */
    inline void set_radius(uint16_t r) {
        frame_.set_radius(r);
        cursor_.set_radius(r - 1);
    }

    inline uint16_t width() const { return frame_.width(); }

    /**
     * @brief Set the width of the object.
     * @note Width value must be > 5.
     * @param width The new width value.
     */
    inline void set_width(uint16_t width) {
        frame_.set_width(width);
        cursor_.set_width(width - 4);
    }

    inline uint16_t height() const { return frame_.height(); }

    /**
     * @brief Set the height
     * @param height 
     */
    inline void set_height(uint16_t height) { frame_.set_height(height); }

    inline uint16_t x() const { return frame_.x(); }

    /**
     * @brief Set the x-coordinate of the object.
     * @param x The new x-coordinate.
     */
    inline void set_x(uint16_t x) {
        frame_.set_x(x);
        cursor_.set_x(x + 2);
    }

    inline uint16_t y() const { return frame_.y(); }

    /**
     * @brief Set the y-coordinate of the object.
     * @param y The new y-coordinate.
     */
    inline void set_y(uint16_t y) {
        frame_.set_y(y);
        cursor_.set_y(y + 2);
    }

private:
    void (*input_event_callback_)(mgui_vertical_scrollbar* sender, const mgui_input_state state[], mgui_string* current_group);
    mgui_rectangle frame_;
    mgui_rectangle cursor_;
    uint16_t full_cursor_height_;
    uint16_t current_index_;
    uint16_t count_;
};

#endif // MGUI_H
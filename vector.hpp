#include <cstring>
#include <iostream>

using namespace std;

template<class T>
class Array {
    T* ptr;
    size_t capacity;
    size_t length;

    void allocate_memory(size_t size) {
        ptr = static_cast<T*>(calloc(size, sizeof(T)));
    }

    void reallocate_memory(size_t size) {
        ptr = static_cast<T*>(realloc(ptr, sizeof(T) * size));
    }

    void free_memory() {
        if (ptr) {
            size_t count = 0;
            while (count < length) {
                (ptr + count)->~T();
                count++;
            }
            free(ptr);
        }

        ptr = nullptr;
    }

    void assign_fields(const Array &other, bool copy) {
        if (copy) free_memory();

        length = other.length;
        capacity = other.capacity;

        allocate_memory(other.capacity);
        for(size_t i = 0; i < other.length; i++) ptr[i] = other.get_ptr()[i];
    }

    public:

    static const size_t STEP = 10;
    static const int MULT = 2;

    typedef T* iterator;
    typedef const T* const const_iterator;

    Array() { capacity = Array::STEP; length = 0; ptr = nullptr; }
    Array(const Array &other) { assign_fields(other, false); }

    ~Array() { free_memory(); }

    Array& operator=(const Array &other) {
        if (this == &other) return *this;
        assign_fields(other, true);
        return *this;
    }

    T& operator[] (size_t index) const { return ptr[index]; }
    T& operator[] (size_t index) { return ptr[index]; }

    iterator begin() { return ptr; }
    iterator end() { return ptr + length; }

    size_t size() const { return length; }
    bool empty() const { return length == 0; }

    T& back() const { return ptr[length - 1]; }
    T& front() const { return ptr[0]; }

    T* get_ptr() const { return ptr; }
    void set_ptr(T* other_ptr, size_t size) {
        ptr = other_ptr;
        length = size;
        capacity = (size > capacity) ? size : capacity;
    }

    void reserve(size_t new_capacity) {
        if (capacity < new_capacity && ptr) reallocate_memory(new_capacity);
        if (capacity < new_capacity) capacity = new_capacity;
        if (!ptr) allocate_memory(capacity);
    }

    void resize(size_t new_size) {
        if (new_size == 0) clear();
        else if (new_size < length) {
            while (length > new_size) this->pop_back();
        }
        else {
            while (length < new_size) this->push_back(T());
        }
    }

    void clear() {
        free_memory();

        length = 0;
        capacity = Array::STEP;
    }

    void push_back(const T& item) {
        if (!ptr) allocate_memory(capacity);

        new(ptr + length) T(item);
        length++;

        if (length == capacity) {
            capacity *= Array::MULT;
            reallocate_memory(capacity);
        }
    }

    void pop_back() {
        if (length == 1) {
            clear();
            return;
        }

        (ptr + length - 1)->~T();
        if (length > 1) length--;
    }

    void swap(Array &other) {
        T* tmp = other.get_ptr();
        other.set_ptr(ptr, length);
        ptr = tmp;
    }

    void append(Array &other) {
        size_t other_size = other.size(), count = 0;

        while (count < other_size) {
            this->push_back(other[count]);
            count++;
        }
    }
};

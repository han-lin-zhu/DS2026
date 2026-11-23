#ifndef MY_STL_VECTOR_HPP
#define MY_STL_VECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <utility>

namespace my_stl {

// T：元素类型。使用 new T[] 管理底层数组，要求 T 支持默认构造。
template <typename T>
class vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    // using reference = value_type&; // 元素类型的左值引用别名，可通过该引用修改元素
    // using const_reference = const value_type&;
    using pointer = value_type*; // 指针
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>; // 反向迭代器类型
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; // 只读反向迭代器类型

public:
    // 常用参数名：
    // count：元素数量；value：元素值。
    // other：另一个容器；init：初始化列表；new_capacity：请求的最小容量。
    // pos：元素访问时为下标，插入或删除时为位置迭代器。
    // first、last：区间起点与终点，表示左闭右开区间 [first, last)。
    // args：转发给元素构造函数的参数包。

    // 构造、析构与赋值
    vector() = default;
    explicit vector(size_type count)
        : data_(count == 0 ? nullptr : new value_type[count]()),
          size_(count), capacity_(count) {}

    vector(size_type count, const value_type& value) : vector(count) {
        for (size_type i = 0; i < count; ++i) {
            data_[i] = value;
        }
    }
    vector(const vector& other) : vector(other.size_) {
        for (size_type i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    vector(vector&& other) noexcept;

    vector(std::initializer_list<value_type> init) : vector(init.size()) {
        size_type i = 0;
        for (const auto& value : init) {
            data_[i++] = value;
        }
    }

    ~vector() { delete[] data_; }

    vector& operator=(const vector& other) {
        if (this != &other) {
            vector copy(other);
            swap(copy);
        }
        return *this;
    }

    vector& operator=(vector&& other) noexcept;

    vector& operator=(std::initializer_list<value_type> init) {
        vector copy(init);
        swap(copy);
        return *this;
    }

    // 元素访问
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // 迭代器
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // 容量管理
    bool empty() const noexcept;//1
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void reserve(size_type new_capacity);
    size_type capacity() const noexcept;
    void shrink_to_fit();

    // 修改操作
    void clear() noexcept;
    void push_back(const_reference value);
    void push_back(value_type&& value);
    template <typename... Args>
    reference emplace_back(Args&&... args);
    void pop_back();
    void resize(size_type count);
    void resize(size_type count, const_reference value);
    void swap(vector& other) noexcept;

    iterator insert(const_iterator pos, const_reference value);
    iterator insert(const_iterator pos, value_type&& value);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

private:
    pointer data_ = nullptr;   // 指向 new[] 创建的数组，由 delete[] 释放
    size_type size_ = 0;       // 逻辑元素数量
    size_type capacity_ = 0;   // 底层数组长度；new[] 会构造整个数组中的对象
};

// 非成员运算符与交换函数
// lhs、rhs：左侧与右侧容器；交换函数中表示参与交换的两个容器。

template <typename T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs);

template <typename T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs);

//交换
template <typename T>
void vector<T>::swap(vector& other) noexcept{
    pointer old_data = data_;
    data_ = other.data_;
    other.data_ = old_data;

    size_type old_size = size_;
    size_ = other.size_;
    other.size_ = old_size;

    size_type old_capacity = capacity_;
    capacity_ = other.capacity_;
    other.capacity_= old_capacity;
}

//判空
template <typename T>
bool vector<T>::empty() const noexcept{
    return size_ == 0;
}
} // 命名空间 my_stl

#endif

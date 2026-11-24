#ifndef MY_STL_VECTOR_HPP
#define MY_STL_VECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

namespace my_stl {

// T：元素类型。使用 new T[] 管理底层数组，要求 T 支持默认构造。
template <typename T>
class vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&; // 元素类型的左值引用别名，可通过该引用修改元素
    using const_reference = const value_type&;
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

    // 构造、析构与赋值1
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

    // vector& operator=(vector&& other) noexcept;

    vector& operator=(std::initializer_list<value_type> init) {
        vector copy(init);
        swap(copy);
        return *this;
    }

    // 元素访问1
    // reference at(size_type pos);
    // const_reference at(size_type pos) const;
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }

    reference front() {
        return *data_;
    }
    const_reference front() const { return *data_; }

    reference back() { return data_[size_-1]; }
    const_reference back() const { return data_[size_-1]; }

    // pointer data() noexcept;
    // const_pointer data() const noexcept;

    // 迭代器1
    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    // const_iterator cbegin() const noexcept;

    iterator end() noexcept { return data_+size_; }
    const_iterator end() const noexcept { return data_+size_; }
    // const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    // const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    // const_reverse_iterator crend() const noexcept;

    // 容量管理1
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept{
        const auto max_bytes =static_cast<size_type>(std::numeric_limits<difference_type>::max());
        return max_bytes / sizeof(value_type);
    }
    void reserve(size_type new_capacity){
        if (new_capacity <= capacity_) {
            return;
        }
        if (new_capacity > max_size()) {
            throw;
        }

        value_type* new_data = new value_type[new_capacity]();

        for (size_type i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = std::move(new_data);
        capacity_ = new_capacity;
    }

    size_type capacity() const noexcept { return capacity_; }
    //void shrink_to_fit();

    // 修改操作
    void clear() noexcept { size_=0; }
    void push_back(const_reference value) {
        if(size_ == capacity_){
            const size_type limit = max_size();
            if(size_ == limit) throw;
            size_type new_capacity = 1;
            if (capacity_ != 0) {
                new_capacity = capacity_ > (limit >> 1) ? limit : capacity_ << 1;
            }
            reserve(new_capacity);
            data_[size_] = value;
        }
        else {
            data_[size_]=value;
        }
        ++size_;
    }
    //void push_back(value_type&& value);
    template <typename... Args>
    reference emplace_back(Args&&... args){
        push_back(value_type(std::forward<Args>(args)...));
        return data_[size_ - 1];
    }
    void pop_back(){
        data_[size_-1]=0;
        size_--;
    }
    void resize(size_type count) {
        if (count > max_size()) {
            throw ;
        }
        if (count > size_) {
            reserve(count);
            for (size_type index = size_; index < count; ++index) {
                data_[index] = value_type{};
            }
        }
        size_ = count;
    }

    void resize(size_type count, const_reference value) {
        if (count > max_size()) {
            throw;
        }
        if (count > size_) {
            value_type value_copy(value);
            reserve(count);
            for (size_type index = size_; index < count; ++index) {
                data_[index] = value_copy;
            }
        }
        size_ = count;
    }
    void swap(vector& other) noexcept{
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

    iterator insert(const_iterator pos, const_reference value) {
        value_type value_copy(value);
        return insert(pos, std::move(value_copy));
    }

    iterator insert(const_iterator pos, value_type&& value) {
        // 空容器的迭代器可能为空指针，不能直接进行指针相减。
        const size_type index = size_ == 0 ? 0 : static_cast<size_type>(pos - data_);
        const size_type limit = max_size();
        if (size_ == limit) {
            throw ;
        }

        value_type value_copy=std::move(value);
        if (size_ == capacity_) {
            const size_type new_capacity = (capacity_ == 0 ? 1
                : (capacity_ > (limit >> 1) ? limit : capacity_ << 1));
            value_type* new_data = new value_type[new_capacity]();
            new_data[index] = std::move(value_copy);
            for (size_type i = 0; i < index; ++i) {
                new_data[i] = std::move(data_[i]);
            }
            for (size_type i = index; i < size_; ++i) {
                new_data[i + 1] = std::move(data_[i]);
            }
            delete[] data_;
            data_ = std::move(new_data);
            capacity_ = new_capacity;
        }
        else {
            for (size_type i = size_; i > index; --i) {
                data_[i] = std::move(data_[i - 1]);
            }
            data_[index] = std::move(value_copy);
        }
        ++size_;
        return data_ + index;
    }

    iterator erase(const_iterator pos) {
        const size_type index = static_cast<size_type>(pos - data_);
        for (size_type i = index; i + 1 < size_; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        // 底层使用 new[]，以默认值替换尾部对象来释放它持有的资源。
        data_[size_ - 1] = value_type{};
        --size_;
        return data_ + index;
    }
    //iterator erase(const_iterator first, const_iterator last);

    static void sort(vector<T>& lhs);

private:
    pointer data_ = nullptr;   // 指向 new[] 创建的数组，由 delete[] 释放
    size_type size_ = 0;       // 逻辑元素数量
    size_type capacity_ = 0;   // 底层数组长度；new[] 会构造整个数组中的对象
    static void sort_down(pointer data,size_type root,size_type size);
};

// 非成员运算符与交换函数
// lhs、rhs：左侧与右侧容器；交换函数中表示参与交换的两个容器。
template <typename T>
void vector<T>::sort_down(pointer data,size_type root,size_type size){

}
template <typename T> 
void vector<T>::sort(vector<T>& lhs){
    
}
template <typename T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs);

template <typename T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs);

} // 命名空间 my_stl

#endif

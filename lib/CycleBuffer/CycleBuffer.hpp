#pragma once

#include <iostream>
#include <iterator>
#include <memory>

template<typename T, typename Alloc = std::allocator<T>>
class CycleBuffer;

template<typename T, typename Alloc = std::allocator<T>>
std::ostream &operator<<(std::ostream &out, CycleBuffer<T, Alloc> &v);


template<typename T, typename Alloc>

class CycleBuffer {
protected:
    size_t capacity_;
    Alloc alloc;
    T *objects_;
    T *begin_;
    T *end_;

    template<bool ConstFlag>
    class Common_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        std::conditional_t<ConstFlag, const CycleBuffer<T, Alloc> *, CycleBuffer<T, Alloc> *> buffer_;
        std::conditional_t<ConstFlag, const T *, T *> current_;

    public:
        Common_iterator(std::conditional_t<ConstFlag, const CycleBuffer<T, Alloc> *, CycleBuffer<T, Alloc> *> buf,
                        T *cur) {
            buffer_ = buf;
            current_ = cur;
        }

        Common_iterator(const Common_iterator &other) : current_(other.current_), buffer_(other.buffer_) {}

        std::conditional_t<ConstFlag, const T &, T &> operator*() {
            if (current_ == buffer_->end_) throw std::out_of_range("out of range");
            return *(this->current_);
        }

        std::conditional_t<ConstFlag, const T *, T *> operator->() {
            return this->current_;
        }

        void swap(Common_iterator &other) {
            std::swap(buffer_, other.buffer_);
            std::swap(current_, other.current_);
        }

        bool operator==(const Common_iterator &iter) {
            return this->current_ == iter.current_;
        }

        bool operator!=(const Common_iterator &iter) {
            return this->current_ != iter.current_;
        }

        Common_iterator &operator+=(int i) {
            while (i < 0) {
                i += buffer_->capacity_;
            }
            i = i % buffer_->capacity_;
            if (i < 0) i += buffer_->capacity_;
            else if (i == 0) return *this;
            if (current_ + i > buffer_->objects_ + buffer_->capacity_ - 1) {
                i -= buffer_->objects_ + buffer_->capacity_ - current_;
                current_ = buffer_->objects_;
            }
            current_ += i;
            if ((current_ > buffer_->end_ && current_ < buffer_->begin_ && buffer_->end_ < buffer_->begin_) &&
                !(current_ >= buffer_->begin_ && current_ <= buffer_->end_ && buffer_->end_ >= buffer_->begin_)) {
                throw std::out_of_range("out of container");
            }
            return *this;

        }

        Common_iterator &operator-=(int i) {
            return (*this) += -i;
        }

        Common_iterator &operator++() {
            return (*this) += 1;
        }

        Common_iterator &operator--() {
            return (*this) -= 1;
        }


        Common_iterator operator--(int) {
            Common_iterator temp = *this;
            --(*this);
            return temp;
        }

        Common_iterator operator++(int) {
            Common_iterator temp = *this;
            ++(*this);
            return temp;
        }

        Common_iterator operator+(int i) {
            Common_iterator temp = *this;
            return temp += i;
        }

        Common_iterator operator-(int i) {
            Common_iterator temp = *this;
            return temp -= i;
        }

        Common_iterator &operator[](size_t i) {
            return *((*this) + i);
        }

        bool operator<(const Common_iterator &iter) const {
            if (buffer_->end_ > buffer_->begin_) return this->current_ < iter.current_;
            if ((this->current_ <= this->buffer_->end_ && iter.current_ <= this->buffer_->end_) ||
                (this->current_ >= this->buffer_->begin_ && iter.current_ >= this->buffer_->begin_))
                return this->current_ < iter.current_;
            return iter.current_ < this->current_;
        }

        difference_type operator-(const Common_iterator &iter) const {
            if (buffer_->end_ > buffer_->begin_) return this->current_ - iter.current_;
            if ((this->current_ <= this->buffer_->end_ && iter.current_ <= this->buffer_->end_) ||
                (this->current_ >= this->buffer_->begin_ && iter.current_ >= this->buffer_->begin_))
                return this->current_ - iter.current_;
            if (this->current_ > iter.current_) return this->current_ - iter.current_ - this->buffer_->capacity_;
            return this->current_ - iter.current_ + this->buffer_->capacity_;
        }

        bool operator<=(const Common_iterator &iter) const {
            return (*this) < iter || (*this) == iter;
        }


        bool operator>=(const Common_iterator &iter) const {
            return iter <= (*this);
        }

        bool operator>(const Common_iterator &iter) const {
            return iter < (*this);
        }
    };

    friend std::ostream &operator<<<>(std::ostream &out, CycleBuffer<T, Alloc> &v);

public:
    using iterator = Common_iterator<false>;
    using const_iterator = Common_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using allocator_traits = typename std::allocator_traits<Alloc>;


    void reserve(size_t n) {
        if (n < capacity_) return;
        T *new_obj = allocator_traits::allocate(alloc, n + 1);
        if (objects_ != nullptr) {
            try {
                std::uninitialized_copy(begin(), end(), new_obj);
            }
            catch (...) {
                allocator_traits::deallocate(alloc, new_obj, n + 1);
                throw;
            }
        }

        for (auto temp = begin(); temp != end(); ++temp) {
            std::destroy_at(temp.operator->());
        }
        end_ = new_obj + size();

        begin_ = objects_ = new_obj;
        capacity_ = n + 1;
    }

    void resize(size_t n, const T &element = T()) {
        if (capacity_ < n) reserve(n);
        for (; size() < n;) {
            std::construct_at(end_, element);
            end_++;
        }
    }

    CycleBuffer() : objects_(nullptr), begin_(nullptr), end_(nullptr), capacity_(0) {
        reserve(1);
    }

    CycleBuffer(const CycleBuffer &other) {
        this->reserve(other.capacity_);
        std::uninitialized_copy(other.cbegin(), other.cend(), objects_);
        begin_ = objects_;
        end_ = objects_ + other.size();
    }

    CycleBuffer &operator=(const CycleBuffer &other) {
        if (this == &other) return *this;

        if (objects_ != nullptr) {
            for (auto it = begin(); it != end(); ++it) {
                std::destroy_at(it.operator->());
            }
            allocator_traits::deallocate(alloc, objects_, capacity_);
        }
        reserve(other.capacity_);
        std::uninitialized_copy(other.begin(), other.end(), objects_);
        begin_ = objects_;
        end_ = objects_ + other.size();

        return *this;
    };

    explicit CycleBuffer(size_t c) : objects_(nullptr), begin_(nullptr), end_(nullptr), capacity_(0) {
        this->reserve(c);
    }

    CycleBuffer(size_t c, const T &element) : objects_(nullptr), begin_(nullptr), end_(nullptr), capacity_(0) {
        this->resize(c, element);
    }

    ~CycleBuffer() {
        if (objects_ == nullptr) return;

        for (auto it = begin(); it != end(); ++it) {
            std::destroy_at(it.operator->());
        }

        allocator_traits::deallocate(alloc, objects_, capacity_);
        objects_ = nullptr;
    }

    bool operator==(const CycleBuffer &other) const {
        if (this->size() != other.size()) return false;
        auto it = other.begin();
        for (auto i = this->begin; i < this->end(); ++i) {
            if (*i != *it) return false;
            ++it;
        }
        return true;
    }

    bool operator!=(const CycleBuffer &other) const {
        return *this != other;
    }

    T &operator[](size_t i) {
        return *(this->begin() + i);
    }

    T &at(size_t i) {
        if (i < 0 || i > capacity_) {
            throw std::out_of_range("Out of range in buffer");
        }
        return *(this->begin() + i);
    }

    void swap(CycleBuffer &other) {
        std::swap(begin_, other.begin_);
        std::swap(end_, other.end_);
        std::swap(objects_, other.objects_);
        std::swap(capacity_, other.capacity_);
    }

    [[nodiscard]] bool empty() const {
        return end_ == begin_ || objects_ == nullptr;
    }

    [[nodiscard]] size_t capacity() const {
        return capacity_ - 1;
    }

    [[nodiscard]] size_t max_size() const {
        return INT_MAX;
    };

    [[nodiscard]] size_t size() const {
        if (objects_ == nullptr || end_ == begin_) return 0;
        if (end_ > begin_) return end_ - begin_;
        return (end_ - objects_) + (objects_ + capacity_ - begin_);
    }

    iterator begin() {
        return iterator(this, begin_);
    }

    iterator end() {
        return iterator(this, end_);
    }

    [[nodiscard]] const_iterator cbegin() const {
        return const_iterator(this, begin_);
    }

    [[nodiscard]] const_iterator cend() const {
        return const_iterator(this, end_);
    }

    reverse_iterator rbegin() {
        return std::reverse_iterator(end());
    }

    reverse_iterator rend() {
        return std::reverse_iterator(begin());
    }

    [[nodiscard]] const_reverse_iterator crbegin() const {
        return const_reverse_iterator(end());
    }

    [[nodiscard]] const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }


    T &front() {
        return *begin_;
    }

    [[nodiscard]] const T &front() const {
        return *begin_;
    }

    T &back() {
        return *(end_ - 1);
    }

    [[nodiscard]] const T &back() const {
        return *(end_ - 1);
    }
};

template<typename T, typename Alloc>
std::ostream &operator<<(std::ostream &out, CycleBuffer<T, Alloc> &v) {
    for (auto i: v) {
        out << i << ' ';
    }
    return out;
}


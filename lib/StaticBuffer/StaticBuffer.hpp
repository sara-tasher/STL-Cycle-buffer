#include "../CycleBuffer/CycleBuffer.hpp"


template<typename T, typename Alloc = std::allocator<T>>
class StaticBuffer : public CycleBuffer<T, Alloc> {
private:
    using CycleBuffer<T, Alloc>::capacity_;
    using CycleBuffer<T, Alloc>::objects_;
    using CycleBuffer<T, Alloc>::begin_;
    using CycleBuffer<T, Alloc>::end_;
public:
    StaticBuffer() : CycleBuffer<T, Alloc>() {};

    explicit StaticBuffer(size_t c, const T &element) : CycleBuffer<T, Alloc>(c, element) {};

    explicit StaticBuffer(size_t c) : CycleBuffer<T, Alloc>(c) {};

    explicit StaticBuffer(const CycleBuffer<T, Alloc> &other) : CycleBuffer<T, Alloc>(other) {};

    StaticBuffer &operator=(const StaticBuffer &other) = delete;

    void push_back(const T &element) {
        if (this->size() == this->capacity()) {
            throw std::out_of_range("out of container");
        }
        std::construct_at(end_, element);

        if (++end_ == objects_ + capacity_) end_ = objects_;
    }

    void pop_back() {
        if (this->empty()) return;
        if (end_ == objects_) end_ = objects_ + capacity_;
        std::destroy_at(--end_);
    }

    void pop_front() {
        if (this->empty()) return;
        std::destroy_at(begin_);
        if (begin_ == objects_ + capacity_ - 1) begin_ = objects_;
        else ++begin_;
    }

    void push_front(const T &element) {
        if (this->size() == this->capacity()) {
            throw std::out_of_range("out of container");
        }
        if (begin_ == objects_) begin_ = objects_ + capacity_ - 1;
        else --begin_;
        std::construct_at(begin_, element);
    }

    void clear() {
        while (!this->empty()) {
            this->pop_back();
        }
        begin_ = end_ = objects_;
    }

    void erase(typename CycleBuffer<T, Alloc>::iterator q1, typename CycleBuffer<T, Alloc>::iterator q2) {
        auto diff = q2 - q1;
        while (q2 != this->end()) {
            (*q1) = *q2;
            q2++;
            q1++;
        }
        while (diff > 0) {
            this->pop_back();
            diff--;
        }
    }

    void erase(typename CycleBuffer<T, Alloc>::iterator q) {
        return this->erase(q, q + 1);
    }

    template<typename InputIt>
    void assign(InputIt f, InputIt l) {
        this->clear();
        while (f != l) {
            this->push_back(*f);
            f++;
        }
    }

    void assign(std::initializer_list<T> l) {
        this->assign(l.begin(), l.end());
    }

    void assign(size_t n, const T &element) {
        this->clear();
        while (n > 0) {
            this->push_back(element);
            n--;
        }
    }

    typename CycleBuffer<T, Alloc>::iterator insert(typename CycleBuffer<T, Alloc>::iterator p, const T &element) {
        auto index = p - this->begin();
        this->push_back(element);
        p = this->begin() + index;
        auto it = p;
        auto iter = this->end() - 1;
        while (iter != it) {
            *iter = *(iter - 1);
            iter--;
        }
        *it = element;
        return this->begin() + index;
    }

    typename CycleBuffer<T, Alloc>::iterator
    insert(typename CycleBuffer<T, Alloc>::iterator p, size_t n, const T &element) {
        auto index = p - this->begin();
        for (int i = 0; i < n; i++) {
            this->push_back(element);
        }
        p = this->begin() + index;
        auto it = p - 1;
        while (p + n != this->end()) {
            std::swap(*p, *(p + n));
            p++;
        }

        return it;
    }

    typename CycleBuffer<T, Alloc>::iterator
    insert(typename CycleBuffer<T, Alloc>::iterator p, const std::initializer_list<T> &l) {
        auto index = p - this->begin();
        for (int i = 0; i < l.size(); i++) {
            this->push_back(*l.begin());
        }
        p = this->begin() + index;
        while (p + l.size() != this->end()) {
            *(p + l.size()) = *p;
            p++;
        }
        p = this->begin() + index;
        auto it = l.begin();
        while (it != l.end()) {
            *p = *it;
            p++;
            it++;
        }
        return this->begin() + index;
    }

    template<typename InputIt>
    typename CycleBuffer<T, Alloc>::iterator insert(typename CycleBuffer<T, Alloc>::iterator p, InputIt f, InputIt l) {
        auto it = p;
        auto index = p - this->begin();
        while (f != l) {
            it = this->insert(it, *f) + 1;
            f++;
        }
        return this->begin() + index;
    }
};
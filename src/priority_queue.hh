#pragma once

#include <exception>
#include <iostream>

class PriorityQueueException : public std::exception {
private:
    std::string error_;
public:
    PriorityQueueException(const std::string &error) noexcept: error_(error) {}
    virtual char const* what() const throw() {
        return error_.c_str();
    }
};

template <class T>
class PriorityQueue {
private:
    struct Node {
        T value_;
        unsigned int key_;
        Node* next_;
    };

    Node* front_;

public:
    PriorityQueue() noexcept;
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue(PriorityQueue&&) = default;

    ~PriorityQueue() noexcept;

    bool is_empty() const noexcept;
    void push_back(const T&, const unsigned int) noexcept;
    T& top() const;
    void pop_front();
    
    bool find_if_in(const T&) const noexcept;

    // void afficher() const noexcept;
};

template <class T>
PriorityQueue<T>::PriorityQueue() noexcept : front_(nullptr) {}

template <class T> PriorityQueue<T>::~PriorityQueue() noexcept {
    Node* p;
    while (front_ != nullptr) {
        p = front_;
        front_ = front_->next_;
        delete p;
    }
}

template <class T> bool PriorityQueue<T>::is_empty() const noexcept {
    return front_ == nullptr;
}

template <class T> void PriorityQueue<T>::push_back(const T &value, const unsigned int key) noexcept {
    Node* p(new Node);
    p->value_ = value;
    p->key_ = key;
    p->next_ = front_;
    
    Node* last(nullptr);
    while(p->next_ && p->key_ >= p->next_->key_) {
        last = p->next_;
        p->next_ = p->next_->next_;
    }
    if (last) last->next_ = p;
    else front_ = p;
}

template <class T> T& PriorityQueue<T>::top() const {
    if (!front_) throw PriorityQueueException("Can't read from empty queue");
    return front_->value_;
}

template <class T> void PriorityQueue<T>::pop_front() {
    if (!front_) throw PriorityQueueException("Can't pop from empty queue");
    Node* p(front_);
    front_ = front_->next_;
    delete p;
}

template <class T> bool PriorityQueue<T>::find_if_in(const T &value) const noexcept {
    Node* p(front_);
    while (p && p->value_ != value) {
        p = p->next_;
    }
    return p != nullptr;
}

// template <class T> void PriorityQueue<T>::afficher() const noexcept {
//     std::cout << "[";
//     Node* p(front_);
//     while(p) {
//         std::cout << p->value_ << " ";
//         p = p->next_;
//     }
//     std::cout << "]" << std::endl;
// }

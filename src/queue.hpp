#pragma once

#include <list>
#include <initializer_list>

//! Generic queue
// Generalized implementation of the queue data structure which
// enforces FIFO (first in, first out) insertion/deletion order.
template <class T>
class queue {
private:
    //! Underlying buffer
    std::list<T> m_buffer;

public:
    //! Default constructor
    queue() = default;

    //! Copy constructor
    queue(const queue<T> &other) = default;

    //! Initializer list constructor
    queue(std::initializer_list<T> list):
        m_buffer{ list.begin(), list.end() }
    {}

    //! Iterators constructor
    template <class InputIt>
    queue(InputIt first, InputIt last):
        m_buffer{ first, last }
    {}

    //! Is empty
    bool empty() const {
        return m_buffer.empty();
    }

    //! Enqueue  value
    // @param val Queued (added) value.
    void enqueue(T val) {
        m_buffer.push_back(val);
    }

    //! Dequeue value
    // @return Dequeued (removed) value, or -1 if empty.
    T dequeue() {
        uint16_t val = -1;
        if (!m_buffer.empty()) {
            val = m_buffer.front();
            m_buffer.pop_front();
        }
        return val;
    }

    //! Erase elements
    // @param n Number of elements to erase.
    void erase(int n) {
        m_buffer.erase(m_buffer.begin(), std::next(m_buffer.begin(), n));
    }

    //! Clear queue of all elements
    void clear() {
        m_buffer.clear();
    }

    //! Is normal (first element is zero)
    bool normal() const {
        return m_buffer.front() == 0;
    }

    //! Normalize
    // If not already normal, subtracts the first element from itself and all elements.
    void normalize() {
        if (!normal()) {
            uint16_t front = m_buffer.front();
            for (auto &v: m_buffer)
                v -= front;
        }
    }

    //! Peek some
    // @return Vector of first n values in queue.
    std::vector<T> peek(size_t n) const {
        return {
            m_buffer.cbegin(),
            std::next(m_buffer.cbegin(), std::min(n, m_buffer.size()))
        };
    };

    //! Peek all
    // @return Vector of all values in queue.
    std::vector<T> peek() const {
        return peek(m_buffer.size());
    }

    //! Size
    size_t size() const {
        return m_buffer.size();
    }
};

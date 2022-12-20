module;

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <variant>
#include <numeric>
#include <compare>
#include <sstream>
#include <ranges>
#include <map>
#include <list>

export module day20.decryption;

export namespace day20::decryption {

// Advance an iterator by the given amount, with wrap-around
auto advance(auto it, int64_t amount, auto begin, auto end) {
    auto pos = it;
    while (amount > 0) {
        if (pos == end)
            pos = begin;
        ++pos;
        --amount;
    }
    return pos;
}

// Shuffle once and return iterator to zero
std::list<int64_t>::iterator shuffle_once(std::list<int64_t>& data, const std::vector<std::list<int64_t>::iterator>& order) {
    // Get a splice position from a positive offset
    auto positive_offset = [&data](auto it, int64_t value) {
        // No-op splice is with an iterator to the next element
        auto pos = std::next(it);
        // Moving an element size-1 times ends up in the same position
        int64_t amount = (value)%(std::ssize(data)-1);
        // Get the new position
        return advance(pos, amount, data.begin(), data.end());
    };
    
    auto negative_offset = [&data](auto it, int64_t value) {
        // Use reverse iterator to move in reverse, make_reverse_iterator already returns an offset iterator, so no need for std::next()
        auto pos = std::make_reverse_iterator(it);
        // Moving an element size-1 times ends up in the same position
        int64_t amount = (-value)%(std::ssize(data)-1);
        pos = advance(pos, amount, data.rbegin(), data.rend());
        // data.rend() maps to data.begin() which would make the splice insert the element at the begining of the list
        // when we move an element past the first element, we actually want it at the end of the list
        if (pos == data.rend())
            return data.end();
        else
            return pos.base();
    };

    // Go over all the elements in the stable order
    std::list<int64_t>::iterator zero;
    for (auto it : order) {
        if (*it > 0) {
            // Use the positive offset for positive numbers to calculate the splice position
            data.splice(positive_offset(it, *it), data, it);
        } else if (*it < 0) {
            // Use the negative offset for negative numbers to calculate the splice position
            data.splice(negative_offset(it, *it), data, it);
        } else {
            zero = it;
        }
    }
    return zero;
}



int64_t decrypt(std::list<int64_t>& data) {
    // Remember the order, so we can iterate as the order of elements changes
    std::vector<std::list<int64_t>::iterator> order;
    std::generate_n(std::back_inserter(order), data.size(), [it = data.begin()]() mutable {
        return it++;
    });

    // Shuffle the elements
    std::list<int64_t>::iterator zero = shuffle_once(data, order);

    // Pick-out the elements 1000, 2000 and 3000 offset from the position of zero
    std::list<int64_t>::iterator it = zero;
    int64_t sum = 0;
    it = advance(it, 1000, data.begin(), data.end());
    sum += *it;
    it = advance(it, 1000, data.begin(), data.end());
    sum += *it;
    it = advance(it, 1000, data.begin(), data.end());
    sum += *it;

    // Return the sum
    return sum;
}

int64_t true_decrypt(std::list<int64_t>& data) {
    // Multiply by decryption key
    std::ranges::transform(data, data.begin(), 
        std::bind_front(std::multiplies<int64_t>{}, 811589153));

    // Remember the order, so we can iterate as the order of elements changes
    std::vector<std::list<int64_t>::iterator> order;
    std::generate_n(std::back_inserter(order), data.size(), [it = data.begin()]() mutable {
        return it++;
    });

    // Shuffle the elements ten times
    std::list<int64_t>::iterator zero;
    for (auto rep : std::views::iota(0, 10)) {
        zero = shuffle_once(data, order);
    }

    // Pick-out the elements 1000, 2000 and 3000 offset from the position of zero
    std::list<int64_t>::iterator it = zero;
    int64_t sum = 0;
    it = advance(it, 1000, data.begin(), data.end());
    sum += *it;
    it = advance(it, 1000, data.begin(), data.end());
    sum += *it;
    it = advance(it, 1000, data.begin(), data.end());
    sum += *it;

    // Return the sum
    return sum;
}

}
module;

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <variant>
#include <compare>

export module day13.signal;

export namespace day13::signal {
struct Node {
    Node* parent;
    std::vector<Node*> content;
    std::optional<int> value;
    
    Node(Node* parent, int value) : parent(parent), value(value) {}
    Node(Node* parent) : parent(parent) {}

    friend auto operator<=>(int value, const Node& right) {
        // Terminal state for recursion, comparing two values.
        if (right.value)
            return value <=> *right.value;
        // Comparing value against and empty list.
        if (right.content.empty())
            return std::strong_ordering::greater;
        // Comparing a value against a list, recurse.
        auto cmp = value <=> *right.content[0];
        if (std::is_eq(cmp)) {
            // X <=> [X,...]
            if (right.content.size() == 1)
                return std::strong_ordering::equal;
            return std::strong_ordering::less;
        }
        return cmp;
    }

    friend auto operator<=>(const Node& left, int value) {
        // Symmetrics situation.
        auto cmp = value <=> left;
        if (std::is_lt(cmp))
            return std::strong_ordering::greater;
        if (std::is_gt(cmp))
            return std::strong_ordering::less;
        return std::strong_ordering::equal;
    }

    friend auto operator<=>(const Node& left, const Node& right) {
        if (left.value && right.value)
            return *left.value <=> *right.value;
        if (left.value)
            return *left.value <=> right;
        if (right.value)
            return left <=> *right.value;

        // std::lexicographical_compare_three_way (not yet implemented in libc++)
        auto it_left = left.content.begin();
        auto it_right = right.content.begin();
        while (true) {
            if (it_left == left.content.end() && it_right == right.content.end())
                return std::strong_ordering::equal;
            if (it_left == left.content.end())
                return std::strong_ordering::less;
            if (it_right == right.content.end())
                return std::strong_ordering::greater;
            auto cmp = **it_left <=> **it_right;
            if (!std::is_eq(cmp))
                return cmp;
            it_left++;
            it_right++;
        }
        return std::strong_ordering::equal;
    }
};

struct Packet {
    std::vector<std::unique_ptr<Node>> storage;
    Node* root = nullptr;

    Packet() = default;
    Packet(int divider) {
        root = new_list_node(nullptr);
        new_value_node(new_list_node(root), divider);
    }

    auto operator<=>(const Packet& other) const {
        return *root <=> *other.root;
    }

    Node* new_value_node(Node* parent, int v) {
        storage.push_back(std::make_unique<Node>(parent, v));
        if (parent != nullptr)
            parent->content.push_back(storage.back().get());
        return storage.back().get();
    }
    Node* new_list_node(Node* parent) {
        storage.push_back(std::make_unique<Node>(parent));
        if (parent != nullptr)
            parent->content.push_back(storage.back().get());
        return storage.back().get();
    }

    friend std::istream& operator>>(std::istream& s, Packet& signal) {
        Node *current = nullptr;
        while (s) {
            if (std::isdigit(s.peek())) {
                int v;
                s >> v;
                signal.new_value_node(current, v);
            } else if (s.peek() == '[') {
                s.get();
                Node *next = signal.new_list_node(current);
                if (current == nullptr)
                    signal.root = next;
                current = next;
            } else if (s.peek() == ']') {
                s.get();
                current = current->parent;
                // Closing bracket for the outer node == end of packet.
                if (current == nullptr)
                    return s;
            } else {
                s.get();
            }
        }
        return s;
    }
};

int decoder_key(std::vector<Packet>& packets) {
    std::sort(packets.begin(), packets.end());

    auto first = std::lower_bound(packets.begin(), packets.end(), Packet(2));
    auto second = std::upper_bound(packets.begin(), packets.end(), Packet(6));

    int key = std::distance(packets.begin(), first) + 1;
    key *= std::distance(packets.begin(), second) + 2;
    return key;
}
}
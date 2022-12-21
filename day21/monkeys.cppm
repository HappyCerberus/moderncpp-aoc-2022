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

export module day21.monkeys;

export namespace day21::monkeys {
struct Monkey {
    struct Op {
        char op;
        std::string left;
        std::string right;
    };

    Monkey(int64_t value) : job(value) {}
    Monkey(char op, std::string left, std::string right, std::function<int64_t(const std::string&)> query) : job(Op{op, std::move(left), std::move(right)}), query(std::move(query)) {}

    int64_t value() {
        if (std::holds_alternative<int64_t>(job))
            return std::get<int64_t>(job);

        auto op = std::get<Op>(job);
        int64_t left = query(op.left);
        int64_t right = query(op.right);
        switch (op.op) {
            case '+': return left+right;
            case '-': return left-right;
            case '*': return left*right;
            case '/': return left/right;
        }
        // unreachable
        return 0;
    }
    std::variant<int64_t,Op> job;
    std::function<int64_t(const std::string&)> query;
};

struct Riddle {
    struct Storage {
        std::unordered_map<std::string, Monkey> monkey_lookup;
    };

    Riddle() : db(std::make_unique<Storage>()) {}

    friend std::istream& operator>>(std::istream& s, Riddle& riddle) {
        std::string monkey;
        while (s >> monkey) {
            monkey.resize(4); // strip the ':'
            while (std::isspace(s.peek())) s.get();
            if (std::isdigit(s.peek())) {
                int64_t value;
                if (s >> value)
                    riddle.db->monkey_lookup.insert_or_assign(monkey, Monkey(value));
            } else {
                std::string left, right;
                char op;
                if (s >> left >> op >> right) {
                    riddle.db->monkey_lookup.insert_or_assign(monkey, Monkey(op, left, right, 
                        [db = riddle.db.get()](const std::string& monkey){
                            return db->monkey_lookup.at(monkey).value();
                        }));
                }
            }
        }
        return s;
    }

    int64_t root() {
        return db->monkey_lookup.at("root").value();
    }

    std::unique_ptr<Storage> db;
};

struct ProperRiddle {
    struct Storage {
        std::unordered_map<std::string, Monkey> monkey_lookup;
        int64_t input;
    };

    ProperRiddle() : db(std::make_unique<Storage>()) {}

    friend std::istream& operator>>(std::istream& s, ProperRiddle& riddle) {
        std::string monkey;
        while (s >> monkey) {
            monkey.resize(4); // strip the ':'
            while (std::isspace(s.peek())) s.get();
            if (std::isdigit(s.peek())) {
                int64_t value;
                if (s >> value)
                    riddle.db->monkey_lookup.insert_or_assign(monkey, Monkey(value));
            } else {
                std::string left, right;
                char op;
                if (s >> left >> op >> right) {
                    // Override the operator for root
                    // Return the difference (we aim for the difference to be zero)
                    if (monkey == "root") op = '-';

                    riddle.db->monkey_lookup.insert_or_assign(monkey, Monkey(op, left, right, 
                        [db = riddle.db.get()](const std::string& monkey){
                            // For "humn", instead fetch the specified value
                            if (monkey == "humn")
                                return db->input;
                            return db->monkey_lookup.at(monkey).value();
                        }));
                }
            }
        }
        return s;
    }

    int64_t search() {
        // Assume the expression is monotonic
        int64_t max = INT64_MAX/1'000'000;
        int64_t min = INT64_MIN/1'000'000;

        db->input = min;
        int64_t min_result = db->monkey_lookup.at("root").value();
        db->input = max;
        int64_t max_result = db->monkey_lookup.at("root").value();
        // The starting range must cross zero
        if (std::signbit(min_result) == std::signbit(max_result))
            throw std::runtime_error("Starting range is insufficient.");

        while (true) {
            // Get the midpoint and calculate the value for it
            int64_t mid = std::midpoint(max,min);
            db->input = mid;
            int64_t mid_result = db->monkey_lookup.at("root").value();
            if (mid_result == 0)
                return mid;

            // Update to use whichever interval [min,mid], [mid,max] that crosses zero.
            if (std::signbit(min_result) == std::signbit(mid_result))
                min = mid;
            else
                max = mid;
        }
        // unreachable
        return 0;
    }

    std::unique_ptr<Storage> db;
};

}
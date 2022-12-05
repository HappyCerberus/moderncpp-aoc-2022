import day05.crates;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

std::vector<std::vector<char>> parse_initial_state(std::istream& s) {
    std::vector<std::vector<char>> result;
    std::string line;
    bool done = false;
    // process input line-by-line
    while (!done && std::getline(s, line)) {
        // scan each line
        for (size_t i = 0; i < line.length(); i++) {
            // if there is a digit in the input, this is terminal line
            if (std::isdigit(line[i])) { done = true; break; }
            // if it's not an upper-case letter, skip over it
            if (!std::isupper(line[i])) continue;
            // calculate the destination stack
            size_t stack = (i-1)/4;
            // and insert
            if (result.size() <= stack) result.resize(stack+1);
            result[stack].push_back(line[i]);
        }
    }
    // flip the vectors for ease of use
    std::ranges::for_each(result, std::ranges::reverse);
    // eat the empty-line after the initial state
    std::getline(s, line);
    return result;
}

int test() {
    using namespace day05::crates;
    std::stringstream s(R"(    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3 

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2)");
    auto state = parse_initial_state(s);
    assert(state.size() == 3);
    auto l1 = {'Z', 'N'};
    assert(std::ranges::equal(state[0],l1));
    auto l2 = {'M', 'C', 'D'};
    assert(std::ranges::equal(state[1],l2));
    auto l3 = {'P'};
    assert(std::ranges::equal(state[2],l3));
    
    std::vector<Order> orders;
    std::ranges::copy(std::views::istream<Order>(s), std::back_inserter(orders));
    assert(orders.size() == 4);
    auto cmp = {Order{1, 2, 1}, Order{3, 1, 3}, Order{2, 2, 1}, Order{1, 1, 2}};
    assert(std::ranges::equal(orders,cmp));

    auto state_copy = state;
    crateMover9000(state, orders);
    assert(top_crates(state) == "CMZ");
    crateMover9001(state_copy, orders);
    assert(top_crates(state_copy) == "MCD");

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day05::crates;
    std::vector<std::string> data;
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }
    
    std::vector<std::vector<char>> state = parse_initial_state(file);
    std::vector<Order> orders;
    std::ranges::copy(std::views::istream<Order>(file), std::back_inserter(orders));
    auto state_copy = state;
    crateMover9000(state, orders);
    std::cout << "After processing orders using crateMover9000, the top crates are " << top_crates(state) << "\n";

    crateMover9001(state_copy, orders);
    std::cout << "After processing orders using crateMover9001, the top crates are " << top_crates(state_copy) << "\n";
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
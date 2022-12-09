import day09.bridge;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day09::bridge;
    std::vector<Order> data {
        { Direction::Right, 4 },
        { Direction::Up, 4 },
        { Direction::Left, 3 },
        { Direction::Down, 1 },
        { Direction::Right, 4 },
        { Direction::Down, 1 },
        { Direction::Left, 5 },
        { Direction::Right, 2 },
    };

    assert(simulate_motions(data) == 13);
    assert(simulate_long_motions(data) == 1);

    std::vector<Order> larger{
        { Direction::Right, 5 },
        { Direction::Up, 8 },
        { Direction::Left, 8 },
        { Direction::Down, 3 },
        { Direction::Right, 17 },
        { Direction::Down, 10 },
        { Direction::Left, 25 },
        { Direction::Up, 20 },
    };

    assert(simulate_long_motions(larger) == 36);

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day09::bridge;
    
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::vector<Order> data;
    std::ranges::copy(std::views::istream<Order>(file), 
        std::back_inserter(data));
    
    std::cout << "The tail has visited " << simulate_motions(data) << " positions.\n";
    std::cout << "When simulating a longer rope, the tail has visited " << simulate_long_motions(data) << " positions.\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
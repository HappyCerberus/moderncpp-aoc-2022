import day17.tetris;

#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day17::tetris;
    assert(simulate(">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>", 2022) == 3068);
    assert(simulate_with_cycle_detection(">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>", 1000000000000) == 1514285714288);
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day17::tetris;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::string pattern;
    std::getline(file, pattern);

    std::cout << "The tower will be " << simulate(pattern, 2022) << " units tall after dropping 2022 pieces.\n";
    std::cout << "The tower will be " << simulate_with_cycle_detection(pattern, 1000000000000) << " units tall after dropping 1000000000000 pieces.\n";
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
import day23.elves;

#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>
#include <list>

int test() {
    using namespace day23::elves;

    std::stringstream s(R"(....#..
..###.#
#...#.#
.#...##
#.###..
##.#.##
.#..#..)");

    Elves elves;
    s >> elves;
    for (int r : std::views::iota(0, 10))
        elves.round();
    std::cout << "Empty tiles covered by elves after 10 rounds " << elves.empty_tiles() << "\n";

    int round = 10;
    while (elves.round())
        ++round;
    std::cout << "The first round when no elf moved was " << round+1 << "\n";

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day23::elves;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    Elves elves;
    file >> elves;
    for (int r : std::views::iota(0, 10))
        elves.round();
    std::cout << "Empty tiles covered by elves after 10 rounds " << elves.empty_tiles() << "\n";

    int round = 10;
    while (elves.round())
        ++round;
    std::cout << "The first round when no elf moved was " << round+1 << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
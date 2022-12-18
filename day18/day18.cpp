import day18.obsidian;

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
    using namespace day18::obsidian;
    std::stringstream s(R"(2,2,2
1,2,2
3,2,2
2,1,2
2,3,2
2,2,1
2,2,3
2,2,4
2,2,6
1,2,5
3,2,5
2,1,5
2,3,5)");

    std::vector<Cube> cubes;
    std::ranges::copy(std::ranges::views::istream<Cube>(s), std::back_inserter(cubes));

    assert(count_uncovered_sides(cubes) == 64);
    assert(external_surface(cubes) == 58);
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day18::obsidian;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::vector<Cube> cubes;
    std::ranges::copy(std::ranges::views::istream<Cube>(file), std::back_inserter(cubes));

    std::cout << "The total surface is " << count_uncovered_sides(cubes) << "\n";
    std::cout << "The total external surface is " << external_surface(cubes) << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
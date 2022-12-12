import day12.hill;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day12::hill;
    std::vector<std::string> map = {
        "Sabqponm",
        "abcryxxl",
        "accszExk",
        "acctuvwj",
        "abdefghi"
    };

    assert(hill_climb(map, Pos{0,0}, Pos{2,5}) == 31);
    assert(hill_plan(map, Pos{2,5}) == 29);

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day12::hill;
    
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }
    std::vector<std::string> map;
    std::string line;
    Pos start, end;
    while(std::getline(file, line)) {
        for (int64_t idx = 0; idx < std::ssize(line); idx++) {
            if (line[idx] == 'S') start = {std::ssize(map), idx};
            if (line[idx] == 'E') end = {std::ssize(map), idx};
        }
        map.push_back(line);
    }

    std::cout << "The shortest hill-climb is " << hill_climb(map, start, end) << " steps long.\n";
    std::cout << "The shortest path to any ground level location is " << hill_plan(map, end) << " steps long.\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
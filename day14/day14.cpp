import day14.sand;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day14::sand;
    std::stringstream s(R"(498,4 -> 498,6 -> 496,6
503,4 -> 502,4 -> 502,9 -> 494,9)");

    Map map;
    std::string line;
    while (getline(s, line))
        map.add_scan(line);
    
    Map map_copy = map;
    size_t cnt = 0;
    while (map.drop_sand_into_void()) cnt++;
    assert(cnt == 24);

    cnt = 0;
    while (map_copy.drop_sand_onto_floor()) cnt++;
    assert(cnt == 93);

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day14::sand;
    
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    Map map;
    std::string line;
    while (getline(file, line))
        map.add_scan(line);

    Map floor_map = map;
    
    size_t cnt = 0;
    while (map.drop_sand_into_void()) cnt++;

    size_t floor_cnt = 0;
    while (floor_map.drop_sand_onto_floor()) floor_cnt++;


    std::cout << "Only the first " << cnt << " grains of sand will be captured by reservoir.\n";
    std::cout << "The sand will stop falling after " << floor_cnt << " grains.\n";


    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
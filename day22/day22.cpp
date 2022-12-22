import day22.map;

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
    using namespace day22::map;
    std::stringstream s(R"(        ...#
        .#..
        #...
        ....
...#.......#
........#...
..#....#....
..........#.
        ...#....
        .....#..
        .#......
        ......#.

10R5L5R10L4R5L5)");

    Map map;
    s >> map;
    std::cout << map.follow_directions() << "\n";

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day22::map;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    Map map;
    file >> map;
    std::cout << "The password for this map is " << map.follow_directions() << "\n";
    TiledMap tiled(map, 50);
    std::cout << "The password when mapped to a cube is " << tiled.follow_directions() << "\n";
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
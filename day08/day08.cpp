import day08.trees;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day08::trees;

    std::vector<std::string> data{
        "30373",
        "25512",
        "65332",
        "33549",
        "35390"
    };

    assert(count_visible(data) == 21);
    assert(best_scenic_score(data) == 8);

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day08::trees;
    
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::vector<std::string> data;
    std::ranges::copy(std::views::istream<std::string>(file), 
        std::back_inserter(data));
    
    std::cout << "The number of visible trees from the map borders is " << count_visible(data) << "\n";
    std::cout << "The best scenic score achievable on the map is " << best_scenic_score(data) << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
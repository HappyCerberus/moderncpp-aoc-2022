import day04.cleaning;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>

int test() {
    using namespace day04::cleaning;
    std::vector<std::pair<Interval,Interval>> test_data{
        {{2,4},{6,8}},
        {{2,3},{4,5}},
        {{5,7},{7,9}},
        {{2,8},{3,7}},
        {{6,6},{4,6}},
        {{2,6},{4,8}}
    };

    assert(contains(test_data) == 2);
    assert(overlaps(test_data) == 4);
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day04::cleaning;
    std::vector<std::pair<Interval,Interval>> data;
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }
    char delimiter;
    Interval left, right;
    while (file >> left >> delimiter >> right) {
        data.push_back(std::make_pair(left,right));
    }
    std::cout << "The number of elves that would do completely redundant cleaning is " << contains(data) << "\n";
    std::cout << "The number of elves that would do partially redudant cleaning is " << overlaps(data) << "\n";
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
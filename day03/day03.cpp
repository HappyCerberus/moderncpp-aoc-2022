import day03.rucksacks;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>

int test() {
    std::vector<std::string> test_data{
        "vJrwpWtwJgWrhcsFMMfFFhFp",
        "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL",
        "PmmdzqPrVvPwwTWBwg",
        "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn",
        "ttgJtRGJQctTZtZT",
        "CrZsJsPPZsGzwwsLwLmpwMDw",
    };
    assert(day03::rucksacks::overlap(test_data) == 157);
    assert(day03::rucksacks::badges(test_data) == 70);

    return 0;
}

int parse_and_run(std::string_view path) {
    std::vector<std::string> data;
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }
    std::string line;
    while (std::getline(file, line)) {
        data.push_back(line);
    }
    std::cout << "The sum of the priorities of items that need to be moved is " << day03::rucksacks::overlap(data) << "\n";
    std::cout << "The sum of the priorities of badges is " << day03::rucksacks::badges(std::move(data)) << "\n";
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
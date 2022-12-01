import day01.calories;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>

int test() {
    std::vector<std::string> test_data{
        "1000", "2000", "3000", "",
        "4000", "",
        "5000", "6000", "",
        "7000", "8000", "9000", "",
        "10000"
    };
    assert(day01::calories::max_calories(test_data) == 24000);

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
    std::cout << "The elf with the maximum number of calorie is carrying " << day01::calories::max_calories(data) << " calories worth of food.\n";
    std::cout << "The top 3 elfs are carrying " << day01::calories::top_three(data) << " calories worth of food.\n";
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
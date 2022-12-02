import day02.tournament;

#include <vector>
#include <iostream>
#include <cassert>
#include <fstream>
#include <ranges>

int test() {
    std::vector<std::pair<char, char>> test_data{
        {'A','Y'},
        {'B','X'},
        {'C','Z'}
    };
    assert(day02::tournament::follow_choice(test_data) == 15);
    assert(day02::tournament::follow_result(test_data) == 12);

    return 0;
}

int parse_and_run(std::string_view path) {
    std::vector<std::pair<char,char>> parsed;
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }
    std::pair<char,char> current;
    while (file >> current.first >> current.second) {
        parsed.push_back(current);
    }
    std::cout << "The total score while following the strategy guide is " << day02::tournament::follow_choice(parsed) << "\n";
    std::cout << "The total score while following the proper strategy guide is " << day02::tournament::follow_result(parsed) << "\n";
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
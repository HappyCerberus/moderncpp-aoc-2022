import day06.tuning;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day06::tuning;
    std::string t1 = "bvwbjplbgvbhsrlpgdmjqwftvncz"; // 5 
    assert(start_of(t1) == 5);
    assert(start_of(t1, 14) == 23);
    std::string t2 = "nppdvjthqldpwncqszvftbrmjlhg"; // 6
    assert(start_of(t2) == 6);
    assert(start_of(t2, 14) == 23);
    std::string t3 = "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg"; // 10
    assert(start_of(t3) == 10);
    assert(start_of(t3, 14) == 29);
    std::string t4 = "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"; // 11
    assert(start_of(t4) == 11);
    assert(start_of(t4, 14) == 26);
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day06::tuning;
    std::vector<std::string> data;
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::string input;
    getline(file, input);
    std::cout << "We need to process " << start_of(input) << " characters before the first start-of-packet marker.\n";
    std::cout << "We need to process " << start_of(input, 14) << " characters before the first start-of-message marker.\n";
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
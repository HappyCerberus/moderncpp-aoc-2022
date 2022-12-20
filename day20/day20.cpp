import day20.decryption;

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
    using namespace day20::decryption;
    std::list<int64_t> data{1, 2, -3, 3, -2, 0, 4};
    std::list<int64_t> copy = data;
    std::cout << "Sum of coordinates is " << decrypt(data) << "\n";
    std::cout << "Sum of coordinates is " << true_decrypt(copy) << "\n";
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day20::decryption;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::list<int64_t> data;
    std::ranges::copy(std::views::istream<int64_t>(file), std::back_inserter(data));
    std::list<int64_t> copy = data;
    std::cout << "Sum of coordinates is " << decrypt(data) << "\n";
    std::cout << "Sum of coordinates is " << true_decrypt(copy) << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
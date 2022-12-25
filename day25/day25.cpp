import day25.snafu;

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
    using namespace day25::snafu;
    std::stringstream s(R"(1=-0-2
12111
2=0=
21
2=01
111
20012
112
1=-1=
1-12
12
1=
122)");

    std::vector<std::string> data;
    std::ranges::copy(std::views::istream<std::string>(s), std::back_inserter(data));
    std::cout << "The sum of the numbers is " << sum_snafu(data) << "\n";
    std::cout << "The SNAFU converted sum is " << to_snafu(sum_snafu(data)) << "\n";

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day25::snafu;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::vector<std::string> data;
    std::ranges::copy(std::views::istream<std::string>(file), std::back_inserter(data));
    std::cout << "The sum of the numbers is " << sum_snafu(data) << "\n";
    std::cout << "The SNAFU converted sum is " << to_snafu(sum_snafu(data)) << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
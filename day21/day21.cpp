import day21.monkeys;

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
    using namespace day21::monkeys;
    std::stringstream s(R"(root: pppw + sjmn
dbpl: 5
cczh: sllz + lgvd
zczc: 2
ptdq: humn - dvpt
dvpt: 3
lfqf: 4
humn: 5
ljgn: 2
sjmn: drzm * dbpl
sllz: 4
pppw: cczh / lfqf
lgvd: ljgn * ptdq
drzm: hmdt - zczc
hmdt: 32)");


    Riddle riddle;
    s >> riddle;
    assert(riddle.root() == 152);

    s.clear();
    s.seekg(0);
    ProperRiddle proper_riddle;
    s >> proper_riddle;
    assert(proper_riddle.search() == 301);

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day21::monkeys;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    Riddle riddle;
    file >> riddle;
    std::cout << "The answer to the monkeys' riddle is " << riddle.root() << "\n";

    file.clear();
    file.seekg(0);

    ProperRiddle proper_riddle;
    file >> proper_riddle;

    std::cout << "The actual answer to the monkeys' riddle is " << proper_riddle.search() << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
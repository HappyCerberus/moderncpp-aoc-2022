import day11.monkeys;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day11::monkeys;
    auto monkeys = std::make_unique<Monkeys>();
    monkeys->troop.push_back(Monkey{
        {79, 98},
        +[](int64_t v) -> int64_t { return v * 19; },
        +[](int64_t v) -> int64_t { if (v % 23 == 0) return 2; return 3; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {54, 65, 75, 74},
        +[](int64_t v) -> int64_t { return v + 6; },
        +[](int64_t v) -> int64_t { if (v % 19 == 0) return 2; return 0; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {79, 60, 97},
        +[](int64_t v) -> int64_t { return v * v; },
        +[](int64_t v) -> int64_t { if (v % 13 == 0) return 1; return 3; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {74},
        +[](int64_t v) -> int64_t { return v + 3; },
        +[](int64_t v) -> int64_t { if (v % 17 == 0) return 0; return 1; },
        monkeys.get()
    });
    for (auto i : std::views::iota(0,20))
        monkeys->round();
    assert(monkeys->monkey_business() == 10605);

    // Reset the monkeys:
    monkeys->troop.clear();
    monkeys->troop.push_back(Monkey{
        {79, 98},
        +[](int64_t v) -> int64_t { return v * 19; },
        +[](int64_t v) -> int64_t { if (v % 23 == 0) return 2; return 3; },
        monkeys.get(),
        23*19*13*17
    });
    monkeys->troop.push_back(Monkey{
        {54, 65, 75, 74},
        +[](int64_t v) -> int64_t { return v + 6; },
        +[](int64_t v) -> int64_t { if (v % 19 == 0) return 2; return 0; },
        monkeys.get(),
        23*19*13*17
    });
    monkeys->troop.push_back(Monkey{
        {79, 60, 97},
        +[](int64_t v) -> int64_t { return v * v; },
        +[](int64_t v) -> int64_t { if (v % 13 == 0) return 1; return 3; },
        monkeys.get(),
        23*19*13*17
    });
    monkeys->troop.push_back(Monkey{
        {74},
        +[](int64_t v) -> int64_t { return v + 3; },
        +[](int64_t v) -> int64_t { if (v % 17 == 0) return 0; return 1; },
        monkeys.get(),
        23*19*13*17
    });
    for (auto i : std::views::iota(0, 10000))
        monkeys->worried_round();
    assert(monkeys->monkey_business() == 2713310158);
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day11::monkeys;
    auto monkeys = std::make_unique<Monkeys>();
    monkeys->troop.push_back(Monkey{
        {65, 58, 93, 57, 66},
        +[](int64_t v) -> int64_t { return v * 7; },
        +[](int64_t v) -> int64_t { if (v % 19 == 0) return 6; return 4; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {76, 97, 58, 72, 57, 92, 82},
        +[](int64_t v) -> int64_t { return v + 4; },
        +[](int64_t v) -> int64_t { if (v % 3 == 0) return 7; return 5; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {90, 89, 96},
        +[](int64_t v) -> int64_t { return v * 5; },
        +[](int64_t v) -> int64_t { if (v % 13 == 0) return 5; return 1; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {72, 63, 72, 99},
        +[](int64_t v) -> int64_t { return v * v; },
        +[](int64_t v) -> int64_t { if (v % 17 == 0) return 0; return 4; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {65},
        +[](int64_t v) -> int64_t { return v + 1; },
        +[](int64_t v) -> int64_t { if (v % 2 == 0) return 6; return 2; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {97, 71},
        +[](int64_t v) -> int64_t { return v + 8; },
        +[](int64_t v) -> int64_t { if (v % 11 == 0) return 7; return 3; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {83, 68, 88, 55, 87, 67},
        +[](int64_t v) -> int64_t { return v + 2; },
        +[](int64_t v) -> int64_t { if (v % 5 == 0) return 2; return 1; },
        monkeys.get()
    });
    monkeys->troop.push_back(Monkey{
        {64, 81, 50, 96, 82, 53, 62, 92},
        +[](int64_t v) -> int64_t { return v + 5; },
        +[](int64_t v) -> int64_t { if (v % 7 == 0) return 3; return 0; },
        monkeys.get()
    });
    for (auto i : std::views::iota(0,20))
        monkeys->round();
    std::cout << "Monkey business " << monkeys->monkey_business() << "\n";

    // Reset the monkeys:
    monkeys->troop.clear();
    monkeys->troop.push_back(Monkey{
        {65, 58, 93, 57, 66},
        +[](int64_t v) -> int64_t { return v * 7; },
        +[](int64_t v) -> int64_t { if (v % 19 == 0) return 6; return 4; },
        monkeys.get(),
        19*3*13*17*2*11*5*7
    });
    monkeys->troop.push_back(Monkey{
        {76, 97, 58, 72, 57, 92, 82},
        +[](int64_t v) -> int64_t { return v + 4; },
        +[](int64_t v) -> int64_t { if (v % 3 == 0) return 7; return 5; },
        monkeys.get(),
        19*3*13*17*2*11*5*7
    });
    monkeys->troop.push_back(Monkey{
        {90, 89, 96},
        +[](int64_t v) -> int64_t { return v * 5; },
        +[](int64_t v) -> int64_t { if (v % 13 == 0) return 5; return 1; },
        monkeys.get(),
        19*3*13*17*2*11*5*7
    });
    monkeys->troop.push_back(Monkey{
        {72, 63, 72, 99},
        +[](int64_t v) -> int64_t { return v * v; },
        +[](int64_t v) -> int64_t { if (v % 17 == 0) return 0; return 4; },
        monkeys.get(),
        19*3*13*17*2*11*5*7
    });
    monkeys->troop.push_back(Monkey{
        {65},
        +[](int64_t v) -> int64_t { return v + 1; },
        +[](int64_t v) -> int64_t { if (v % 2 == 0) return 6; return 2; },
        monkeys.get(),
        19*3*13*17*2*11*5*7
    });
    monkeys->troop.push_back(Monkey{
        {97, 71},
        +[](int64_t v) -> int64_t { return v + 8; },
        +[](int64_t v) -> int64_t { if (v % 11 == 0) return 7; return 3; },
        monkeys.get(),
        19*3*13*17*2*11*5*7
    });
    monkeys->troop.push_back(Monkey{
        {83, 68, 88, 55, 87, 67},
        +[](int64_t v) -> int64_t { return v + 2; },
        +[](int64_t v) -> int64_t { if (v % 5 == 0) return 2; return 1; },
        monkeys.get(),
        19*3*13*17*2*11*5*7
    });
    monkeys->troop.push_back(Monkey{
        {64, 81, 50, 96, 82, 53, 62, 92},
        +[](int64_t v) -> int64_t { return v + 5; },
        +[](int64_t v) -> int64_t { if (v % 7 == 0) return 3; return 0; },
        monkeys.get(),
        19*3*13*17*2*11*5*7
    });
    for (auto i : std::views::iota(0,10000))
        monkeys->worried_round();
    std::cout << "Worried monkey business " << monkeys->monkey_business() << "\n";
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
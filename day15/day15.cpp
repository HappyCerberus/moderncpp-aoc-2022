import day15.sensors;

#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day15::sensors;
    std::vector<Sensor> sensors {
        {{2, 18}, {-2, 15}},
        {{9, 16}, {10, 16}},
        {{13, 2}, {15, 3}},
        {{12, 14}, {10, 16}},
        {{10, 20}, {10, 16}},
        {{14, 17}, {10, 16}},
        {{8, 7}, {2, 10}},
        {{2, 0}, {2, 10}},
        {{0, 11}, {2, 10}},
        {{20, 14}, {25, 17}},
        {{17, 20}, {21, 22}},
        {{16, 7}, {15, 3}},
        {{14, 3}, {15, 3}},
        {{20, 1}, {15, 3}}
    };

    assert(sensors[6].coverage_at_row(-3) == std::nullopt);
    assert((sensors[6].coverage_at_row(-2) == Interval{8,8}));
    assert((sensors[6].coverage_at_row(7) == Interval{-1,17}));
    assert(count_covered_spaces_at_coord(sensors, 10) == 26);
    assert(tuning_frequency(sensors, 0, 20) == 56000011);
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day15::sensors;
    
    std::vector<Sensor> sensors {
        {{193758,2220950}, {652350, 2000000}},
        {{3395706, 3633894}, {3404471, 3632467}},
        {{3896022, 3773818}, {3404471, 3632467}},
        {{1442554, 1608100}, {652350, 2000000}},
        {{803094, 813675}, {571163, 397470}},
        {{3491072, 3408908}, {3404471, 3632467}},
        {{1405010, 486446}, {571163, 397470}},
        {{3369963, 3641076}, {3404471, 3632467}},
        {{3778742, 2914974}, {4229371, 3237483}},
        {{1024246, 3626229}, {2645627, 3363491}},
        {{3937091, 2143160}, {4229371, 3237483}},
        {{2546325, 2012887}, {2645627, 3363491}},
        {{3505386, 3962087}, {3404471, 3632467}},
        {{819467, 239010}, {571163, 397470}},
        {{2650614, 595151}, {3367919, -1258}},
        {{3502942, 6438}, {3367919, -1258}},
        {{3924022, 634379}, {3367919, -1258}},
        {{2935977, 2838245}, {2645627, 3363491}},
        {{1897626, 7510}, {3367919, -1258}},
        {{151527, 640680}, {571163, 397470}},
        {{433246, 1337298}, {652350, 2000000}},
        {{2852855, 3976978}, {3282750, 3686146}},
        {{3328398, 3645875}, {3282750, 3686146}},
        {{3138934, 3439134}, {3282750, 3686146}},
        {{178, 2765639}, {652350, 2000000}},
        {{3386231, 3635056}, {3404471, 3632467}},
        {{3328074, 1273456}, {3367919, -1258}},
        {{268657, 162438}, {571163, 397470}}};

    int64_t y_coord = 2000000;
    std::cout << "There are " << count_covered_spaces_at_coord(sensors, y_coord) << " spaces that cannot contain a beacon at y_coord " << y_coord << "\n";
    std::cout << "The tuning frequency of the distress beacon is " << tuning_frequency(sensors, 0, 4000000) << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
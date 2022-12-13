import day13.signal;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day13::signal;
    std::stringstream s(R"([1,1,3,1,1]
[1,1,5,1,1]

[[1],[2,3,4]]
[[1],4]

[9]
[[8,7,6]]

[[4,4],4,4]
[[4,4],4,4,4]

[7,7,7,7]
[7,7,7]

[]
[3]

[[[]]]
[[]]

[1,[2,[3,[4,[5,6,7]]]],8,9]
[1,[2,[3,[4,[5,6,0]]]],8,9])");

    Packet a, b;
    int idx = 1;
    int sum = 0;
    std::vector<Packet> packets;
    while (s >> a >> b) {
        if (a < b) {
            sum += idx;
        }
        idx++;
        packets.push_back(std::move(a));
        packets.push_back(std::move(b));
    }
    assert(sum == 13);
    assert(decoder_key(packets) == 140);
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day13::signal;
    
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    Packet a, b;
    int idx = 1;
    int sum = 0;
    std::vector<Packet> packets;
    while (file >> a >> b) {
        if (a < b)
            sum += idx;
        idx++;
        packets.push_back(std::move(a));
        packets.push_back(std::move(b));
    }
    
    std::cout << "The sum of indices that are already in order is " << sum << "\n";
    std::cout << "The decoder key is " << decoder_key(packets) << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
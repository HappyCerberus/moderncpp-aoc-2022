import day07.terminal;

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day07::terminal;
    std::stringstream s(R"($ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k)");
    auto tree = parse_output(s);
    assert(visit_and_sum_up(&tree->root) == 95437);
    size_t used = tree->root.recursive_size();
    if (70000000 - used < 30000000) {
        size_t goal = 30000000 - (70000000 - used);
        assert(find_smallest_but_sufficient(&tree->root, goal) == 24933642);
    }
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day07::terminal;
    std::vector<std::string> data;
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    auto tree = parse_output(file);
    std::cout << "The sum of the sizes of small sized directories is " << visit_and_sum_up(&tree->root) << "\n";
    size_t used = tree->root.recursive_size();
    if (70000000 - used < 30000000) {
        size_t goal = 30000000 - (70000000 - used);
        std::cout << "The size of the directory that needs to be deleted is " << find_smallest_but_sufficient(&tree->root, goal) << "\n";
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}
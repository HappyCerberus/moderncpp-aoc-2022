module;

#include <unordered_map>
#include <string>
#include <iostream>
#include <ranges>
#include <numeric>

export module day07.terminal;

// Local helpers, since they are not exported, they have local linkage.
constexpr inline auto values = std::views::transform([](auto &p) { return p.second; });

void skipws(std::istream& s) {
    while (std::isspace(s.peek())) s.get();
}

export namespace day07::terminal {

struct Directory {
    // Recursive size of the directory.
    size_t recursive_size();
    // Parse content of directory from "$ ls".
    friend std::istream& operator>>(std::istream& s, Directory& dir);

    // Required for "$ cd ..".
    Directory *parent = nullptr;
    // Required for "$ cd subdirectory".
    std::unordered_map<std::string, Directory*> subdirs;
    // Information about files.
    std::unordered_map<std::string, size_t> files;
private:
    // Cache, so we do not repeatedly re-calculate the size.
    std::optional<size_t> recursive_size_;
};

struct Tree {
    // Implementation of "$ cd subdirectory", also constructs the directory.
    Directory* cd(Directory* current, std::string dir);
    // Directory root.
    Directory root;
    // Storage for all Directories.
    std::vector<std::unique_ptr<Directory>> storage;
};


size_t Directory::recursive_size() {
    // If we have a cached value, we can simply return it.
    if (recursive_size_) return *recursive_size_;

    // Normally we would use std::views::values to get std::pair::second,
    // but libc++ has not implemented this view yet.

    // Sizes of files in this directory.
    auto fsz = files | values | std::views::common;
    // Sizes of subdirectories in this directory.
    auto dsz = subdirs | values | std::views::transform(&Directory::recursive_size) | std::views::common;
    // Sum up the sizes and return.
    recursive_size_ = std::reduce(fsz.begin(), fsz.end()) + std::reduce(dsz.begin(), dsz.end());
    return *recursive_size_;
}

std::istream& operator>>(std::istream& s, Directory& dir) {
    // Read until the next "$ ls" or "$ cd" or end of input
    while (s.peek() != '$' && s) {
        if (std::isdigit(s.peek())) {
            // Parse a file-size and store.
            size_t file_size;
            std::string filename;
            s >> file_size >> filename;
            dir.files[filename] = file_size;
            skipws(s); // Skip the trailing whitespace.
        } else {
            // Since we handle directory as we enter them using cd, we can ignore them in the ls output.
            std::string line;
            std::getline(s, line);
        }
    }
    return s;
}

Directory* Tree::cd(Directory* current, std::string dir) {
    if (dir == "/") return &root;
    if (dir == "..") return current->parent;

    // Directory already exists.
    if (current->subdirs[dir] != nullptr)
        return current->subdirs[dir];
    
    // Create and set-up the directory.
    auto &sub_dir = storage.emplace_back(std::make_unique<Directory>());
    sub_dir->parent = current;
    current->subdirs[dir] = sub_dir.get();
    return current->subdirs[dir];
}

std::unique_ptr<Tree> parse_output(std::istream& s) {
    auto tree = std::make_unique<Tree>();
    Directory *current = &tree->root;
    current->parent = current;

    // Read each command:
    while (s.peek() == '$') {
        char c;
        std::string cmd;
        // read the '$' token and the command
        s >> c >> cmd;
        if (cmd == "cd") {
            std::string where_to;
            s >> where_to;
            current = tree->cd(current, where_to);
            skipws(s);
        } else if (cmd == "ls") {
            s >> *current;
        }
    }
    return tree;
}

size_t visit_and_sum_up(Directory *current) {
    // Visit sub-directories and sum up.
    auto dsz = current->subdirs | values | std::views::transform(visit_and_sum_up) | std::views::common;
    size_t sum = std::reduce(dsz.begin(), dsz.end());
    // If this directory is less than 100000, also add it to the sum:
    if (current->recursive_size() <= 100000)
        sum += current->recursive_size();
    return sum;
}

size_t find_smallest_but_sufficient(Directory *current, size_t goal) {
    // Visit sub-directories, and filter out directories that are not large enough.
    auto dsz = current->subdirs | values | std::views::transform([goal](Directory *current) {
        return find_smallest_but_sufficient(current, goal);
    }) | std::views::filter([goal](size_t value) { return value >= goal; });
    // If none of the subdirectories is large enough return the size of this directory.
    if (dsz.empty()) return current->recursive_size();
    // Otherwise, return the smallest subdirectory (large enough).
    return std::ranges::min(dsz);
}

}
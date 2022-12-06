module;

#include <string>
#include <unordered_map>

export module day06.tuning;

export namespace day06::tuning {
size_t start_of(const std::string& s, size_t offset = 4) {
    // To remember the seen frequency of characters:
    std::unordered_map<char,size_t> cnt;
    // Iterate over the string:
    size_t i = 0;
    for (; i < s.length(); i++) {
        // Increase frequency for the current character
        ++cnt[s[i]];
        // Decrease the frequency for the character leaving the window
        if (i >= offset) {
            --cnt[s[i-offset]];
            // If the frequency is now zero, remove from the map
            if (cnt[s[i-offset]] == 0)
                cnt.erase(s[i-offset]);
        }
        // If we have offset elements, we know that each of them has frequency == 1
        if (cnt.size() == offset) break;
    }
    return i+1;
}
}
module;

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <variant>
#include <numeric>
#include <compare>
#include <sstream>
#include <ranges>
#include <map>
#include <set>
#include <list>
#include <queue>

export module day25.snafu;

export namespace day25::snafu {
// Convert a SNAFU to an integer
int64_t to_int(const std::string& num) {
    int64_t result = 0;
    int64_t current = 1;
    // libc++ had a problem with: for (auto v : num | std::views::reverse)
    for (auto it = num.rbegin(); it != num.rend(); it++) {
        switch (*it) {
            case '2': result += 2*current; break;
            case '1': result += 1*current; break;
            case '0': result += 0*current; break;
            case '-': result += -1*current; break;
            case '=': result += -2*current; break;
        }
        current *= 5;
    }
    return result;
}

// Convert an integer to SNAFU
std::string to_snafu(int64_t num) {
    std::string result;

    // figure out the maximum exponent
    int64_t exp = 1;
    // but also keep track the sum of the previous exponents
    int64_t sum_of_exp = 0;
    // Maximum number we can express is 2222...
    while (exp*2+2*sum_of_exp < num) {
        sum_of_exp += exp;
        exp *= 5;
    }

    while (num != 0 || exp != 0) {
        // The minimum number we can represent by putting '2' at this position is 2*exp-2*sum_of_exp, which corresponds to the 2==... pattern.
        // If our target is above or equal to this, we know we have to put '2' on this position.
        if (2*exp-2*sum_of_exp <= num) {
            result += '2';
            num -= 2*exp;
        // Same logic for '1'
        } else if (exp-2*sum_of_exp <= num) {
            result += '1';
            num -= exp;
        // Same logic for '0'
        } else if (-2*sum_of_exp <= num) {
            result += '0';
        // Same logic for '-'
        } else if (-1*exp-2*sum_of_exp <= num) {
            result += '-';
            // Note that now we are actually increasing our target
            num += exp;
        // Same logic for '='
        } else if (-2*exp-2*sum_of_exp <= num) {
            result += '=';
            num += 2*exp;
        }
        // Adjust exponent and the sum of exponents.
        exp /= 5;
        if (sum_of_exp != 0)
            sum_of_exp -= exp;
    }
    return result;
}

// Sum up converted SNAFUs
int64_t sum_snafu(const std::vector<std::string>& data) {
    return std::accumulate(data.begin(), data.end(), INT64_C(0), [](int64_t acc, const std::string& num) {
        return acc + to_int(num);
    });
}

}
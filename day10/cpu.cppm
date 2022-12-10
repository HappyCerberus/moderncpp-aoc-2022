module;

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <unordered_set>
#include <iostream>

export module day10.cpu;

export namespace day10::cpu {

// Noop instruction, does nothing, returns done at the end of the first tick.
struct InstrNoop {
    // Required to avoid the need for std::monostate
    InstrNoop() {}
    InstrNoop(int64_t*) {}
    bool tick() { return true; }
    friend std::istream& operator>>(std::istream& s, InstrNoop&) {
        // noop doesn't have any arguments
        return s;
    }
};
// Add instruction, does nothing for the first cycle, at the end of the second cycle adds the argument to the register X.
struct InstrAdd {
    InstrAdd(int64_t* X) : X(X) { }
    bool tick() {
        ++phase;
        if (phase != 2)
            return false;
        *X += arg;
        return true;
    }
    friend std::istream& operator>>(std::istream& s, InstrAdd& add) {
        // read the single argument
        return s >> add.arg;
    }
private:
    int32_t phase = 0;
    int64_t arg = 0;
    int64_t* X = nullptr;
};

struct Program {
    Program() : X(std::make_unique<int64_t>(1)) {}

    // state
    std::unique_ptr<int64_t> X;
    // instructions
    std::vector<std::variant<InstrNoop,InstrAdd>> instructions;

    int64_t run(std::vector<std::string>& screen, const std::vector<int64_t>& breakpoints) {
        int64_t tick = 0;
        int64_t sum = 0;
        auto it = breakpoints.begin();
        // Process all instructions
        for (auto& instruction : instructions) {
            do {
                ++tick;
                if (it != breakpoints.end() && *it == tick) {
                    // if we are at a tick listed in the breakpoints
                    // add tick * X to the sum
                    sum += tick*(*X);
                    ++it;
                }

                int64_t pos = (tick-1)%40;
                if (pos == 0)
                    // Every 40 ticks add a new line to the screen
                    screen.push_back("");
                if (pos >= *X-1 && pos <= *X+1)
                    // Sprite is visible
                    screen.back().push_back('#');
                else
                    // Sprite isn't visible
                    screen.back().push_back(' ');
            // Advance the current instruction, loop if it returns false.
            } while (!visit([](auto& i) { return i.tick(); }, instruction));
        }
        return sum;
    }

    friend std::istream& operator>>(std::istream& s, Program& program) {
        std::variant<InstrNoop, InstrAdd> decoded;
        std::string encoded;
        while (s >> encoded) {
            if (encoded == "addx") {
                decoded = InstrAdd(program.X.get());
            } else if (encoded == "noop") {
                decoded = InstrNoop(program.X.get());
            }
            visit([&s](auto& instr) {
                if (!(s >> instr)) throw std::runtime_error("Failed parsing program.");
            }, decoded);
            program.instructions.emplace_back(std::move(decoded));
        }
       
        return s;
    }
};

}
module;

#include <iostream>

export module day00.simple;

export namespace day00::simple {

auto greet() -> void {
   std::cout << "Hello World! I'm a simple interface-only module.\n";
}

}
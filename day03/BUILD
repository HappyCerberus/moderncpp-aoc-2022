load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day03",
   srcs = ["day03.cpp"],
   interfaces = {"rucksacks.cppm": "day03.rucksacks"},
   compile_flags = ["-std=c++20"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day03",
)
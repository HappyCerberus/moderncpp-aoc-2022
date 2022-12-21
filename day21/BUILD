load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day21",
   srcs = ["day21.cpp"],
   interfaces = {"monkeys.cppm": "day21.monkeys"},
   compile_flags = ["-std=c++20", "-Wall", "-Wextra", "-O3"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day21",
)
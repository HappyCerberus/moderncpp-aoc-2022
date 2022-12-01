load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day01",
   srcs = ["day01.cpp"],
   interfaces = {"calories.cppm": "day01.calories"},
   compile_flags = ["-std=c++20"],
   data = ["input.txt"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day01",
)
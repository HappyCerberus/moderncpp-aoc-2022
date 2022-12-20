load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day17",
   srcs = ["day17.cpp"],
   interfaces = {"tetris.cppm": "day17.tetris"},
   compile_flags = ["-std=c++20", "-Wall", "-Wextra", "-O3"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day17",
)
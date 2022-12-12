load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day12",
   srcs = ["day12.cpp"],
   interfaces = {"hill.cppm": "day12.hill"},
   compile_flags = ["-std=c++20", "-Wall", "-Wextra"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day12",
)
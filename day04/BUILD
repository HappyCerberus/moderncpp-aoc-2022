load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day04",
   srcs = ["day04.cpp"],
   interfaces = {"cleaning.cppm": "day04.cleaning"},
   compile_flags = ["-std=c++20"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day04",
)
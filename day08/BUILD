load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day08",
   srcs = ["day08.cpp"],
   interfaces = {"trees.cppm": "day08.trees"},
   compile_flags = ["-std=c++20"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day08",
)
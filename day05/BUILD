load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day05",
   srcs = ["day05.cpp"],
   interfaces = {"crates.cppm": "day05.crates"},
   compile_flags = ["-std=c++20"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day05",
)
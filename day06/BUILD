load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day06",
   srcs = ["day06.cpp"],
   interfaces = {"tuning.cppm": "tuning.crates"},
   compile_flags = ["-std=c++20"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day06",
)
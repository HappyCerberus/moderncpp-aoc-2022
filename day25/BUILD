load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day25",
   srcs = ["day25.cpp"],
   interfaces = {"snafu.cppm": "day25.snafu"},
   compile_flags = ["-std=c++20", "-Wall", "-Wextra", "-gdwarf-2"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day25",
)
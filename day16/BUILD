load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day16",
   srcs = ["day16.cpp"],
   interfaces = {"volcano.cppm": "day16.volcano"},
   compile_flags = ["-std=c++20", "-Wall", "-Wextra", "-O3"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day16",
)
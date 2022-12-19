load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day19",
   srcs = ["day19.cpp"],
   interfaces = {"robots.cppm": "day19.robots"},
   compile_flags = ["-std=c++20", "-Wall", "-Wextra", "-gdwarf-2", "-O3"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day19",
)
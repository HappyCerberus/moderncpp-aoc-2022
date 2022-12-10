load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day10",
   srcs = ["day10.cpp"],
   interfaces = {"cpu.cppm": "day10.cpu"},
   compile_flags = ["-std=c++20", "-gdwarf-2"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day10",
)
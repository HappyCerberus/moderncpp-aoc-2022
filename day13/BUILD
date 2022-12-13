load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day13",
   srcs = ["day13.cpp"],
   interfaces = {"signal.cppm": "day13.signal"},
   compile_flags = ["-std=c++20", "-Wall", "-Wextra", "-gdwarf-2"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day13",
)
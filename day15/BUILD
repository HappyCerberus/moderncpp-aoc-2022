load("@rules_ll//ll:defs.bzl", "ll_binary", "ll_compilation_database")

ll_binary(
   name = "day15",
   srcs = ["day15.cpp"],
   interfaces = {"sensors.cppm": "day15.sensors"},
   compile_flags = ["-std=c++20", "-Wall", "-Wextra", "-O3"],
)

ll_compilation_database(
    name = "compile_commands",
    config = "//:clang_tidy_config",
    target = ":day15",
)
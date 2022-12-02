# Modern C++ in Advent of Code 2022

This repository contains the code for the solutions of Advent of Code 2022 using Modern C++. The solutions themselves are explained in the daily posts accompanying this repository (linked below).

## Devcontainer

The repository uses a VSCode devcontainer environment with the Bazel build system. Opening the repository in VSCode with Docker installed on your machine will download and start a devcontainer that can then be used to build and run all the solutions.

Note that since the Bazel project relies on the `ll_rules` module to provide C++20 modules, the first time you try to run/build any of the examples, the module will build a patched version of clang (which will take a significant amount of time).

## Links to accompanying posts

|   | Twitter | LinkedIn | Mastodon | Medium | Substack |
|---|---      |---       |---       |---     |---       |
| Day 1 | [Link](https://twitter.com/SimonToth83/status/1598376580874735616) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7004142130919055360/) | [Link](https://mastodon.world/@simontoth/109439720217557936) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-1-e50a681ce1dc) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-1) |
| Day 2 | TBD | TBD | TBD | TBD | TBD |
# Modern C++ in Advent of Code 2022

This repository contains the code for the solutions of Advent of Code 2022 using Modern C++. The solutions themselves are explained in the daily posts accompanying this repository (linked below).

## Devcontainer

The repository uses a VSCode devcontainer environment with the Bazel build system. Opening the repository in VSCode with Docker installed on your machine will download and start a devcontainer that can then be used to build and run all the solutions.

Note that since the Bazel project relies on the [`rules_ll`](https://github.com/eomii/rules_ll) module to provide C++20 modules, the first time you try to run/build any of the examples, the module will build a patched version of clang (which will take a significant amount of time).

The IntelliSense in the VSCode C++ plugin currently struggles with C++20 modules ([bug](https://github.com/microsoft/vscode-cpptools/issues/10258)).

## Links to accompanying posts

|   | Twitter | LinkedIn | Mastodon | Medium | Substack |
|---|---      |---       |---       |---     |---       |
| Day 1 | [Link](https://twitter.com/SimonToth83/status/1598376580874735616) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7004142130919055360/) | [Link](https://mastodon.world/@simontoth/109439720217557936) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-1-e50a681ce1dc) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-1) |
| Day 2 | [Link](https://twitter.com/SimonToth83/status/1598739110826577929) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7004505083920994304/) | [Link](https://mastodon.world/@simontoth/109445382216203373) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-2-1f2c4dc73c94) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-2) |
| Day 3 | [Link](https://twitter.com/SimonToth83/status/1599101477359947776) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7004867048375787520/) | [Link](https://mastodon.world/@simontoth/109451044535863950) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-3-b09b622be4ae) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-3) |
| Day 4 | [Link](https://twitter.com/SimonToth83/status/1599463700506763265) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7005229259082326016/) | [Link](https://mastodon.world/@simontoth/109456707770126990) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-4-f43e503d3564) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-4) |
| Day 5 | [Link](https://twitter.com/SimonToth83/status/1599825940137246720) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7005591639335280640/) | [Link](https://mastodon.world/@simontoth/109462369331968903) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-5-b0b6729056bb) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-5) |
| Day 6 | [Link](https://twitter.com/SimonToth83/status/1600188649399881729) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7005954292687769600/) | [Link](https://mastodon.world/@simontoth/109468031729071088) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-6-d5e00f90b752) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-6) |
| Day 7 | [Link](https://twitter.com/SimonToth83/status/1600550961021435926) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7006316698714656768/) | [Link](https://mastodon.world/@simontoth/109473693847283708) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-7-2c3816e00cf) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-7) |
| Day 8 | [Link](https://twitter.com/SimonToth83/status/1600913104233271333) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7006678805025087488/) | [Link](https://mastodon.world/@simontoth/109479356492120426) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-8-3de2465e9f2e) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-8) |
| Day 9 | [Link](https://twitter.com/SimonToth83/status/1601275541444853763) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7007041276638281729/) | [Link](https://mastodon.world/@simontoth/109485018323807134) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-9-78558d0f7d20) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-9) |
| Day 10 | [Link](https://twitter.com/SimonToth83/status/1601637900650692609) | [Link](https://www.linkedin.com/feed/update/urn:li:activity:7007403717087887360/) | [Link](https://mastodon.world/@simontoth/109490680697585054) | [Link](https://medium.com/@simontoth/modern-c-in-advent-of-code-day-10-b4910e2ab63b) | [Link](https://simontoth.substack.com/p/modern-c-in-advent-of-code-day-10) |
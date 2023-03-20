// #include "algo.hpp"
#include "io.hpp"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

int get_rand() {
    return rand() % 20 + 1;
}

int main() {
    msc::bench_god     benchgod;
    vector<msc::robot> robots;

    for (int i = 0; i < ROBOT_N; ++i)
        robots.push_back(msc::robot(benchgod, i));

    msc::io io(robots, benchgod);

    while (1) {
        io.receive();
        for (int i = 0; i < ROBOT_N; ++i) {
            if (robots[i].is_idle())
                robots[i].start_task(0, get_rand());
            else
                robots[i].continue_task();
        }
        io.send();
    }

    return 0;
}

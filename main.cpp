// #include "algo.hpp"
#define DEBUG
#include "data_types.hpp"
#include "io.hpp"
#include "algo.hpp"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

// int get_rand() {
//     return rand() % 20;
// }

int main() {
    msc::bench_god     benchgod;
    vector<msc::robot> robots;

    for (int i = 0; i < ROBOT_N; ++i)
        robots.push_back(msc::robot(i));

    msc::io io(robots, benchgod);

    while (1) {
        io.receive();
        for (int i = 0; i < 1; ++i) {
            if (robots[i].is_waiting()) {
                msc::point pos = robots[i].pos();
                msc::task t = benchgod.get_task(pos);
                robots[i].start_task(t);
            }
            else
                robots[i].continue_task();
        }
        io.send();
    }

    return 0;
}

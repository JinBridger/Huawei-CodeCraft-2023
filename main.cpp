// #include "algo.hpp"
#define DEBUG
#include "data_types.hpp"
#include "io.hpp"

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

int get_rand() {
    return rand() % 20;
}

int main() {
    msc::bench_god     benchgod;
    vector<msc::robot> robots;

    for (int i = 0; i < ROBOT_N; ++i)
        robots.push_back(msc::robot(i));

    msc::io io(robots, benchgod);

    while (1) {
        io.receive();
        for (int i = 0; i < ROBOT_N; ++i) {
            if (robots[i].is_waiting()) {
                int buy_num  = get_rand();
                int sell_num = get_rand();
                robots[i].start_task(msc::task{ benchgod.get_workbench(buy_num).pos(), benchgod.get_workbench(sell_num).pos() });
            }
            else
                robots[i].continue_task();
        }
        io.send();
    }

    return 0;
}

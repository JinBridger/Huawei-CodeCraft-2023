// #include "algo.hpp"
#include "io.hpp"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
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
        robots.push_back(msc::robot(benchgod, i));

    msc::io io(robots, benchgod);

    while (1) {
        io.receive();
        for (int i = 0; i < ROBOT_N; ++i) {
            if (robots[i].is_waiting()) {
                int    buy_num  = get_rand();
                int    sell_num = get_rand();
                double buy_x    = benchgod.get_workbench(buy_num).x;
                double buy_y    = benchgod.get_workbench(buy_num).y;
                double sell_x   = benchgod.get_workbench(sell_num).x;
                double sell_y   = benchgod.get_workbench(sell_num).y;
                robots[i].begin_action(buy_x, buy_y, sell_x, sell_y);
            }
            else
                robots[i].continue_action();
        }
        io.send();
    }

    return 0;
}

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

int sequ = 0;
int get_sequ() {
    if (sequ == 30)
        sequ = 0;
    return sequ++;
}

int get_rand() {
    return rand() % 30;
}

int main() {
    srand(time(0));

    msc::bench_god     benchgod;
    vector<msc::robot> robots;

    for (int i = 0; i < ROBOT_N; ++i)
        robots.push_back(msc::robot(benchgod, i));

    msc::io io(robots, benchgod);
    int     b_num[4];

    while (1) {
        io.receive();
        for (int i = 0; i < ROBOT_N; ++i) {
            if (robots[i].is_idle()) {
                b_num[i]    = get_rand();
                double tg_x = benchgod.get_workbench(b_num[i]).x;
                double tg_y = benchgod.get_workbench(b_num[i]).y;
                robots[i].start_task(0, tg_x, tg_y);
            }
            else
                robots[i].continue_task();
        }
        io.send();
    }

    return 0;
}

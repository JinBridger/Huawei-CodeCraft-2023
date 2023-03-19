#include "algo.hpp"
#include "io.hpp"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <stdlib.h>

using namespace std;

void algo();

int FINISHED = 0;

void simple_goto(msc::robot& bot, double x, double y) {
    if (bot.angle_v == 0 && bot.linear_v == 0 && !FINISHED) {  // STOP
        double target_angle = atan((y - bot.y) / (x - bot.x));
        if (y - bot.y > 0 && x - bot.x < 0) {
            target_angle += PI;
        }
        if (y - bot.y < 0 && x - bot.x < 0) {
            target_angle -= PI;
        }
        double delta_angle = target_angle - bot.direction;
        if (delta_angle > 0) {
            bot.rotate(1);
        }
        else {
            bot.rotate(-1);
        }
        return;
    }
    if (bot.linear_v == 0 && !FINISHED) {  // ROTATING
        double target_angle = atan((y - bot.y) / (x - bot.x));
        if (y - bot.y > 0 && x - bot.x < 0) {
            target_angle += PI;
        }
        if (y - bot.y < 0 && x - bot.x < 0) {
            target_angle -= PI;
        }
        double delta_angle = target_angle - bot.direction;
        if (abs(delta_angle) < 0.05) {
            bot.rotate(0);
            FINISHED = 1;
        }
        return;
    }
    if (bot.angle_v == 0 && FINISHED) {
        bot.forward(5);
    }
    if (sqrt((bot.x - x) * (bot.x - x) + (bot.y - y) * (bot.y - y)) < 0.4) {
        std::cerr << "SUCCESS!";
        exit(1);
    }
    // RUNNING
}

bool readUntilOK() {
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        // do something
    }
    return false;
}

// void test_robot() {
//     msc::robot r;
//     r.forward(1);
//     r.rotate(2);
//     r.buy();
//     r.sell();
//     r.destroy();
// }

int main() {
    // freopen("C:/Users/26354/Documents/HuaweiCodeCraft/WindowsRelease/maps/1.txt", "r", stdin);
    array<msc::robot, ROBOT_N> robots;
    msc::io                    io(robots);

    robots[0].id = 0;
    robots[1].id = 1;
    robots[2].id = 2;
    robots[3].id = 3;

    msc::robot_controller debug_controller(io, robots[0]);

    while (1) {
        io.receive();
        if (debug_controller.isIdle())
            debug_controller.start_task(0, 1);
        else
            debug_controller.continue_task();
        io.send();
    }

    // auto start = std::chrono::system_clock::now();
    // // put what you want to test time here
    // // io.send();
    // auto end      = std::chrono::system_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // std::cout << "It takes " << duration.count() << " us";

    // readUntilOK();
    // puts("OK");
    // fflush(stdout);
    // int frameID;
    // while (scanf("%d", &frameID) != EOF) {
    //     readUntilOK();
    //     printf("%d\n", frameID);
    //     int    lineSpeed  = 6;
    //     double angleSpeed = 1.5;
    //     for (int robotId = 0; robotId < 4; robotId++) {
    //         printf("forward %d %d\n", robotId, lineSpeed);
    //         printf("rotate %d %f\n", robotId, angleSpeed);
    //     }
    //     printf("OK\n", frameID);
    //     fflush(stdout);
    // }
    return 0;
}

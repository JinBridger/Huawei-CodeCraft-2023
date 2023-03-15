#include "io.hpp"

#include <chrono>
#include <cstdio>
#include <iostream>

using namespace std;

void algo();

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

void test_robot() {
    msc::robot r;
    r.forward(1);
    r.rotate(2);
    r.buy();
    r.sell();
    r.destroy();
}

int main() {
    // freopen("C:/Users/26354/Documents/HuaweiCodeCraft/WindowsRelease/maps/1.txt", "r", stdin);
    array<msc::robot, ROBOT_N> robots;
    msc::io                    io(robots);

    robots[0].id = 0;
    robots[1].id = 1;
    robots[2].id = 2;
    robots[3].id = 3;

    robots[0].rotate(456.789);
    robots[1].buy();
    robots[1].destroy();
    robots[2].destroy();
    robots[3].forward(123.456);

    auto start = std::chrono::system_clock::now();
    // put what you want to test time here
    // io.send();
    auto end      = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "It takes " << duration.count() << " us";

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

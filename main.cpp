#include "io.hpp"
#include "robot.hpp"
#include "workbench.hpp"

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
    r.update(0, 1, 2, 0.1, 0.2, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6);
    r.forward(1);
    r.rotate(2);
    r.buy();
    r.sell();
    r.destroy();
    std::cout << r.get_state();
}

int main() {
    test_robot();
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

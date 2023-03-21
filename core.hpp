// predefined constant
#pragma once
#define MAP_SIZE 100
#define ROBOT_N 4
#define PI 3.1415926

#define ROBOT_RADIUS 0.45
#define ROBOT_RADIUS_ON_TAKING 0.53
#define ROBOT_DENSITY 20

namespace msc {
    constexpr double price_on_buy(int type) {
        switch (type) {
        case 1:
            return 3'000;
        case 2:
            return 4'400;
        case 3:
            return 5'800;
        case 4:
            return 15'400;
        case 5:
            return 17'200;
        case 6:
            return 19'200;
        case 7:
            return 76'000;
        }
        // unreachable
        return -1;
    }

    constexpr double price_on_sell(int type) {
        switch (type) {
        case 1:
            return 6'000;
        case 2:
            return 7'600;
        case 3:
            return 9'200;
        case 4:
            return 22'500;
        case 5:
            return 25'000;
        case 6:
            return 27'500;
        case 7:
            return 105'000;
        }
        // unreachable
        return -1;
    }

    inline bool can_product(int type) {
        return 1 <= type && type <= 7;
    }

    // NOTE: This function is for the workbench, so different primitives are used to distinguish it from the robot.
    constexpr bool can_purchase(int type, int material) {
        switch (type) {
        case 4:
            return material == 1 || material == 2;
        case 5:
            return material == 1 || material == 3;
        case 6:
            return material == 2 || material == 3;
        case 7:
            return material == 4 || material == 5 || material == 6;
        case 8:
            return material == 7;
        case 9:
            return can_product(type);
        }
        return false;
    }

    constexpr bool can_consume(int type) {
        return 4 <= type && type <= 7;
    }

}  // namespace msc
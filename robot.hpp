#pragma once

#include <cmath>
#include <string>

namespace msc {
    struct robot {
        // v - in unit m/s
        bool forward(double v) {
            state += "forward " + std::to_string(id) + " " + std::to_string(v) + "\n";
            return true;
        }

        // w - in unit rad/s
        bool rotate(double w) {
            state += "rotate " + std::to_string(id) + " " + std::to_string(w) + "\n";
            return true;
        }

        bool buy() {
            state += "buy " + std::to_string(id) + "\n";
            return true;
        }
        bool sell() {
            state += "sell " + std::to_string(id) + "\n";
            return true;
        }
        bool destroy() {
            state += "destroy " + std::to_string(id) + "\n";
            return true;
        }

        int id;

        int workbench_type;
        int object_type;

        double time_coef;  // time coefficient
        double coll_coef;  // collision coefficient

        double angle_v;    // angular velocity
        double linear_vx;  // x linear velocity
        double linear_vy;  // y linear velocity

        double direction;
        double x;
        double y;

        std::string state;
    };
}  // namespace msc
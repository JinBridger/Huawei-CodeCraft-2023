#pragma once

#include "core.hpp"

#include <cmath>
#include <string>
#include <utility>

namespace msc {
    struct robot {
        double area() const {
            return PI * pow(item ? ROBOT_RADIUS_ON_TAKING : ROBOT_RADIUS, 2);
        }

        double mass() const {
            return ROBOT_DENSITY * area();
        }

        // v - in unit m/s
        bool forward(double v) {
            _state += "forward " + std::to_string(id) + " " + std::to_string(v) + "\n";
            return true;
        }

        // w - in unit rad/s
        bool rotate(double w) {
            _state += "rotate " + std::to_string(id) + " " + std::to_string(w) + "\n";
            return true;
        }

        bool buy() {
            _state += "buy " + std::to_string(id) + "\n";
            return true;
        }
        bool sell() {
            _state += "sell " + std::to_string(id) + "\n";
            return true;
        }
        bool destroy() {
            _state += "destroy " + std::to_string(id) + "\n";
            return true;
        }

        std::string get_state() {
            return std::exchange(_state, "");
        }

        double sell_price() const {
            return origin_sell_price(item) * time_coef * coll_coef;
        }

        int id;

        int workbench;
        int item;

        double time_coef;  // time coefficient
        double coll_coef;  // collision coefficient

        double angle_v;   // angular velocity
        double linear_v;  // linear velocity

        double direction;
        double x;
        double y;

    private:
        std::string _state;
    };
}  // namespace msc
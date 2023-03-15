#pragma once

#include <bitset>

namespace msc {
    struct workbench {
        workbench(double x, double y, int type)
            : x(x), y(y), type(type), left_time(0), material_state(), product_state(), id(0) {}

        int id;
        int type;

        double x;
        double y;

        int            left_time;  // in unit frame
        std::bitset<6> material_state;
        bool           product_state;
    };
}  // namespace msc
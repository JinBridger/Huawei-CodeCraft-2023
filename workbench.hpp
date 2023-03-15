#pragma once

#include <bitset>

namespace msc {
    struct workbench {
        int id;
        int type;

        double x;
        double y;

        int            left_time;  // in unit frame
        std::bitset<6> material_state;
        bool           product_state;
    };
}  // namespace msc
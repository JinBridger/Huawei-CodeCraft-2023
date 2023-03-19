#pragma once

#include "workbench.hpp"

#include <iostream>
#include <vector>

namespace msc {
    /**
     * TODO: Analyze the map
     */
    class bench_god {
    public:
        template <class... Ts>
        void add_bench(Ts&&... args) {
            _workbenches.emplace_back(std::forward<Ts>(args)...);
        }

        size_t size() noexcept {
            return _workbenches.size();
        }

        void update() {
            for (auto& ben : _workbenches) {
                std::cin >> ben.type;
                std::cin >> ben.x;
                std::cin >> ben.y;
                std::cin >> ben.left_time;
                std::cin >> ben.material_state;
                std::cin >> ben.product_state;
            }
        }

        // TO REMOVE
        workbench& get_workbench(int workbench_num) {
            return _workbenches[workbench_num];
        }

    private:
        std::vector<workbench> _workbenches;
    };
}  // namespace msc
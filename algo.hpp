#pragma once

#include "data_types.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace msc {
    class bench_god {
    public:
        bench_god() {
            _workbenches.reserve(50);
        }

        void add_bench(const point& pos, int type) {
            _workbenches.emplace_back(pos, type);
        }

        void analyze() {
            for (int i = 0; i < _workbenches.size(); ++i) {
                if (can_product(_workbenches[i]._bench.type())) {
                    for (int j = 0; j < _workbenches.size(); ++j) {
                        if (i != j && can_purchase(_workbenches[j]._bench.type(), _workbenches[i]._bench.type()))
                            _workbenches[i]._tasks.emplace_back(std::addressof(_workbenches[j]._bench));
                    }
                }
                if (can_consume(_workbenches[i]._bench.type())) {
                    for (int j = 0; j < _workbenches.size(); ++j) {
                        if (i != j && can_product(_workbenches[j]._bench.type()))
                            _workbenches[i]._next_tasks.emplace_back(std::addressof(_workbenches[j]._bench));
                    }
                }
            }
        }

        size_t size() noexcept {
            return _workbenches.size();
        }

        friend std::istream& operator>>(std::istream& is, bench_god& god) {
            for (auto& h : god._workbenches) {
                is >> h._bench;
            }
            return is;
        }

        task get_task(const point& pos) noexcept {
            double     max_eff = -1;
            workbench* start{};
            edge*      max_task{};
            for (auto& h : _workbenches) {
                for (auto& t : h._tasks) {
                    if (!t._used && h._bench.produced()) {
                        if (double eff = task_profit(h._bench, *t._target)
                                         / (h._bench.pos().distance(t._target->pos()) + pos.distance(h._bench.pos()));
                            eff > max_eff) {
                            max_eff  = eff;
                            start    = &h._bench;
                            max_task = &t;
                        }
                    }
                }
            }


            // auto& h   = _workbenches[_indeces.find(pos)->second];
            // auto  res = std::find_if(h._tasks.begin(), h._tasks.end(),
            //                          [](const edge& e) { return !e._used && !e._target->is_producing(); });
            // assert(res != h._tasks.end());
            if (max_task) {
                max_task->_used = true;
                return { start->pos(), max_task->_target->pos(), [&] { max_task->_used = false; } };
            }
            return { point{}, point{}, nullptr };
        }

        // task get_next_task(const point& pos) noexcept {
        //     auto& h   = _workbenches[_indeces.find(pos)->second];
        //     auto  res = std::find_if(h._next_tasks.begin(), h._next_tasks.end(),
        //                              [](const edge& e) { return !e._used && !e._target->is_producing(); });
        //     assert(res != h._tasks.end());

        //     return { h._bench.pos(), res->_target->pos(), [&] { res->_used = false; } };
        // }

#ifdef DEBUG

        workbench& get_workbench(int index) {
            return _workbenches[index]._bench;
        }
#endif

    private:
        struct point_hash {
            static_assert(std::is_standard_layout_v<point>, "class point should be standard layout.");
            using result_type   = size_t;
            using argument_type = point;
            [[nodiscard]] size_t operator()(const point& p) const {
                constexpr uint64_t seed = 0xdeadbeef;
                constexpr uint64_t m    = 0xc6a4a7935bd1e995;
                constexpr uint64_t r    = 47;

                uint64_t h = seed ^ (sizeof(point) * m);

                const uint64_t* data        = reinterpret_cast<const uint64_t*>(&p);
                const uint64_t  num_doubles = sizeof(point) / sizeof(double);
                for (size_t i = 0; i < num_doubles; i++) {
                    uint64_t k = data[i];
                    k *= m;
                    k ^= k >> r;
                    k *= m;

                    h ^= k;
                    h *= m;
                }

                const size_t num_remaining_bytes = sizeof(point) % sizeof(double);
                if (num_remaining_bytes != 0) {
                    uint64_t k = 0;
                    memcpy(&k, reinterpret_cast<const uint8_t*>(&p) + sizeof(msc::point) - num_remaining_bytes,
                           num_remaining_bytes);
                    k *= m;
                    k ^= k >> r;
                    k *= m;

                    h ^= k;
                    h *= m;
                }

                h ^= h >> r;
                h *= m;
                h ^= h >> r;

                return static_cast<size_t>(h);
            }
        };
        struct edge {
            edge(workbench* target) : _target(target), _profit(0.0), _used(false) {}

            workbench* _target;
            bool       _used;
            double     _profit;
        };

        struct header {

            header(const point& pos, int type) : _bench(pos, type), _tasks(), _next_tasks() {}

            workbench         _bench;
            std::vector<edge> _tasks;
            std::vector<edge> _next_tasks;
        };

        std::vector<header>                           _workbenches;
        std::vector<robot>                            _robots;
    };
}  // namespace msc
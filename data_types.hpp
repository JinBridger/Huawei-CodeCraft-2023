#pragma once

#include "core.hpp"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace msc {
    struct point {
        point(double x = 0.0, double y = 0.0) noexcept : x(x), y(y) {}
        point(const point& p) noexcept : x(p.x), y(p.y) {}
        point(point&& p) noexcept : x(p.x), y(p.y) {}

        double distance(const point& p) const noexcept {
            return sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
        }

        double arctan(const point& p) const noexcept {
            return atan((p.y - y) / (p.x - x));
        }

        friend std::istream& operator>>(std::istream& is, point& p) {
            return is >> p.x >> p.y;
        }
        friend std::ostream& operator<<(std::ostream& os, const point& p) {
            return os << p.x << p.y;
        }
        point& operator=(const point& p) noexcept {
            if (std::addressof(p) != this)
                x = p.x, y = p.y;
            return *this;
        }
        point& operator=(point&& p) noexcept {
            return *this = p;
        }

        [[nodiscard]] friend bool operator==(const point& lhs, const point& rhs) noexcept {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }

        [[nodiscard]] friend bool operator!=(const point& lhs, const point& rhs) noexcept {
            return !(lhs == rhs);
        }

        double x, y;
    };

    struct task {
        point                 sell, buy;
        std::function<void()> clear_up;
    };

    class workbench {
    public:
        workbench(const point& p, int type)
            : _pos(p), _type(type), _left_time(0), _material_state(), _product_state() {}

        bool operator[](size_t idx) {
            return _material_state[idx];
        }

        friend std::istream& operator>>(std::istream& is, workbench& ben) {
            return is >> ben._type >> ben._pos >> ben._left_time >> ben._material_state >> ben._product_state;
        }

        int type() const noexcept {
            return _type;
        }

        bool is_producing() const noexcept {
            return _product_state;
        }

        const point& pos() const noexcept {
            return _pos;
        }

    private:
        int _type;

        point _pos;

        int            _left_time;  // in unit frame
        std::bitset<8> _material_state;
        bool           _product_state;
    };

    class bench_god {
    public:
        bench_god() {
            _workbenches.reserve(50);
        }
        template <class... Ts>
        void add_bench(Ts&&... args) {
            _workbenches.emplace_back(std::forward<Ts>(args)...);
            _indeces.emplace(_workbenches.back()._bench.pos(), _workbenches.size() - 1);
        }

        void analyze() {
            for (auto& h : _workbenches) {
                if (can_product(h._bench.type())) {
                    for (auto& other : _workbenches) {
                        if (can_purchase(other._bench.type(), h._bench.type()))
                            h._tasks.emplace_back(std::addressof(other._bench), task_profit());
                    }
                    std::sort(h._tasks.begin(), h._tasks.end(),
                              [](const edge& l, const edge& r) { return l._profit < r._profit; });
                }
                if (can_consume(h._bench.type())) {
                    for (auto& other : _workbenches) {
                        if (can_product(other._bench.type()))
                            h._next_tasks.emplace_back(std::addressof(other._bench), next_task_profit());
                    }
                    std::sort(h._tasks.begin(), h._tasks.end(),
                              [](const edge& l, const edge& r) { return l._profit < r._profit; });
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
            auto& h   = _workbenches[_indeces.find(pos)->second];
            auto  res = std::find_if(h._tasks.begin(), h._tasks.end(), [](const edge& e) { return !e._used; });
            assert(res != h._tasks.end());

            return { h._bench.pos(), res->_target->pos(), [&] { res->_used = false; } };
        }

        task get_next_task(const point& pos) noexcept {
            auto& h  = _workbenches[_indeces.find(pos)->second];
            auto res = std::find_if(h._next_tasks.begin(), h._next_tasks.end(), [](const edge& e) { return !e._used; });
            assert(res != h._tasks.end());

            return { h._bench.pos(), res->_target->pos(), [&] { res->_used = false; } };
        }

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
            edge(workbench* target, double weight) : _target(target), _profit(weight), _used(false) {}

            workbench* _target;
            bool       _used;
            double     _profit;
        };

        struct header {
            template <class... Ts>
            header(Ts&&... args) : _bench(std::forward<Ts>(args)...), _tasks(), _next_tasks() {}

            workbench         _bench;
            std::vector<edge> _tasks;
            std::vector<edge> _next_tasks;
        };

        std::vector<header>                           _workbenches;
        std::unordered_map<point, size_t, point_hash> _indeces;
    };

    class robot {
    public:
        robot(int id) : _action_status(Idle), _task_status(Waiting), _id(id), _task() {}

        bool is_waiting() const noexcept {
            return _task_status == Waiting;
        }

        bool is_busy() const noexcept {
            return _action_status > 0;
        }

        void start_task(const task& t) {
            // Called by scheduler to start a task
            _task = t;

            std::cerr << "[LOG] Robot " << _id << " Buy: [" << _task.buy.x << ", " << _task.buy.y << "]\tSell: ["
                      << _task.sell.x << ", " << _task.sell.y << "]" << std::endl;
            _task_status = Buying;
            start_action<Buy>(_task.buy);
        }

        void continue_task() {
            // Call by scheduler to continueu the unfinished task
            if (is_busy())
                continue_action();
            else {
                if (_task_status == Buying) {
                    _task_status = Selling;
                    start_action<Sell>(_task.sell);
                }
                else if (_task_status == Selling) {
                    _task_status = Waiting;
                    if (_task.clear_up)
                        _task.clear_up();
                }
            }
        }

        friend std::istream& operator>>(std::istream& is, robot& r) {
            is >> r._workbench >> r._item >> r._time_coef >> r._coll_coef >> r._angle_v;
            double vx, vy;
            is >> vx >> vy;
            r._linear_v = sqrt(vx * vx + vy * vy);
            return is >> r._direction >> r._pos;
        }

        void update_pos(const point& p) noexcept {
            _pos = p;
        }

        std::string get_state() {
            return std::exchange(_state, "");
        }

    private:
        enum ActionStatus { Idle, Buy, Sell };
        enum TaskStatus { Waiting, Buying, Selling };

        template <ActionStatus status>
        void start_action(const point& target) {
            // Set action
            _target = target;
            if constexpr (status == Sell)
                _action_status = Sell;
            else
                _action_status = Buy;

            // std::cerr << "[LOG] Robot " << _id << " now going to [" << _target_x << ", " << _target_y << "]"
            //   << std::endl;

            continue_action();
        }

        void continue_action() {
            // Continue the unfinished action

            // Calculate the target angle of workbench
            double target_angle = _pos.arctan(_target);

            // copysign()
            if (_target.x < _pos.x)
                target_angle += copysign(PI, _target.y - _pos.y);

            // Calculate the angle between robot and workbench
            double delta_angle = target_angle - _direction;
            if (delta_angle < -PI)
                delta_angle += 2 * PI;

            if (delta_angle > PI)
                delta_angle -= 2 * PI;

            // Calculate the distance between robot and workbench
            double delta_dis = _pos.distance(_target);

            if (is_busy() && delta_dis < 0.4) {
                if (_action_status == Buy) {
                    buy();
                }
                if (_action_status == Sell) {
                    sell();
                }
                forward(0);
                rotate(0);
                // std::cerr << "[LOG] ROBOT X: " << _x << "\tY: " << _y << std::endl;
                // std::cerr << "[LOG] TARGT X: " << _target_x << "\tY: " << _target_y << std::endl;
                // std::cerr << "[LOG] Robot " << _id << " dis is " << delta_dis << std::endl;
                // std::cerr << "[LOG] Robot " << _id << " set to idle" << std::endl;

                _action_status = Idle;
            }
            if (is_busy()) {
                // Correction the direction
                if (fabs(delta_angle) > get_angle_threshold(delta_dis, fabs(delta_angle))) {
                    if (delta_angle > 0)
                        rotate(PI);
                    else
                        rotate(-PI);
                }
                else {
                    rotate(0);
                }

                // Set Speed
                forward(get_speed_threshold(delta_dis, fabs(delta_angle)));
            }
        }

        double get_angle_threshold(double dt_dis, double dt_angle) {
            if (dt_dis > 20)
                return 0.08;
            return 0.04;
        }

        double get_speed_threshold(double dt_dis, double dt_angle) {
            if (dt_dis > 2) {
                if (dt_angle < 0.1)
                    return 6;
                return 0;
            }
            if (dt_angle < 0.04)
                return 5;
            return 2;
        }

        // Low level API
        double area() const {
            return PI * pow(_item ? ROBOT_RADIUS_ON_TAKING : ROBOT_RADIUS, 2);
        }

        double mass() const {
            return ROBOT_DENSITY * area();
        }

        // v - in unit m/s
        bool forward(double v) {
            _state += "forward " + std::to_string(_id) + " " + std::to_string(v) + "\n";
            return true;
        }

        // w - in unit rad/s
        bool rotate(double w) {
            _state += "rotate " + std::to_string(_id) + " " + std::to_string(w) + "\n";
            return true;
        }

        bool buy() {
            // _state += "buy " + std::to_string(_id) + "\n";
            return true;
        }
        bool sell() {
            // _state += "sell " + std::to_string(_id) + "\n";
            return true;
        }
        bool destroy() {
            _state += "destroy " + std::to_string(_id) + "\n";
            return true;
        }

        double sell_price() const {
            return origin_sell_price(_item) * _time_coef * _coll_coef;
        }

        int _id;

        int _workbench;
        int _item;

        double _time_coef;  // time coefficient
        double _coll_coef;  // collision coefficient

        double _angle_v;   // angular velocity
        double _linear_v;  // linear velocity

        double _direction;
        point  _pos;

        point _target;

        task _task;

        ActionStatus _action_status;
        TaskStatus   _task_status;

        std::string _state;
    };
}  // namespace msc

#pragma once

#include "bench_god.hpp"
#include "core.hpp"

#include <cmath>
#include <string>
#include <utility>

namespace msc {
    class robot {
    public:
        robot(bench_god& b, int id) : _bench_god(b), _target_workbench(nullptr), _task_status(Idle), _id(id) {}

        bool is_idle() const {
            return _task_status == Idle ? true : false;
        }

        void start_task(bool is_sell, int target_workbench_num) {
            // Called by scheduler when robot is idle
            // Start a new task

            // Set task
            _target_workbench = &_bench_god.get_workbench(target_workbench_num);
            _task_status      = Busy;
            _task_type        = is_sell ? Sell : Buy;

            std::cerr << "[LOG] Now going to [" << _target_workbench->x << ", " << _target_workbench->y << "]"
                      << std::endl;

            continue_task();
        }

        void continue_task() {
            // Called by scheduler when robot is busy
            // Continue the unfinished task

            // Calculate the target angle of workbench
            double dy           = _target_workbench->y - _y;
            double dx           = _target_workbench->x - _x;
            double target_angle = atan(dy / dx);
            if (dx < 0) {
                if (dy > 0)
                    target_angle += PI;
                if (dy < 0)
                    target_angle -= PI;
            }

            // Calculate the angle between robot and workbench
            double delta_angle = target_angle - _direction;
            if (delta_angle < -PI)
                delta_angle += 2 * PI;

            if (delta_angle > PI)
                delta_angle -= 2 * PI;

            // Calculate the distance between robot and workbench
            double delta_dis = sqrt(dy * dy + dx * dx);

            if (_task_status == Busy && delta_dis < 0.4) {
                if (_task_type == Buy) {
                    buy();
                }
                if (_task_type == Sell) {
                    sell();
                }
                forward(0);
                rotate(0);
                _task_status = Idle;
            }
            if (_task_status == Busy) {
                // Correction the direction
                if (abs(delta_angle) > get_angle_threshold(delta_dis, abs(delta_angle))) {
                    if (delta_angle > 0)
                        rotate(PI);
                    else
                        rotate(-PI);
                }
                else {
                    rotate(0);
                }

                // Set Speed
                forward(get_speed_threshold(delta_dis, abs(delta_angle)));
            }
        }

        void update() {
            std::cin >> _workbench;
            std::cin >> _item;

            std::cin >> _time_coef;
            std::cin >> _coll_coef;

            std::cin >> _angle_v;
            double vx, vy;
            std::cin >> vx >> vy;
            _linear_v = sqrt(vx * vx + vy * vy);

            std::cin >> _direction;
            std::cin >> _x;
            std::cin >> _y;
        }

        void update_x_and_y(double x, double y) {
            _x = x;
            _y = y;
        }

        std::string get_state() {
            return std::exchange(_state, "");
        }

    private:
        // Low level APIs

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

        enum TaskStatus { Idle, Busy };
        enum TaskType { Buy, Sell };

        bench_god& _bench_god;
        workbench* _target_workbench;

        TaskStatus _task_status;
        TaskType   _task_type;

        // private:
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
            _state += "buy " + std::to_string(_id) + "\n";
            return true;
        }
        bool sell() {
            _state += "sell " + std::to_string(_id) + "\n";
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
        double _x;
        double _y;

    private:
        std::string _state;
    };
}  // namespace msc
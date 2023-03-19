#include "io.hpp"
#include "robot.hpp"
#include "workbench.hpp"

#include <algorithm>
#include <cmath>

namespace msc {
    class robot_controller {
    public:
        robot_controller(io& i, robot& r) : _io(i), _robot(r), _target_workbench(nullptr), _task_status(Idle) {}

        bool is_idle() const {
            return _task_status == Idle ? true : false;
        }

        void start_task(bool is_sell, int target_workbench_num) {
            // Called by scheduler when robot is idle
            // Start a new task

            // Set task
            _target_workbench = &_io.get_workbench(target_workbench_num);
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
            double dy           = _target_workbench->y - _robot.y;
            double dx           = _target_workbench->x - _robot.x;
            double target_angle = atan(dy / dx);
            if (dx < 0) {
                if (dy > 0)
                    target_angle += PI;
                if (dy < 0)
                    target_angle -= PI;
            }

            // Calculate the angle between robot and workbench
            double delta_angle = target_angle - _robot.direction;
            if (delta_angle < -PI)
                delta_angle += 2 * PI;

            if (delta_angle > PI)
                delta_angle -= 2 * PI;

            // Calculate the distance between robot and workbench
            double delta_dis = sqrt(dy * dy + dx * dx);

            if (_task_status == Busy && delta_dis < 0.4) {
                if (_task_type == Buy) {
                    _robot.buy();
                }
                if (_task_type == Sell) {
                    _robot.sell();
                }
                _robot.forward(0);
                _robot.rotate(0);
                _task_status = Idle;
            }
            if (_task_status == Busy) {
                // Correction the direction
                if (abs(delta_angle) > get_angle_threshold(delta_dis, abs(delta_angle))) {
                    if (delta_angle > 0)
                        _robot.rotate(PI);
                    else
                        _robot.rotate(-PI);
                }
                else {
                    _robot.rotate(0);
                }

                // Set Speed
                _robot.forward(get_speed_threshold(delta_dis, abs(delta_angle)));
            }

            // std::cerr << "[LOG] SPD LIMIT: " << get_speed_threshold(abs(delta_angle))
            //   << "\t ANG LIMIT: " << get_angle_threshold(delta_dis) << std::endl;
        }

    private:
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

        io&        _io;
        robot&     _robot;
        workbench* _target_workbench;

        TaskStatus _task_status;
        TaskType   _task_type;
    };
}  // namespace msc
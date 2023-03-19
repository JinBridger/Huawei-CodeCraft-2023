#include "io.hpp"
#include "robot.hpp"
#include "workbench.hpp"

#include <cmath>

namespace msc {
    class robot_controller {
    public:
        robot_controller(io& i, robot& r) : _io(i), _robot(r), _target_workbench(nullptr), _task_status(Idle) {}

        bool isIdle() const {
            return _task_status == Idle ? true : false;
        }

        void start_task(bool is_sell, int target_workbench_num) {
            // Called by scheduler when robot is idle
            // Start a new task

            if (_robot.angle_v != 0 && _robot.linear_v != 0) {
                // Check if robot is static
                // If not, stop it
                _robot.forward(0);
                _robot.rotate(0);
                return;
            }

            // Set task
            _target_workbench = &_io.get_workbench(target_workbench_num);
            _task_status      = Busy;
            _task_type        = is_sell ? Sell : Buy;
            _control_level    = CoarseControl;
            _current_status   = AdjustAngle;

            // Start the task
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

            // Calculate the distance between robot and workbench
            double delta_dis = sqrt(dy * dy + dx * dx);

            if (_control_level == CoarseControl) {
                if (_current_status == AdjustAngle) {
                    if (abs(delta_angle) > 0.1) {
                        // Continue rotate
                        if (delta_angle > 0)
                            _robot.rotate(PI);
                        else
                            _robot.rotate(-PI);
                    }
                    else {
                        // If rotating, stop rotate
                        if (_robot.angle_v != 0)
                            _robot.rotate(0);
                        else {
                            // Start forward
                            _current_status = MoveForward;
                            _robot.forward(6.0);
                        }
                    }
                }
                if (_current_status == MoveForward) {
                    if (delta_dis <= 5) {
                        // Change to fine control and stop
                        _control_level  = FineControl;
                        _current_status = WaitForStop;
                        _robot.forward(0);
                    }
                }
            }
            if (_control_level == FineControl) {
                if (_current_status == WaitForStop) {
                    if (_robot.linear_v != 0)
                        _robot.forward(0);
                    else {
                        _current_status = AdjustAngle;
                    }
                }
                if (_current_status == AdjustAngle) {
                    if (abs(delta_angle) > 0.04) {
                        // Continue rotate
                        if (delta_angle > 0)
                            _robot.rotate(PI);
                        else
                            _robot.rotate(-PI);
                    }
                    else {
                        // If rotating, stop rotate
                        if (_robot.angle_v != 0)
                            _robot.rotate(0);
                        else {
                            // Start forward
                            _current_status = MoveForward;
                            _robot.forward(6.0);
                        }
                    }
                }
                if (_current_status == MoveForward) {
                    if (delta_dis < 0.4) {
                        // Reach the workbench
                        if (_task_type == Buy) {
                            _robot.buy();
                        }
                        if (_task_type == Sell) {
                            _robot.sell();
                        }
                        _control_level = FinishTask;
                        _robot.forward(0);
                    }
                    else {
                        _robot.forward(6.0);
                    }
                }
            }
            if (_control_level == FinishTask) {
                if (_robot.linear_v == 0) {
                    // End task
                    _task_status      = Idle;
                    _target_workbench = nullptr;
                }
                else {
                    _robot.forward(0);
                }
            }
        }

    private:
        enum ControlLevel { CoarseControl, FineControl, FinishTask };
        enum TaskStatus { Idle, Busy };
        enum TaskType { Buy, Sell };
        enum CurrentStatus { AdjustAngle, MoveForward, WaitForStop };

        io&        _io;
        robot&     _robot;
        workbench* _target_workbench;

        ControlLevel  _control_level;
        TaskStatus    _task_status;
        TaskType      _task_type;
        CurrentStatus _current_status;
    };
}  // namespace msc
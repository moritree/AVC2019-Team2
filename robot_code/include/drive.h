//
// Created by Zuni Preece on 2019-05-22.
//

#ifndef ROBOT_CODE_DRIVE_H
#define ROBOT_CODE_DRIVE_H


class drive {
public:
    explicit  drive();

    void turn(int turn, int forward_speed);
    void forward(int speed);
    void stop();
    void lost();
    void look_up();
    void look_down();

private:
    int max_speed;
    int zero_speed;
    int motor_left;
    int motor_right;
    int motor_camera;
};


#endif //ROBOT_CODE_DRIVE_H

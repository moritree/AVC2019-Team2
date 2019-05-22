//
// Created by Zuni Preece on 2019-05-22.
//

#ifndef ROBOT_CODE_DRIVE_H
#define ROBOT_CODE_DRIVE_H


class drive {
public:
    explicit drive(bool db);
    void turn(int direction, bool stationary, double scale);
    void forward(int speed);
    void stop();
    void lost();
    void look_up(int time);
    void look_down(int time);
private:
    int zero_speed;
    int motor_left;
    int motor_right;
    int motor_camera;
    bool debug;
};


#endif //ROBOT_CODE_DRIVE_H

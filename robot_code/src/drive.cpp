#include "drive.h"

#include <stdio.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "E101.h"

drive::drive() {
    zero_speed = 48;
    max_speed = 17;

    motor_left = 5;     // port for left motor
    motor_right = 1;    // port for right motor
    motor_camera = -1;  // port for the camera motor
}

void drive::turn(int turn, int fwd_speed){
    stop();
    set_motor(motor_left, zero_speed + turn - fwd_speed);
    set_motor(motor_right, zero_speed + turn + fwd_speed);
    hardware_exchange();
}

void drive::forward (int speed) {
    set_motor(motor_left, zero_speed - speed);
    set_motor(motor_right, zero_speed + speed);
    hardware_exchange();
}

void drive::stop() {
    forward(0);
}

void drive::lost(){
    forward(-5);
}

void drive::look_up() {
    set_motor(motor_camera,10);
    hardware_exchange();
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
    set_motor(motor_camera,0);
    hardware_exchange();
}

void drive::look_down() {
    set_motor(motor_camera,-10);
    hardware_exchange();
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
    set_motor(motor_camera,0);
    hardware_exchange();
}

#include "drive.h"

#include <stdio.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "E101.h"

// max values = 18
drive::drive(bool db) {
    zero_speed = 48;
    motor_left = 5;     // set to the port for left motor
    motor_right = 1;    // set to the port for right motor
    motor_camera = -1;  // set to the port for the camera motor
    debug = db;
}

void drive::turn(int direction, bool stationary, double scale){
    if (stationary){
        stop();
        set_motor(motor_left, direction);
        set_motor(motor_right, direction);
        if (debug) {printf("stationary turn\n");}
    }

//    else{ /// this will need testing for how the motors work
//        set_motor(motor_left,(direction > 0 )? -direction : -(direction/scale));
//        set_motor(motor_right,(direction > 0 )? direction : (direction/scale));
//        if (debug) {printf("turn\n");}
//    }
//    hardware_exchange();
//    sleep1(5);
}

void drive::forward (int speed) {
    if (abs(speed) < 15){
        set_motor(motor_left,zero_speed-speed);
        set_motor(motor_right,zero_speed+speed);
        hardware_exchange();
    }

    else{
        printf("ERROR: Speed too high\n");
        if (debug) {printf("speed : %d \n",speed);}
    }

}

void drive::stop() {
    forward(0);
}

void drive::lost(){
    forward(-5);
}

void drive::look_up(int time_millis) {
    set_motor(motor_camera,10);
    hardware_exchange();
    std::this_thread::sleep_for (std::chrono::milliseconds(time_millis));
    set_motor(motor_camera,0);
    std::this_thread::sleep_for (std::chrono::milliseconds(time_millis));
}

void drive::look_down(int time_millis) {
    set_motor(motor_camera,-10);
    hardware_exchange();
    std::this_thread::sleep_for (std::chrono::milliseconds(time_millis));
    set_motor(motor_camera,0);
    hardware_exchange();
    std::this_thread::sleep_for (std::chrono::milliseconds(time_millis));
}

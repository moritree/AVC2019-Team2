//
// Created by Zuni Preece on 2019-05-23.
//

#include "../include/robot.h"
#include "../include/readCamera.h"
#include "../include/drive.h"

robot::robot(readCamera rc, class drive dr) {
    camera = rc;
    drive = dr;

    // TODO optimise values of Kp and Kd
    // Start with Kp, increase slowly from 0 until robot starts swinging
    // Then increase Kd until movement is smooth
    Kp = 0;
    Kd = 0;
}

void robot::quadrant1() {}

void robot::quadrant2() {
    bool run = true;
    int count = 0;

    // Initialise error and derivative (so frame 1 derivative is correct)
    double array[camera.CAM_WIDTH];
    camera.simplePixelsFromCamera(3, array, 120);
    camera.getDerivative(camera.getError(array));

    while (run) {
        followLine();

        count ++;
        if (count > 100) run = false;
    }
}

void robot::quadrant3() {}

void robot::quadrant4() {}

void robot::followLine() {
    // Get current error and derivative
    double array[camera.CAM_WIDTH];
    camera.simplePixelsFromCamera(3, array, 120);
    double error = camera.getError(array);
    double derivative = camera.getDerivative(error);

    // Continuously move forwards, and turn according to proportional formula
    drive.turn(int(Kp * error + Kd * derivative), 5);
}
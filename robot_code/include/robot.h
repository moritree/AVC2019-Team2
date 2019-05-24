//
// Created by Zuni Preece on 2019-05-23.
//

#ifndef ROBOT_CODE_ROBOT_H
#define ROBOT_CODE_ROBOT_H

#include "drive.h"
#include "readCamera.h"

class robot {
public:
    explicit robot(readCamera rc, drive dr);

    /**
     * Quadrant 1
     * Open the gate by exchange with the server over WiFi
     */
    void quadrant1();

    /**
     * Quadrant 2
     * Follow smooth wiggly line
     */
    void quadrant2();

    /**
     * Quadrant 3
     * Make sharp turns. Logic of direction selection.
     */
    void quadrant3();

    /**
     * Quadrant 4
     * Come close to the colored cylinders and drive to the finish line
     */
    void quadrant4();

private:
    readCamera cam;
    drive dr;
    float Kp, Kd;  // Coefficients

    /**
     * Using proportional formula, follow the black
     * line, keeping the error to a minimum
     */
    void followLine();
};


#endif //ROBOT_CODE_ROBOT_H

//
// Created by Zuni Preece on 2019-05-21.
//

#ifndef TRY2_READCAMERA_H
#define TRY2_READCAMERA_H

#include <stdio.h>
#include <string>
#include <fstream>
#include <cmath>
#include <sys/time.h>
#include <cassert>

using namespace std;

/**
 * This class interperets data from the Pi's camera and gives
 * information we can use to inform our movement decisions.
 */
class readCamera{
public:
    int CAM_WIDTH, CAM_HEIGHT;
    explicit readCamera();

    /**
     * Gives the error of a specific colour channel.
     * @param channel R (0), G (1), B (2), or all (3)
     * @return error sum between -1 and 1
     */
    double getError(double* picture);

    /**
     * Fills array from the camera for the get_error function
     * Average of the chosen channel in each column, but
     *  restricted to a range between 0 and 1.
     * @param channel R (0), G (1), B (2), or all (3)
     */
    void pixelsFromCamera(int channel, double *array);
    double getDerivative(double errror);
private:
    double prevError;
    struct timeval thisTime, prevTime;
};

#endif //TRY2_READCAMERA_H

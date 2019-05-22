//
// Created by Zuni Preece on 2019-05-21.
//

#ifndef TRY2_READ_CAMERA_H
#define TRY2_READ_CAMERA_H

#include <stdio.h>
#include <string>
#include <fstream>
#include <cmath>
#include <sys/time.h>
#include <cassert>

using namespace std;

/**
 * This class interprets data from the Raspberry Pi's camera and gives
 * information we can use to inform our movement decisions.
 */
class readCamera{
public:
    int CAM_WIDTH, CAM_HEIGHT;
    explicit readCamera();

    /**
     * Gives the error e of a specific colour channel, where e=0
     * when the image is horizontally symmetrical.
     * @param picture array of CAM_WIDTH
     * @return e sum between -1 and 1
     */
    double getError(double* picture);

    /**
     * Writes from camera input into 1-dimensional array for finding the error values.
     * Each index contains, restricted to a range of 0 to 1, the average value of:
     *  - For R/G/B channels: The ratio of this channel to the total whiteness
     *  - For the whiteness channel: Unprocessed value
     * @param channel R (0), G (1), B (2), or all (3)
     * @param array Array to write into
     */
    void pixelsFromCamera(int channel, double *array);

    /**
     * Gets the time derivative of the error. Automatically keeps track of previous
     * error and time in private fields, so the only parameter is the error which has
     * just been calculated.
     * @param error Sum at this time
     * @return Time derivative of e, de/dt
     */
    double getDerivative(double error);
private:
    double prevError;
    struct timeval thisTime, prevTime;
};

#endif //TRY2_READ_CAMERA_H

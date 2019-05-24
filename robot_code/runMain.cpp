//
// Created by Zuni Preece on 2019-05-23.
//

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "E101/libtest/E101.h"
#include "include/drive.h"

using namespace std;

/**
 * This class interprets data from the Raspberry Pi's camera and gives
 * information we can use to inform our movement decisions.
 */
class readCamera{
public:
    int CAM_WIDTH, CAM_HEIGHT;
    explicit readCamera() {
        prevError = 0.0;
        gettimeofday(&prevTime, nullptr);
        CAM_WIDTH = 320;
        CAM_HEIGHT = 240;
        open_screen_stream();
    }

    /**
     * Calculates the error e, where e = 0 if the “center
     * of mass” of the array is in the center index.
     * @param array Array of CAM_WIDTH
     * @return e sum between -1 and 1
     */
    double getError(double* array) {
        // Generate error array
        double error_array [CAM_WIDTH];
        for(int i = 0; i < CAM_WIDTH; i ++) {
            // Restrict to range of -1 to 1
            error_array[i] = i - (CAM_WIDTH/2.0) + 0.5;
        }

        // Sum error array and picture array
        double sum = 0;
        for(int i = 0; i < CAM_WIDTH; i ++) {
            sum += array[i] * error_array[i];
        }
        return sum / 12800;
    }

    /**
     * Writes from camera input into 1-dimensional array for finding the error values.
     * Each index contains, restricted to a range of 0 to 1, the average value of:
     *  - For R/G/B channels: The ratio of this channel to the total whiteness
     *  - For the whiteness channel: Unprocessed value
     * @param channel R (0), G (1), B (2), or all (3)
     * @param array Array to write into
     */
    void pixelsFromCamera(int channel, double *array) {
        // Check that the channel is valid
        assert(channel >= 0 && channel <= 3);

        take_picture();

        for (int i = 0; i < CAM_WIDTH; i ++) {
            // Add together this channel's values for every
            // pixel in this column
            for (int j = 0; j < CAM_HEIGHT; j ++) {
                if (channel == 3) {
                    // Whiteness channel needs no change
                    array[i] += get_pixel(i, j, channel);
                } else {
                    // Colour ratio for the selected channel
                    array[i] += get_pixel(i, j, channel) / (float)get_pixel(i, j, 3);
                }
            }
            // Restrict to range of 0 to 1
            array[i] *= 1.0/(CAM_HEIGHT * 255);
        }
    }

    void simplePixelsFromCamera(int channel, double *array, int row) {
        // Check that the channel is valid
        assert(channel >= 0 && channel <= 3);

        take_picture();

        for (int i = 0; i < CAM_WIDTH; i ++) {
            // Add together this channel's values for every pixel in the middle column
            if (channel == 3) {
                // Whiteness channel needs no change
                array[i] += get_pixel(i, row, channel);
            } else {
                // Colour ratio for the selected channel
                array[i] += get_pixel(i, row, channel) / (float)get_pixel(i, row, 3);
            }

            // Restrict to range of 0 to 1
            array[i] *= 1.0/255.0;
        }
    }

    /**
     * Gets the time derivative of the error. Automatically keeps track of previous
     * error and time in private fields, so the only parameter is the error which has
     * just been calculated.
     * @param error Sum at this time
     * @return Time derivative of e, de/dt
     */
    double getDerivative(double error) {
        gettimeofday(&thisTime, nullptr);
        double timeDiff = (thisTime.tv_sec - prevTime.tv_sec) * 1e6;
        timeDiff = (timeDiff + (thisTime.tv_usec - prevTime.tv_usec)) * 1e-6;
        double derivative = (error - prevError) / (timeDiff);

        gettimeofday(&prevTime, nullptr);
        prevError = error;
        return derivative;
    }

private:
    double prevError;
    struct timeval thisTime, prevTime;
};

class drive {
public:
    explicit  drive() {
        zero_speed = 48;
        max_speed = 17;

        motor_left = 5;     // port for left motor
        motor_right = 1;    // port for right motor
        motor_camera = -1;  // port for the camera motor
    }

    void turn(int turn, int forward_speed) {
        stop();
        set_motor(motor_left, zero_speed + turn - fwd_speed);
        set_motor(motor_right, zero_speed + turn + fwd_speed);
        hardware_exchange();
    }

    void forward(int speed) {
        set_motor(motor_left, zero_speed - speed);
        set_motor(motor_right, zero_speed + speed);
        hardware_exchange();
    }

    void stop() {
        forward(0);
    }

    void lost() {
        forward(-5);
    }

    void look_up() {
        set_motor(motor_camera,10);
        hardware_exchange();
        std::this_thread::sleep_for (std::chrono::milliseconds(500));
        set_motor(motor_camera,0);
        hardware_exchange();
    }

    void look_down() {
        set_motor(motor_camera,-10);
        hardware_exchange();
        std::this_thread::sleep_for (std::chrono::milliseconds(500));
        set_motor(motor_camera,0);
        hardware_exchange();
    }

private:
    int max_speed;
    int zero_speed;
    int motor_left;
    int motor_right;
    int motor_camera;
};

class robot {
public:
    explicit robot(readCamera rc, drive dr) {
        cam = rc;
        dri = dr;

        // TODO optimise values of Kp and Kd
        // Start with Kp, increase slowly from 0 until robot starts swinging
        // Then increase Kd until movement is smooth
        Kp = 0;
        Kd = 0;
    }

    /**
     * Quadrant 1
     * Open the gate by exchange with the server over WiFi
     */
    void quadrant1() {}

    /**
     * Quadrant 2
     * Follow smooth wiggly line
     */
    void quadrant2() {
        bool run = true;
        int count = 0;

        // Initialise error and derivative (so frame 1 derivative is correct)
        double array[cam.CAM_WIDTH];
        cam.simplePixelsFromCamera(3, array, 120);
        cam.getDerivative(cam.getError(array));

        while (run) {
            followLine();

            count ++;
            if (count > 100) run = false;
        }
    }

    /**
     * Quadrant 3
     * Make sharp turns. Logic of direction selection.
     */
    void quadrant3() {}

    /**
     * Quadrant 4
     * Come close to the colored cylinders and drive to the finish line
     */
    void quadrant4() {}

private:
    readCamera cam;
    drive dri;
    float Kp, Kd;  // Coefficients

    /**
     * Using proportional formula, follow the black
     * line, keeping the error to a minimum
     */
    void followLine() {
        double array[cam.CAM_WIDTH];
        cam.simplePixelsFromCamera(3, array, 120);
        double error = cam.getError(array);
        double derivative = cam.getDerivative(error);

        // Continuously move forwards, and turn according to proportional formula
        dri.turn(int(Kp * error + Kd * derivative), 5);
    }
};




int main() {
    int err;
    readCamera rc;
    drive dr;
    robot rb(rc, dr);

    rb.quadrant1();
    rb.quadrant2();
    return 0;
}
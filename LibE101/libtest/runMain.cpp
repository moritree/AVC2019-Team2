//
// Created by Zuni Preece on 2019-05-23.
//

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <cmath>
#include <sys/time.h>
#include <cassert>
#include "E101.h"

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
    }

    /**
     * Calculates the error e, where e = 0 if the “center
     * of mass” of the array is in the center index.
     * @param array Array of CAM_WIDTH
     * @return e sum between -1 and 1
     */
    double getError(double* array) {
        // Sum error array and picture array
        double sum = 0.0;
        for(int i = 0; i < CAM_WIDTH; i ++) {
			double tmp_x = array[i];
			double tmp_y = (i - (CAM_WIDTH/2.0) + 0.5)/159.5;
			double tmp_z = tmp_x * tmp_y;
            sum = sum + tmp_z;
        }
        return (sum / CAM_WIDTH);
    }

    /**
     * Writes from camera input into 1-dimensional array for finding the error values.
     * Each index contains, restricted to a range of 0 to 1, the average value of:
     *  - For R/G/B channels: The ratio of this channel to the total whiteness
     *  - For the whiteness channel: Unprocessed value
     * @param channel R (0), G (1), B (2), or all (3)
     * @param array Array to write into
     */
    void pixelsFromCamera(int channel, double *array, int exclude_x) {
        // Check that the channel is valid
        assert(channel >= 0 && channel <= 3);
		double min = 255;
        take_picture();
	
        for (int i = int(exclude_x / 2); i < CAM_WIDTH - (exclude_x / 2); i ++) {
            // Add together this channel's values for every
            // pixel in this column
            for (int j = 0; j < CAM_HEIGHT; j ++) {
                if (channel == 3) {
                    // Whiteness channel needs no change
                    array[i] += get_pixel(j, i, channel);
                    if (get_pixel(j, i, channel) < min) {
						min = get_pixel(j, i, channel);
					}
                } else {
                    // Colour ratio for the selected channel
                    array[i] += get_pixel(j, i, channel) / (float)get_pixel(i, j, 3);
                }
            }
            // Restrict to range of 0 to 1
            array[i] *= 1.0/(CAM_HEIGHT * 255);
        }
    }

    void simplePixelsFromCamera(int channel, double *array, int row, int thresh) {
        take_picture();

        for (int i = 0; i < CAM_WIDTH; i ++) {
	    array[i] = 0;
            // Add together this channel's values for every pixel in the middle column
            if (channel == 3 && get_pixel(row, i, channel) <= thresh) {
                // Whiteness channel needs no change
                array[i] = get_pixel(row, i, channel) /255.0;
            } else {
                // Colour ratio for the selected channel
                array[i] = get_pixel(row, i, channel) / (float)get_pixel(row, i, 3);
            }
        }
    }

    bool isLine(double *array, double threshold) {
        int count = 0;
        for (int i = 0; i < CAM_WIDTH; i ++) {
            if (array[i] < threshold) count ++;
        }
        return (count > 10);
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
        zero_speed = 47;
        max_speed = 17;

        motor_left = 5;     // port for left motor
        motor_right = 1;    // port for right motor
        motor_camera = -1;  // port for the camera motor
    }

    void turn(int turn, int fwd_speed) {
        stop();
        set_motors(motor_left, zero_speed - turn + fwd_speed);
        set_motors(motor_right, zero_speed - turn - fwd_speed);
        hardware_exchange();
    }

    void forward(int speed) {
        set_motors(motor_left, zero_speed + speed);
        set_motors(motor_right, zero_speed - speed);
        hardware_exchange();
    }

    void stop() {
        forward(0);
    }

    void lost() {
        forward(-5);
    }

    void look_up() {
        set_motors(motor_camera,10);
        hardware_exchange();
        std::this_thread::sleep_for (std::chrono::milliseconds(500));
        set_motors(motor_camera,0);
        hardware_exchange();
    }

    void look_down() {
        set_motors(motor_camera,-10);
        hardware_exchange();
        std::this_thread::sleep_for (std::chrono::milliseconds(500));
        set_motors(motor_camera,0);
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
        Kp = 20;
        Kd = 3;
    }

    /**
     * Quadrant 1
     * Open the gate by exchange with the server over WiFi
     */
    void quadrant1() {
		//Set address and port
		char server_addr[15] = "130.195.6.196";
		int port = 1024;
		char request[24] = "Please";
		char password[24];
		
		int connectionValue = -1;
		int receiveValue = -1;	
		
		//Connect to server
		do {
			connectionValue = connect_to_server(server_addr, port);
		}while (connectionValue != 0);
		
		//Request password
		do {
			receiveValue = send_to_server(request);
			receive_from_server(password);
		}while (receiveValue != 0);
		
		//Send password
		send_to_server(password);
	}

    /**
     * Quadrant 2
     * Follow smooth wiggly line
     */
    void quadrant2() {

        bool run = true;
        int count = 0;

        // Initialise error and derivative (so frame 1 derivative is correct)
        double array[cam.CAM_WIDTH];
        cam.simplePixelsFromCamera(3, array, 230, 80);
        cam.getDerivative(cam.getError(array));

        while (run) {
            if (cam.isLine(array, 0.2)) { followLine(array, 3); }
            else  {
            printf("LOST\n");
                dri.lost();
            }
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

            
            count ++;
            if (count >= 150) run = false;
            // TODO check for red
        }
    }

    /**
     * Quadrant 3
     * Make sharp turns. Logic of direction selection.
     */
    void quadrant3() {
        // Initialise error and derivative (so frame 1 derivative is correct)
        double array[cam.CAM_WIDTH];
        cam.simplePixelsFromCamera(3, array, 230, 80);
        cam.getDerivative(cam.getError(array));
        int count = 0;
        
        while (cam.isLine(array, 0.3) && count < 200) {
            followLine(array, 2);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        printf("END LINE\n");
        while (!cam.isLine(array, 0.2)) {
            cam.simplePixelsFromCamera(3, array, 230, 70);
            dri.turn(2, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(600));
            printf("turn\n");
        }
        quadrant3();
    }

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
    void followLine(double *array, int drive) {
        cam.simplePixelsFromCamera(3, array, 230, 70);
        double error = cam.getError(array);
        double derivative = cam.getDerivative(error);
		
        // Continuously move forwards, and turn according to proportional formula
        dri.turn(int(Kp * error + Kd * derivative), drive);
        
        printf("Error: %.4f %.2f Deriv: %.4f %.2f\n", error, Kp * error, derivative, Kd * derivative);
    }
};

int main() {
	int err;
	err = init(0);

	std::this_thread::sleep_for (std::chrono::milliseconds(500));
	open_screen_stream();
	std::this_thread::sleep_for (std::chrono::milliseconds(500));
	
    readCamera rc;
    drive dr;
    robot rb(rc, dr);

	//dr.forward(10);
	//std::this_thread::sleep_for (std::chrono::milliseconds(5000));
    //rb.quadrant1();
    //rb.quadrant2();
    rb.quadrant3();
    dr.stop();
    
    printf("Done\n");
    printf("Err: %d\n", err);
    return 0;
}

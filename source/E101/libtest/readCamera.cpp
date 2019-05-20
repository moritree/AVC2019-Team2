//
// Created by Zuni Preece on 2019-05-13.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <string>
#include <fstream>
#include <cmath>
#include <sys/time.h>
#include <time.h>
#include <cassert>
#include "E101.h"

using namespace std;

int CAM_WIDTH = 320;
int CAM_HEIGHT = 240;

class readCamera {
public:
    explicit readCamera() {
    	prevError = 0.0;
    	gettimeofday(&prevTime, NULL);
    }

	/**
     * Gives the error of a specific colour channel.
     * @param channel R (0), G (1), B (2), or all (3)
     * @return error sum between -1 and 1
     */
    double getError(double *picture) {
        // Generate error array
        double error_array [CAM_WIDTH];
        for(int i = 0; i < CAM_WIDTH; i ++) {
            // Restrict to range of -1 to 1
            error_array[i] = i - (CAM_WIDTH/2.0) + 0.5;
        }

		// Sum error array and picture array
        double sum = 0;
        for(int i = 0; i < CAM_WIDTH; i ++) {
            sum += picture[i] * error_array[i];
        }
        return sum / 12800;
    }

	/**
	 * Fills array from the camera for the get_error function
	 * Average of the chosen channel in each column, but
	 *  restricted to a range between 0 and 1.
	 * @param channel R (0), G (1), B (2), or all (3)
	 */
	void pixelsFromCamera(int channel, double *array){
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
                    array[i] += get_pixel(i, j, channel) / get_pixel(i, j, 3);
                }
			}
			// Restrict to range of 0 to 1
			array[i] *= 1.0/(CAM_HEIGHT * 255);
		}
	}


    // TODO Find the derivative wrt time, of the error
    double getDerivative(double error){ 
    	gettimeofday(&thisTime, NULL);    	
    	double timeDiff = (thisTime.tv_sec - prevTime.tv_sec) * 1e6;
    	timeDiff = (timeDiff + (thisTime.tv_usec - prevTime.tv_usec)) * 1e-6; 
    	double derivative = (error - prevError) / (timeDiff);

    	gettimeofday(&prevTime, NULL);
    	prevError = error;
    	return derivative; 
    }

private:
    double prevError;
    struct timeval thisTime, prevTime;
};

/**
 * Tools for testing the readCamera class.
 */
class testCamera: public readCamera {
public:
	explicit testCamera() : readCamera() { }

	/**
	 * Fills array with 0, with 1s in the range specified by
	 *  the offset and width
	 */
	static void generateTestArray(double *array, int offset, int width) {
		if (offset + width > CAM_WIDTH) {
			printf("Attempt to access index %d\n", offset + width);
			printf("Cannot change pixels outside of array bounds.\n");
			return;
		}

		for(int i = 0; i < CAM_WIDTH; i ++) {
			if (i >= offset && i < offset + width) {
				array[i] = 1;
			} else {
				array[i] = 0;
			}
		}
	}

	/** Test getError */
	void getError_test1() {
		printf("~ getError() TEST BANK 1 ~\n");
		string result = "✓";
		double array[CAM_WIDTH];

		// Maximum left error (0)
		generateTestArray(array, 0, 160);
		if (getError(array) != -1.0) result = "✗";
		printf("Max left:\t%.2f%s\n", getError(array), result.c_str());

		// Maximum right error (+1)
		generateTestArray(array, 160, 160);
		if (getError(array) == 1.0) result = "✓";
		else result = "✗";
		printf("Max right:\t%.2f%s\n", getError(array), result.c_str());

		// Empty array
		generateTestArray(array, 0, 0);
		if (getError(array) == 0.0) result = "✓";
		else result = "✗";
		printf("Empty array:\t%.2f%s\n", getError(array), result.c_str());

		// Full array
		generateTestArray(array, 0, CAM_WIDTH);
		if (getError(array) == 0.0) result = "✓";
		else result = "✗";
		printf("Filled array:\t%.2f%s\n", getError(array), result.c_str());

		// Left to right (width 10)
		printf("Left to right (width 10): \t");
		double expected[6] = {-0.12, -0.07, -0.02, 0.02, 0.07, 0.12};
		for (int i = 0; i <= 5; i ++) {
			generateTestArray(array, i * 62, 10);
			if (abs(getError(array) - expected[i]) < 0.005) result = "✓";
			else result = "✗";
			printf("%.2f%s ", getError(array), result.c_str());
		}
		printf("\n");

		// Left to right (width 30)
		printf("Left to right (width 30): \t");
		double expected2[6] = {-0.34, -0.20, -0.07, 0.07, 0.20, 0.34};
		for (int i = 0; i <= 5; i ++) {
			generateTestArray(array, i * 58, 30);
			if (abs(getError(array) - expected2[i]) < 0.005) result = "✓";
			else result = "✗";
			printf("%.2f%s ", getError(array), result.c_str());
		}
		printf("\n\n");
	}

	void getDerivative_test1() {
		printf("~ getDerivative() TEST BANK 1 ~\n");		
		double array[CAM_WIDTH];

		printf("Instant min to max:\t\t");
		double sum = 0;
		for (int i = 0; i < 3; i ++){
			generateTestArray(array, 0, 160);
			getDerivative(getError(array));
			std::this_thread::sleep_for (std::chrono::milliseconds(200));
			generateTestArray(array, 160, 160);
			sum += getDerivative(getError(array));
		}
		if (abs(sum/3 - 10) < 1) result = "✓";
		else result = "✗";
		printf("%.2f%s (avg. of 3)\n", sum / 3, result.c_str());

		printf("Left to right (width 10): \t");
		sum = 0;
		generateTestArray(array, 0, 10);
		getDerivative(getError(array));
		for (int i = 1; i <= 5; i ++) {
			std::this_thread::sleep_for (std::chrono::milliseconds(100));
			generateTestArray(array, i * 62, 10);
			sum += getDerivative(getError(array));
		}
		if (abs(sum/5 - 0.5) < 0.05) result = "✓";
		else result = "✗";
		printf("%.2f%s (avg. of 5 steps)\n", sum / 5, result.c_str());

		printf("\n");
	}
private:
	string result;
};

// Test
int main() {
	//int err = init();
	//open_screen_stream();
	
	readCamera cam;

	testCamera test_cam;
	test_cam.getError_test1();
	test_cam.getDerivative_test1();
}


//
// Created by Zuni Preece on 2019-05-13.
//

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sys/time.h>
#include <cassert>
#include "E101.h"

using namespace std;

class readCamera {
public:

    int WIDTH = 320;
    int HEIGHT = 240;
    readCamera(){};
    
	/**
     * Gives the error, between between -1 (full left) and
     *  1 (full right), of a specific colour channel.
     * @param channel R (0), G (1), B (2), or all (3)
     * @return error sum (-1..1)
     */
    double get_error(double *picture) {
        // Generate error array
        double error_array [WIDTH];
        for(int i = 0; i < WIDTH; i ++) {
            // Restrict to range of -1 to 1
            error_array[i] = (i - (WIDTH/2.0)) / (WIDTH/2.0);
        }

		// Sum error array and picture array
        double sum = 0;
        for(int i = 0; i < WIDTH; i ++) {
            sum += picture[i] * error_array[i];
        }
        return sum;
    }

	/**
	 * 
	 */
	void pixels_from_cam(int channel, double *array){
		// Check that the channel is valid
        assert(channel >= 0 && channel <= 3);
        
		take_picture();
		
		for (int i = 0; i < WIDTH; i ++) {
			// Add together this channel's values for every
            // pixel in this column
			for (int j = 0; j < WIDTH; j ++) {
				if (channel == 3) {
                    // Whiteness channel needs no change
                    array[i] += get_pixel(i, j, channel);
                } else {
                    // Colour ratio for the selected channel
                    array[i] += get_pixel(i, j, channel) / get_pixel(i, j, 3);
                }
			}
			// Restrict to range of 0 to 1
			array[i] *= 1.0/(HEIGHT * 255);
		}
	}

    // TODO Find the derivative wrt time, of the error
    double getDerivative(){
		return 0.0;
    }

private:
    double prevError = 0;
    struct timeval prevTime;
};

int main() {
	int err = init();
	open_screen_stream();
	
	readCamera cam;
	double picture[320] = {0};
	cam.pixels_from_cam(3, picture);
	double error = cam.get_error(picture);
	printf("ERROR: %f\n", error);
	printf("end Err: %d\n", err);
}


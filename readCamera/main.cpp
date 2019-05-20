//
// Created by Zuni Preece on 2019-05-13.
//

#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <cassert>
#include "E101.h"

using namespace std;

class readCamera {
public:

    int WIDTH = 320;
    int HEIGHT = 240;

    /**
     * Gives the error, between between -1 (full left) and
     *  1 (full right), of a specific colour channel.
     * @param channel R (0), G (1), B (2), or all (3)
     * @return error (-1..1)
     */
    double get_error(int channel){
        // Check that the channel is valid
        assert(channel >= 0 && channel <= 3);

        take_picture();

        // Generate error array
        double error_array [WIDTH];
        for(int i = 0; i < WIDTH; i ++) {
            // Restrict to range of -1 to 1
            error_array[i] = (i - (WIDTH/2.0)) / (WIDTH/2.0);
        }

        double tmp_colour;
        double sum = 0;
        for(int i = 0; i < WIDTH; i ++) {
            tmp_colour = 0;

            // Add together this channel's values for every
            // pixel in this column
            for(int j = 0; j < HEIGHT; j ++) {
                if (channel == 3) {
                    // Whiteness channel needs no change
                    tmp_colour += get_pixel(i, j, channel);
                } else {
                    // Colour ratio for the selected channel
                    tmp_colour += get_pixel(i, j, channel) / get_pixel(i, j, 3);
                }
            }

            // Restrict to range of 0 to 1
            tmp_colour *= 1.0/(HEIGHT * 255);

            // Multiply with error array
            tmp_colour *= error_array[i];

            // Add to error sum
            sum += tmp_colour;
        }

        // Return error sum
        return sum;
    }

    // TODO Find the derivative wrt time, of the error
    double getDerivative(){
        double currError = get_error(3);
        prevError = currError;
        return 0.0;
    }

private:
    // Fields for derivative
    double prevError = 0;
    struct timeval prevTime;
};
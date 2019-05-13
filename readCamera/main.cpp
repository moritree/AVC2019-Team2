//
// Created by Zuni Preece on 2019-05-13.
//

#include <stdio.h>
#include <iostream>
#include "E101.h"

using namespace std;

int WIDTH = 320;
int HEIGHT = 240;

/** Takes a picture
 *
 * @param channel R (0), G (1), B (2), or all (3)
 * @return error: negative is left, positive is right
 */
double get_error(int channel){
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
            tmp_colour += get_pixel(i, j, channel);
        }

        // Restrict to range of -1 to 1
        tmp_colour *= 1.0/(HEIGHT * 255);

        // Multiply with error array
        tmp_colour *= error_array[i];

        // Add to error sum
        sum += tmp_colour;
    }

    // Return error sum
    return sum;
}
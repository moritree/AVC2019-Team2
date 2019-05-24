//
// Created by Zuni Preece on 2019-05-21.
//

#include "../include/readCamera.h"
#include "../include/E101.h"

readCamera::readCamera() {
    prevError = 0.0;
    gettimeofday(&prevTime, nullptr);
    CAM_WIDTH = 320;
    CAM_HEIGHT = 240;
    open_screen_stream();
}

double readCamera::getError(double *array) {
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

void readCamera::pixelsFromCamera(int channel, double *array){
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

void readCamera::simplePixelsFromCamera(int channel, double *array, int row) {
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

double readCamera::getDerivative(double error){
    gettimeofday(&thisTime, nullptr);
    double timeDiff = (thisTime.tv_sec - prevTime.tv_sec) * 1e6;
    timeDiff = (timeDiff + (thisTime.tv_usec - prevTime.tv_usec)) * 1e-6;
    double derivative = (error - prevError) / (timeDiff);

    gettimeofday(&prevTime, nullptr);
    prevError = error;
    return derivative;
}
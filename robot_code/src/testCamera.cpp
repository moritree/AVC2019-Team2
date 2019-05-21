//
// Created by Zuni Preece on 2019-05-21.
//

#include "testCamera.h"
#include <unistd.h>
#include <thread>

testCamera::testCamera() : readCamera() {
    getError_test();
    getDerivative_test();
}

void testCamera::generateTestArray(double *array, int offset, int width) {
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

void testCamera::getError_test() {
    printf("~ getError() TESTS ~\n");
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

void testCamera::getDerivative_test() {
    printf("~ getDerivative() TESTS ~\n");
    double array[CAM_WIDTH];

    printf("One step min to max:\t\t");
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
    printf("%.2f%s (avg. of 3 trials)\n", sum / 3, result.c_str());

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
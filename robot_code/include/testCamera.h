//
// Created by Zuni Preece on 2019-05-21.
//

#ifndef TRY2_TESTCAMERA_H
#define TRY2_TESTCAMERA_H

#include "readCamera.h"

using namespace std;

/**
 * Tools for testing the readCamera class.
 * Inherits readCamera.
 */
class testCamera: public readCamera {
public:
    /** Automatically runs all tests when created */
    explicit testCamera();

    /**
     * Fills array with 0, with 1s in the range specified by
     *  the offset and width
     */
    void generateTestArray(double *array, int offset, int width);

    void getError_test();
    void getDerivative_test();
private:
    string result;
};

#endif //TRY2_TESTCAMERA_H

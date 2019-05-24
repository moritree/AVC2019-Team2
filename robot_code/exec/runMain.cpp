//
// Created by Zuni Preece on 2019-05-23.
//

#include <iostream>
#include "robot.h"
#include "readCamera.h"
#include "drive.h"

int main() {
    readCamera rc;
    drive dr;
    robot rb(rc, dr);

    rb.quadrant1();
    rb.quadrant2();
    return 0;
}
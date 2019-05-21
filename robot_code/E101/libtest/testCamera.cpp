//
// Created by Zuni Preece on 2019-05-20.
//

#include <iostream>
#include <stdio.h>
#include <fstream>
#include "E101.h"


int main() {
	int err = init();
	open_screen_stream();
	
	readCamera cam;
	double picture[320] = {0};
	cam.pixels_from_cam(3, picture);
	cam.get_error(picture);
	printf("Err: %d", err);
}

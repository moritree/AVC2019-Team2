#include "E101.h"
#include <iostream>
#include <stdio.h>

int main()
{
  int err;
  printf(" Hello\
  n");
  err = init();
  
  int img_height = 240;
  int img_width = 320;
  
  int totalred = 0;
  int bright = 0;

  int count = 0;
  int framerate = 10;
  int ain= 0;
  select_IO(0, 0);
  open_screen_stream();
  
  while(count< 80)
	{
		take_picture();
		update_screen();
	
		totalred = 0;
		bright = 0;
		
		for (int i = 0; i < img_height; i++){
			for (int j = 0; j < img_width; j++){
				int R = (int)get_pixel(i, j, 0);
				int G = (int)get_pixel(i, j, 1);
				int B = (int)get_pixel(i, j, 2);
				
				if(R < G && R < B){
					
					
				}
				
				
				totalred += (int)get_pixel(i, j, 0);
				bright += (int)get_pixel(i, j, 3);
			}
		}
      		
		count++;
		printf("Frame: %i\n", count);
		printf("Red: %i \t Bright: %i\n", totalred, bright);
		sleep1(0,framerate*10000);
	}
    close_screen_stream();
	printf("Finshed!\n");
	
	return 0;
}

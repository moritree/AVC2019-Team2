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
  
  double totalred = 0;
  double bright = 0;
  double redratio = totalred/bright;
  
  int frame = 0;
  int framerate = 10;
  int ain= 0;
  open_screen_stream();
  
  while(frame < 80)
	{
		take_picture();
		update_screen();
	
		totalred = 0;
		bright = 0;
		
		for (int i = 0; i < img_height; i++){ // should height/width be the other way around? Does it matter?
			for (int j = 0; j < img_width; j++){
				int R = (int)get_pixel(i, j, 0);
				int G = (int)get_pixel(i, j, 1);
				int B = (int)get_pixel(i, j, 2);
				
				if(R > G && R > B){
					totalred += R;	
				}
				bright += (int)get_pixel(i, j, 3);
			}
		}
      		
		frame++;
		printf("Frame: %i\n", frame);
		printf("Red: %.2f \t Bright: %.2f\n", totalred, bright);
		printf("Red ratio: %f\n", redratio);
		
		//sleep1(0,framerate*10000);
	}
    close_screen_stream();
	printf("Finshed!\n");
	
	return 0;
}

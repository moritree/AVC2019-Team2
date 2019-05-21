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
  int border = 2;
  
  double totalred = 0;
  double bright = 0;
  double redratio = totalred/bright;
  
  int frame = 0;
  int framerate = 10;
  int ain= 0;
  select_IO(0, 0);
  open_screen_stream();
  
  int borderimage[img_height + border][img_width + border][4];
  
  for(int i = 0; i < (img_width + border); i++){
	  for(int j = 0; j < (img_height + border); j++){
		  if(i < border-1 || j < border-1 || j > img_height-1 || i > img_width-1){
			  // colour pixel black
		  }
		  else{
			  // copy original pixel
		  }
	  }
  }
  
  
  
  
  
  
  while(frame < 80)
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
		
		sleep1(0,framerate*10000);
	}
    close_screen_stream();
	printf("Finshed!\n");
	
	return 0;
}

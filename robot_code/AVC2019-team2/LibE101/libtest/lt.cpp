#include <iostream>
#include "E101.h"

using namespace std;

int main()
{
  int err;
  cout<<" Hello"<<endl;
  err = init(0);
  cout<<"After init() error="<<err<<endl;
  
  int count = 0;
  open_screen_stream();
  while(count < 500)
  {
	 set_motors(1,48);
	 set_motors(5,48);
     set_digital(0,1);
	 set_digital(1,1);
	 set_digital(2,1);
	 set_digital(3,1);
	 set_digital(4,1);
	 hardware_exchange();
	 take_picture();
	 update_screen();
	 //sleep1(10);
     set_digital(0,0);
	 set_digital(1,0);
	 set_digital(2,0);
	 set_digital(3,0);
	 set_digital(4,0);
	 hardware_exchange();
	 take_picture();
	 update_screen();
	 
	 //char fname[5]={'r','r','r','r','r'};
	 //save_picture(fname);
	 //display_picture(0,200000);
	 	
	 //sleep1(1,200);
       count++;
    }
    //save_picture("file1");
    close_screen_stream();
    stoph();
	
	return 0;
}

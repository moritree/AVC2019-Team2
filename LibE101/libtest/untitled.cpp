#include <iostream>
#include "E101.h"

using namespace std;

int main()
{
  //int err;
  //cout<<" Hello"<<endl;
  //err = init(0);
  

	 set_motors(1,43);
	 set_motors(5,50);
	 hardware_exchange();

    stoph();
	//cout<<"After init() error="<<err<<endl;
  
	return 0;
}


//Drive libary
#include <stdio.h>
#include "E101.h"
bool debug = true;
int motor_left = 5; // set to the port for left motor		positive clockwise	negitve anticlock wise
int motor_right = 1;// set to the port for right motor
int motor_camera = -1; // set to the port for the camera motor
double zero_speed = 47; // set for speed at which motor is stationary


void drive (double speed) {

	//if (((speed > 0)? speed : 0-speed) < 15){
	for (int i = 0; i < 500; i++){
		set_motors(1,50);
		set_motors(5,50);
		hardware_exchange();
		//sleep1(5);
	}	
	

	/*else{
		printf("ERROR \n speed enterd is to big \n");
		if (debug) {printf("speed : %.2f \n",speed);}
		* 
	}*/

}
int main (){
	drive(55);
	return 0;
}

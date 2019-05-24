//Drive libary
#include <iostream>
#include <stdio.h>
#include "E101.h"

using namespace std;

bool debug = true;
int motor_left = 5; // set to the port for left motor		positive clockwise	negitve anticlock wise
int motor_right = 1;// set to the port for right motor
int motor_camera = -1; // set to the port for the camera motor
double zero_speed = 47; // set for speed at which motor is stationary


void drive (double speed = 10) {

	if (((speed > 0)? speed : 0-speed) < 15){
		set_motors(motor_left,zero_speed+speed);
		set_motors(motor_right,zero_speed-speed);
		hardware_exchange();
		sleep1(5);
	}	

	else{
		printf("ERROR \n speed enterd is to big \n");
		if (debug) {printf("speed : %.2f \n",speed);}
	}

}

void park () {
	drive(0);
}



void turn ( double direction ,bool stationary = false, double scale = 2){
	if (stationary){
		park();
		set_motors(motor_left, zero_speed + direction);
		set_motors(motor_right, zero_speed + direction);
		if (debug) {printf("stationary turn\n");}
	}

	else{ /// this will need testing for how the motors work
		set_motors(motor_left,zero_speed +  direction/scale);
		set_motors(motor_right,zero_speed + direction/scale);
		if (debug) {printf("turn\n");}
	}
	hardware_exchange();
	sleep1(5);
}





void lost(){
	drive(-5);
}

/* if way to tell angle then need to update
 * angle to got 90 or equivalent and 0 or equivalent
*/

void look_up(int time = 500){
	set_motors(motor_camera,10);
	//hardware_exchange()
	sleep1(time);//wait 5 seconds
	set_motors(motor_camera,0);
	sleep1(5);
}

void look_down(int time = 500){
	set_motors(motor_camera,-10);
	hardware_exchange();
	sleep1(time);//wait 5 seconds
	set_motors(motor_camera,0);
	hardware_exchange();
	sleep1(5);
}

int main (){
	 int err;
  //cout<<" Hello"<<endl;
  err = init(0);
	
	park();

	stoph();
	cout<<"After init() error="<<err<<endl;

	return 0;
}

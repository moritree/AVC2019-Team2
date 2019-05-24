class Robot { //decalarations
private:
	int v_left, v_right, cam_tilt;
	int dv;
	double line_error;
	int quadrant;
	const int cam_width = 320;
	const int cam_height = 240;
	const int v_left_go = 52;
	const int v_right_go = 43;
	double kp = 0.05;
	int line_present = 1;
public:
	Rob () {} ; //default constructor
	int InitHardware ();
	void ReadSetMotors ();
	void SetMotors ();
	int MeasureLine () ;
	int FollowLine ();
};

#include <iostream>
#include "E101.h"

int Robot::FollowLine (){
	MeasureLine();
	if (line_present) {
		dv = (int)(line_error∗kp);
		//dv = 0;
		v_left = v_left_go + dv;
		v_right = v_right_go + dv;
		cout<<" line_error = "<<line_error<<"dv="<<dv;
		SetMotors();
	} else {
		//go back
		cout<<" Line missing"<<endl;
		v_left = 39;
		v_right = 55;
		SetMotors ();
		sleep1 (100);
	}
	return 0;
}

int main () { //example of main() invoking the class
Robot robot;
robot.InitHardware();
int count = 0;
open_screen_stream();
while (count < 5000)
{
	robot.MeasureLine();
	robot.FollowLine();
	count++;
}
//save_picture("file1");
close_screen_stream();
stoph();
return 0;
}

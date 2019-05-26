//Drive libary
class Drive{
	public :
		static void turn ( double direction ,bool stationary = false ,double scale = 2  ); // turns robot depending on value entered
																// stationary is for if the robot should turn on spot
		static void drive ( double speed = 10); // starts robot going forward at value enterd
		
		static void park(); 	// stops robot
		
		static void look_up(int time = 5000); // make robot look up
		
		static void look_down(int time = 5000); // make robot look down
		
		static void lost(); // if lost go back
		
		
	private:
		bool debug;
		int motor_left;
		int motor_right;
		int motor_camera;
		double zero_speed;
};


void stop(int){};
int init(){ return 0; };
int take_picture(){ return 0; };
int save_picture(char fn[5]){ return 0; };
char get_pixel( int row,int col, int color){ return 0; };
int set_pixel(int row, int col, char red, char green,char blue){ return 0; };
void convert_camera_to_screen(){};
int open_screen_stream(){ return 0; };
int close_screen_stream(){ return 0; };
int update_screen(){ return 0; };
int display_picture(int delay_sec,int delay_usec){ return 0; };
int set_motor(int motor,int speed){ return 0; };
int sleep1(int sec, int usec){ return 0; };
int select_IO(int chan, int direct){ return 0; };
int write_digital(int chan,char level){ return 0; };
int read_digital(int chan){ return 0; };
int read_analog(int in_ch_adc){ return 0; };
int set_PWM(int chan, int value){ return 0; };
int set_PWM_frequency(int chan, int freq){ return 0; };
int set_servo(int chan, int value){ return 0; };
int connect_to_server( char server_addr[15],int port){ return 0; };
int send_to_server(char message[24]){ return 0; };
int receive_from_server(char message[24]){ return 0; };


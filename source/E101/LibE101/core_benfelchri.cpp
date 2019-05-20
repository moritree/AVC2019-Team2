#include <iostream>
#include "E101.h"
using namespace std;

int main() {
    int WIDTH = 320;
    int HEIGHT = 240;

    init();
    open_screen_stream();
    for (int i = 0; i < 1000; i++) {
        take_picture();
        update_screen();
        for(int row = 0; row < WIDTH; row++) {
            for (int col = 0; col < HEIGHT; col++) {
                int red = (int)get_pixel(row, col, 0);
                int green = (int)get_pixel(row, col, 1);
                int blue = (int)get_pixel(row, col, 2);
            }
        }
    }
    close_screen_stream();
}

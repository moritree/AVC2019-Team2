#include "E101.h"
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;

int main()
{
	ifstream image;
    image.open("ruby.png");
    if(image.is_open()){
	    cout<< "Opened successfully\n";
    }
    else{
	    cout<< "Could not open file\n";
    } 
    //get length of file:
    image.seekg(0, image.end);
	int n = image.tellg();
	image.seekg (0, image.beg);
	cout << n;
	return 0;
}

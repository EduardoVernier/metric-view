using namespace std;


#include <stdio.h>              //for printing the help text
#include <math.h>               //for various math functions
#include <string.h>
#include <GL/glut.h>            //the GLUT graphics
#include <GL/glui.h>

#include <ctime>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	printf("Fluid Flow Simulation and Visualization\n");
	printf("=======================================\n");
	printf("Click and drag the mouse to steer the flow!\n");
	printf("T/t:   increase/decrease simulation timestep\n");
	printf("S/s:   increase/decrease hedgehog scaling\n");
	printf("c:     toggle direction coloring on/off\n");
	printf("V/v:   increase decrease fluid viscosity\n");
	printf("x:     toggle drawing matter on/off\n");
	printf("y:     toggle drawing hedgehogs on/off\n");
	printf("m:     toggle thru scalar coloring\n");
	printf("a:     toggle the animation on/off\n");
	printf("q:     quit\n\n");

	return 0;
}

#pragma once
#include "Cube.h"

class Cube;

class Display {
private:
	char screen[20][40];
	/* tValue is plugged into an equation to help determine 
	   projection of a 3D object onto a plane */
	double tValue;
	double camPoint[3]; //the x-y-z values of camPoint
	double planeConstant; //the equation of the display surface plane is 0 = y + planeConstant
	double directionVector[3]; // the direction vector from camera point to cube point
public:
	Display();
	//setters
	void setTValue(double newTVal);
	void setCamPoints(double x, double y, double z);
	void setPlaneConstant(double newPC);
	void setDirectionVector(double x, double y, double z);

	//getters
	double getTValue();
	double* getCamPointArray();
	double getPlaneConstant();
	double* getDirectionVector();

	//maps the 3D points to a 2D display
	void ThreeDToTwoD(Cube* changeCube);

	void displayCube(Cube cube);
	//finds the next odd integer in the positive direction
	int findNextOddIntUp(double number);
	//finds the next odd integer in the negative direction
	int findNextOddIntDown(double number);
	//similar to findNextOddIntUp but it's in the x direction
	double findNextXMidpoint(double number);
	double findLastXMidpoint(double number);
	//determines x index based on a midpoint passed to it
	int detXIndex(double number);
	//determines Z index based on a midpoint passed to it
	int detZIndex(int number);
	//return the character to be printed for a certain
	char charType(int edges[3][2]);
};
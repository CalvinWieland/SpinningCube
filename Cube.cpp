#include "Cube.h"

Cube::Cube () {
	//the default points for a cube are (10,10,10), (10,10,-10), ... (-10,-10,-10)
	xCoordinates[0] = 10;
	yCoordinates[0] = 10;
	xCoordinates[1] = 10;
	yCoordinates[1] = -10;
	xCoordinates[2] = -10;
	yCoordinates[2] = 10;
	xCoordinates[3] = -10;
	yCoordinates[3] = -10;
	//the default value for rotationSize is PI/12
	rotationSize = PI / 12;
	//rotation Calculation initial value is 0
	rotationCalculation = 0;

	for (int i = 0; i < 2; i++) {
		centerOfCubeTwoD[i] = -1;
	}

	//defulat value for slots is -1
	for (int i = 0; i < 4; i++) {
		invalidPoints[i] = -1;
	}
	cubeLength = 20;

	insNum = 0;

	for (int i = 0; i < 2; i++) {
		for (int j = 1; j >= 0; j--) {
			pointsMap[0][i][j] = insNum;
			insNum++;
		}
	}
	for (int i = 1; i >= 0; i--) {
		for (int j = 1; j >= 0; j--) {
			pointsMap[1][i][j] = insNum;
			insNum++;
		}
	}

	for (int i = 0; i < 3; i++) {
		currentTestPoint[i] = 0;
	}
}

void Cube::setCoordinates() {
	//multCoefficient maintains the cubeSize
	double multCoefficient = ((cubeLength) / 2) * (1 / sin(PI/4));
	xCoordinates[0] = multCoefficient * (std::round(sin(rotationCalculation) * 1000.0) / 1000.0);
	yCoordinates[0] = multCoefficient * (std::round(-cos(rotationCalculation) * 1000.0) / 1000.0);
	zCoordinates[0] = getCubeLength() / 2;

	xCoordinates[1] = multCoefficient * (std::round(sin(rotationCalculation) * 1000.0) / 1000.0);
	yCoordinates[1] = multCoefficient * (std::round(-cos(rotationCalculation) * 1000.0) / 1000.0);
	zCoordinates[1] = (-1 * getCubeLength()) / 2;

	xCoordinates[2] = multCoefficient * (std::round(cos(rotationCalculation) * 1000.0) / 1000.0);
	yCoordinates[2] = multCoefficient * (std::round(sin(rotationCalculation) * 1000.0) / 1000.0);
	zCoordinates[2] = getCubeLength() / 2;

	xCoordinates[3] = multCoefficient * (std::round(cos(rotationCalculation) * 1000.0) / 1000.0);
	yCoordinates[3] = multCoefficient * (std::round(sin(rotationCalculation) * 1000.0) / 1000.0);
	zCoordinates[3] = (-1 * getCubeLength()) / 2;

	xCoordinates[4] = multCoefficient * (std::round(-sin(rotationCalculation) * 1000.0) / 1000.0);
	yCoordinates[4] = multCoefficient * (std::round(cos(rotationCalculation) * 1000.0) / 1000.0);
	zCoordinates[4] = getCubeLength() / 2;

	xCoordinates[5] = multCoefficient * (std::round(-sin(rotationCalculation) * 1000.0) / 1000.0);
	yCoordinates[5] = multCoefficient * (std::round(cos(rotationCalculation) * 1000.0) / 1000.0);
	zCoordinates[5] = (-1 * getCubeLength()) / 2;

	xCoordinates[6] = multCoefficient * (std::round(-cos(rotationCalculation) * 1000.0) / 1000.0);
	yCoordinates[6] = multCoefficient * (std::round(-sin(rotationCalculation) * 1000.0) / 1000.0);
	zCoordinates[6] = getCubeLength() / 2;

	xCoordinates[7] = multCoefficient * (std::round(-cos(rotationCalculation) * 1000.0) / 1000.0);
	yCoordinates[7] = multCoefficient * (std::round(-sin(rotationCalculation) * 1000.0) / 1000.0);
	zCoordinates[7] = (-1 * getCubeLength()) / 2;
}

//rotationSize says how many rotations the cube should make. If it's 1, the cube rotates 2 * PI couts
void Cube::rotateCube() {
	//update rotationCalculation by rotationSize
	this->rotationCalculation += rotationSize;
	//update the coordinates of the Cube
	setCoordinates();
}

double* Cube::getXCoordinates() {
	return this->xCoordinates;
}
double* Cube::getYCoordinates() {
	return this->yCoordinates;
}
double* Cube::getZCoordinates() {
	return this->zCoordinates;
}
double Cube::getRotationCalculation() {
	return this->rotationCalculation;
}
double* Cube::getXCoordinates2D() {
	return this->xCoordinatesTwoD;
}
double* Cube::getZCoordinates2D() {
	return this->zCoordinatesTwoD;
}

//
void Cube::setCenter(double camPoint[], double tValue, double directionComp, double directionComp2) {
	centerOfCubeTwoD[0] = camPoint[0] + directionComp * tValue;
	centerOfCubeTwoD[1] = camPoint[2] + directionComp2 * tValue;
}

void Cube::setCenter(double x, double z) {
	centerOfCubeTwoD[0] = { x };
	centerOfCubeTwoD[1] = { z };
}

//use the campoint, tvalue, and directionComp to determine coordinate at index
void Cube::setCoordinatesTwoDX(int index, double camPoint, double tValue, double directionComp) {
	xCoordinatesTwoD[index] = camPoint + directionComp * tValue;
}

void Cube::setCoordinatesTwoDZ(int index, double camPoint, double tValue, double directionComp) {
	zCoordinatesTwoD[index] = camPoint + directionComp * tValue;
}

//set a new value for the coordinate with index
void Cube::setCoordinatesTwoDZ(int index, double newValue) {
	zCoordinatesTwoD[index] = newValue;
}

void Cube::setCoordinatesTwoDX(int index, double newValue) {
	xCoordinatesTwoD[index] = newValue;
}

void Cube::setRotationCalculation(double newRotationCalculation) {
	this->rotationCalculation = newRotationCalculation;
}

void Cube::setRotationSize(double newRotationSize) {
	this->rotationSize = newRotationSize;
}

//Find four closest points and disqualify them from being printed to screen
void Cube::findInvalidPoints(Display& camera) {
	//set invalid points array to all -1
	for (int i = 0; i < 4; i++) {
		invalidPoints[i] = -1;
	}
	//numInvalid tracks how many invalid points have been found
	int numInvalid = 0;

	//create array of pairs. the double value is the distance from campoint to point on cube. int is the index
	std::pair<int, double> pointDistances[8] = { std::make_pair(-1, -1) };

	bool collisionDetected = false;

	//make array for the four points which should be tested for collision
	int testPoints[4] = { -1 };

	double tempX = -1, tempY = -1, tempZ = -1;

	//find the lengths of all of the distances from the cube points to the camera point
	for (int i = 0; i < 8; i++) {
		tempX = camera.getCamPointArray()[0] - xCoordinates[i];
		tempY = camera.getCamPointArray()[1] - yCoordinates[i];
		tempZ = camera.getCamPointArray()[2] - zCoordinates[i];
		pointDistances[i] = std::make_pair(i, sqrt(tempX * tempX + tempY * tempY + tempZ * tempZ));
	}
	//sort array (a possible place for optimization is here with the sorting)
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8 - i - 1; j++) {
			if (pointDistances[j].second > pointDistances[j + 1].second) {
				std::pair<int, double> temp = std::make_pair(pointDistances[j + 1].first, pointDistances[j + 1].second);
				pointDistances[j + 1] = pointDistances[j];
				pointDistances[j] = temp;
			}
		}
	}

	//load testPoints with the four point we need to check
	for (int i = 0; i < 4; i ++) {
		testPoints[i] = pointDistances[i+4].first;
	}

	//oppositeToCurPoint is the common point of all of the parrelograms which will be tested
	bool oppositeToCurPoint[3] = { 0, 0, 0 };

	//make arrays which stores the points for each parallelogram to be tested
	std::tuple<bool, bool, bool> para1[4];
	std::tuple<bool, bool, bool> para2[4];
	std::tuple<bool, bool, bool> para3[4];

	//for each point that needs to be checked, check it for collisions
	for (int i = 0; i < 4; i++) {
		collisionDetected = false;

		//find the current point in terms of 
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				for (int l = 0; l < 2; l++) {
					if (testPoints[i] == pointsMap[j][k][l]) {
						//point found, set currentTestPoint and exit for loop
						currentTestPoint[0] = j;
						currentTestPoint[1] = k;
						currentTestPoint[2] = l;
						j = 5;
						k = 5;
						l = 5;
					}
				}
			}
		}
		oppositeToCurPoint[0] = !currentTestPoint[0];
		oppositeToCurPoint[1] = !currentTestPoint[1];
		oppositeToCurPoint[2] = !currentTestPoint[2];

		//find all parallelograms 
		makeParallelograms(para1, para2, para3, oppositeToCurPoint);

		//function which takes the parallelogram and currentTestPoint and checks for collision
		if (isCollision(para1, testPoints[i], camera) == 1) {
			collisionDetected = 1;
		}
		if (isCollision(para2, testPoints[i], camera) == 1) {
			collisionDetected = 1;
		}
		if (isCollision(para3, testPoints[i], camera) == 1) {
			collisionDetected = 1;
		}
		//if a collision is found, 
		if (collisionDetected == true) {
			invalidPoints[numInvalid] = testPoints[i];
			numInvalid++;
		}

	}
	for (int i = 0; i < 4; i++) {
		if (invalidPoints[i] != -1) {
			//invalidPoints[numInvalid] = i;
			//numInvalid++;

			xCoordinates[invalidPoints[i]] = std::numeric_limits<double>::max();
			yCoordinates[invalidPoints[i]] = std::numeric_limits<double>::max();
			zCoordinates[invalidPoints[i]] = std::numeric_limits<double>::max();

			xCoordinatesTwoD[invalidPoints[i]] = std::numeric_limits<double>::max();
			zCoordinatesTwoD[invalidPoints[i]] = std::numeric_limits<double>::max();
		}
	}
}

int* Cube::getInvalidPoints() {
	return invalidPoints;
}

double Cube::getCubeLength() {
	return this->cubeLength;
}

double* Cube::getCenterOfCubeTwoD() {
	return this->centerOfCubeTwoD;
}

void Cube::makeParallelograms(std::tuple<bool, bool, bool> para1[],
	std::tuple<bool, bool, bool> para2[], std::tuple<bool, bool, bool> para3[],
	bool oppositeToCurPoint[]) {
	//set up the parallelograms such that opposite points are in indexes
	//0 and 3, and 1 and 2, where index 0 is the oppositeToCurPoint
	//para1 is parallel to the z axis
	para1[0] = std::make_tuple(oppositeToCurPoint[0], oppositeToCurPoint[1], oppositeToCurPoint[2]);
	para1[3] = std::make_tuple(!oppositeToCurPoint[0], !oppositeToCurPoint[1], oppositeToCurPoint[2]);
	para1[1] = std::make_tuple(oppositeToCurPoint[0], !oppositeToCurPoint[1], oppositeToCurPoint[2]);
	para1[2] = std::make_tuple(!oppositeToCurPoint[0], oppositeToCurPoint[1], oppositeToCurPoint[2]);

	//para2 is connected to para1 and para3. para2 is connected to the right edge of
	//oppositeToCurPoint, connected to para1
	para2[0] = std::make_tuple(oppositeToCurPoint[0], oppositeToCurPoint[1], oppositeToCurPoint[2]);
	para2[3] = std::make_tuple(!oppositeToCurPoint[0], oppositeToCurPoint[1], !oppositeToCurPoint[2]);
	para2[1] = std::make_tuple(oppositeToCurPoint[0], oppositeToCurPoint[1], !oppositeToCurPoint[2]);
	para2[2] = std::make_tuple(!oppositeToCurPoint[0], oppositeToCurPoint[1], oppositeToCurPoint[2]);

	//para3 is connected to para1 and para2. para3 is connected to the left edge of oppositeToCurPoint,
	//oppositeToCurPoint, connected to para1
	para3[0] = std::make_tuple(oppositeToCurPoint[0], oppositeToCurPoint[1], oppositeToCurPoint[2]);
	para3[3] = std::make_tuple(oppositeToCurPoint[0], !oppositeToCurPoint[1], !oppositeToCurPoint[2]);
	para3[1] = std::make_tuple(oppositeToCurPoint[0], oppositeToCurPoint[1], !oppositeToCurPoint[2]);
	para3[2] = std::make_tuple(oppositeToCurPoint[0], !oppositeToCurPoint[1], oppositeToCurPoint[2]);
}

//functions accepts a tuple of booleans and returns the point number it corresponds to
int Cube::getPointNum(std::tuple<bool, bool, bool> &testTuple) {
	//if the first
	if (std::get<0>(testTuple) == true) {
		if (std::get<1>(testTuple) == true) {
			if (std::get<2>(testTuple) == true) {
				return 4;
			}
			else {
				return 5;
			}
		}
		else {
			if (std::get<2>(testTuple) == true) {
				return 6;
			}
			else {
				return 7;
			}
		}
	}
	else {
		if (std::get<1>(testTuple) == true) {
			if (std::get<2>(testTuple) == true) {
				return 2;
			}
			else {
				return 3;
			}
		}
		else {
			if (std::get<2>(testTuple) == true) {
				return 0;
			}
			else {
				return 1;
			}
		}
	}

	return -1;
}

//uses currentTestPoint (data memeber) and parallelogram to check if there is a collision
//returns 1 if a collision is detected
int Cube::isCollision(std::tuple<bool, bool, bool> parallelogram[], int testPoint, Display& camera) {

	//create lines which will be used to calculate the plane equation
	double PQ[3] = { -1 };
	double PR[3] = { -1 };

	//cross product of PQ and PR, and later repurpoused to find determining cross products
	std::tuple<double, double, double> crossProduct1;
	std::tuple<double, double, double> crossProduct2;
	//angle between cross products. determines whether the point is covered or not
	double angleBetweenCrPs = -1;
	//right value of plane equation
	double otherside = -1;

	//get the current test point using data members
	double curPoint[3] = { xCoordinates[testPoint], yCoordinates[testPoint], zCoordinates[testPoint] };
	//parameterize by determining vector from point to camera
	double curVectortoCamera[3] = { camera.getCamPointArray()[0] - curPoint[0], camera.getCamPointArray()[1] - curPoint[1], camera.getCamPointArray()[2] - curPoint[2] };
	//tvalue is plugged into vectors to determine intersection points
	double tvalue = -1;
	//array which stores the values of the intersection point
	double intPoint[3] = { -1 };

	//Calculate PQ, where P is para1[0] and Q is para1[1]
	PQ[0] = xCoordinates[getPointNum(parallelogram[1])] - xCoordinates[getPointNum(parallelogram[0])];
	PQ[1] = yCoordinates[getPointNum(parallelogram[1])] - yCoordinates[getPointNum(parallelogram[0])];
	PQ[2] = zCoordinates[getPointNum(parallelogram[1])] - zCoordinates[getPointNum(parallelogram[0])];

	//Calculate PR, where P is para1[0] and R is para1[2]
	PR[0] = xCoordinates[getPointNum(parallelogram[2])] - xCoordinates[getPointNum(parallelogram[0])];
	PR[1] = yCoordinates[getPointNum(parallelogram[2])] - yCoordinates[getPointNum(parallelogram[0])];
	PR[2] = zCoordinates[getPointNum(parallelogram[2])] - zCoordinates[getPointNum(parallelogram[0])];


	crossProduct1 = std::make_tuple(PQ[1] * PR[2] - PQ[2] * PR[1], -1* (PQ[0] * PR[2] - PQ[2] * PR[0]), PQ[0] * PR[1] - PQ[1] * PR[0]);

	otherside = (std::get<0>(crossProduct1) * xCoordinates[getPointNum(parallelogram[0])] + std::get<1>(crossProduct1) * yCoordinates[getPointNum(parallelogram[0])] + std::get<2>(crossProduct1) * zCoordinates[getPointNum(parallelogram[0])]);

	tvalue = (otherside - std::get<0>(crossProduct1) * curPoint[0] - std::get<1>(crossProduct1) * curPoint[1] - std::get<2>(crossProduct1) * curPoint[2]) /
		(std::get<0>(crossProduct1) * curVectortoCamera[0] + std::get<1>(crossProduct1) * curVectortoCamera[1] + std::get<2>(crossProduct1) * curVectortoCamera[2]);

	intPoint[0] = curPoint[0] + curVectortoCamera[0] * tvalue;
	intPoint[1] = curPoint[1] + curVectortoCamera[1] * tvalue;
	intPoint[2] = curPoint[2] + curVectortoCamera[2] * tvalue;

	//find first cross product
	PQ[0] = intPoint[0] - xCoordinates[getPointNum(parallelogram[3])];
	PQ[1] = intPoint[1] - yCoordinates[getPointNum(parallelogram[3])];
	PQ[2] = intPoint[2] - zCoordinates[getPointNum(parallelogram[3])];

	PR[0] = xCoordinates[getPointNum(parallelogram[2])] - xCoordinates[getPointNum(parallelogram[3])];
	PR[1] = yCoordinates[getPointNum(parallelogram[2])] - yCoordinates[getPointNum(parallelogram[3])];
	PR[2] = zCoordinates[getPointNum(parallelogram[2])] - zCoordinates[getPointNum(parallelogram[3])];

	crossProduct1 = std::make_tuple(PQ[1] * PR[2] - PQ[2] * PR[1], -1 * (PQ[0] * PR[2] - PQ[2] * PR[0]), PQ[0] * PR[1] - PQ[1] * PR[0]);

	//find second cross product (there's an error in here)
	PQ[0] = intPoint[0] - xCoordinates[getPointNum(parallelogram[1])];
	PQ[1] = intPoint[1] - yCoordinates[getPointNum(parallelogram[1])];
	PQ[2] = intPoint[2] - zCoordinates[getPointNum(parallelogram[1])];

	PR[0] = xCoordinates[getPointNum(parallelogram[0])] - xCoordinates[getPointNum(parallelogram[1])];
	PR[1] = yCoordinates[getPointNum(parallelogram[0])] - yCoordinates[getPointNum(parallelogram[1])];
	PR[2] = zCoordinates[getPointNum(parallelogram[0])] - zCoordinates[getPointNum(parallelogram[1])];

	crossProduct2 = std::make_tuple(PQ[1] * PR[2] - PQ[2] * PR[1], -1 * (PQ[0] * PR[2] - PQ[2] * PR[0]), PQ[0] * PR[1] - PQ[1] * PR[0]);
	
	//find the angle between the cross product vectors and convert to degrees
	angleBetweenCrPs = (std::get<0>(crossProduct1) * std::get<0>(crossProduct2) + std::get<1>(crossProduct1) * std::get<1>(crossProduct2) + std::get<2>(crossProduct1) * std::get<2>(crossProduct2)) /
		(sqrt(std::get<0>(crossProduct1) * std::get<0>(crossProduct1) + std::get<1>(crossProduct1) * std::get<1>(crossProduct1) + std::get<2>(crossProduct1) * std::get<2>(crossProduct1)) *
			sqrt(std::get<0>(crossProduct2) * std::get<0>(crossProduct2) + std::get<1>(crossProduct2) * std::get<1>(crossProduct2) + std::get<2>(crossProduct2) * std::get<2>(crossProduct2)));

	if (angleBetweenCrPs < 1.0000001 && angleBetweenCrPs > .9999999) {
		angleBetweenCrPs = 1;
	}

	angleBetweenCrPs = acos(angleBetweenCrPs) * (180 / PI);
	
	//if the angle is 0, there is no intersection, return 0
	if (angleBetweenCrPs < 90 && angleBetweenCrPs > -90) {
		return 0;
	}

	//
	//repeat the same for the other set of parallel lines in the parallelogram
	//

	//find first cross product
	PQ[0] = intPoint[0] - xCoordinates[getPointNum(parallelogram[1])];
	PQ[1] = intPoint[1] - yCoordinates[getPointNum(parallelogram[1])];
	PQ[2] = intPoint[2] - zCoordinates[getPointNum(parallelogram[1])];

	PR[0] = xCoordinates[getPointNum(parallelogram[3])] - xCoordinates[getPointNum(parallelogram[1])];
	PR[1] = yCoordinates[getPointNum(parallelogram[3])] - yCoordinates[getPointNum(parallelogram[1])];
	PR[2] = zCoordinates[getPointNum(parallelogram[3])] - zCoordinates[getPointNum(parallelogram[1])];

	crossProduct1 = std::make_tuple(PQ[1] * PR[2] - PQ[2] * PR[1], -1 * (PQ[0] * PR[2] - PQ[2] * PR[0]), PQ[0] * PR[1] - PQ[1] * PR[0]);

	//find second cross product
	PQ[0] = intPoint[0] - xCoordinates[getPointNum(parallelogram[0])];
	PQ[1] = intPoint[1] - yCoordinates[getPointNum(parallelogram[0])];
	PQ[2] = intPoint[2] - zCoordinates[getPointNum(parallelogram[0])];

	PR[0] = xCoordinates[getPointNum(parallelogram[2])] - xCoordinates[getPointNum(parallelogram[0])];
	PR[1] = yCoordinates[getPointNum(parallelogram[2])] - yCoordinates[getPointNum(parallelogram[0])];
	PR[2] = zCoordinates[getPointNum(parallelogram[2])] - zCoordinates[getPointNum(parallelogram[0])];

	crossProduct2 = std::make_tuple(PQ[1] * PR[2] - PQ[2] * PR[1], -1 * (PQ[0] * PR[2] - PQ[2] * PR[0]), PQ[0] * PR[1] - PQ[1] * PR[0]);

	angleBetweenCrPs = -1;

	//find the angle between the cross product vectors and convert to degrees
	angleBetweenCrPs = (std::get<0>(crossProduct1) * std::get<0>(crossProduct2) + std::get<1>(crossProduct1) * std::get<1>(crossProduct2) + std::get<2>(crossProduct1) * std::get<2>(crossProduct2)) /
		(sqrt(std::get<0>(crossProduct1) * std::get<0>(crossProduct1) + std::get<1>(crossProduct1) * std::get<1>(crossProduct1) + std::get<2>(crossProduct1) * std::get<2>(crossProduct1)) *
			sqrt(std::get<0>(crossProduct2) * std::get<0>(crossProduct2) + std::get<1>(crossProduct2) * std::get<1>(crossProduct2) + std::get<2>(crossProduct2) * std::get<2>(crossProduct2)));

	if (angleBetweenCrPs < 1.0000001 && angleBetweenCrPs > .9999999) {
		angleBetweenCrPs = 1;
	}

	angleBetweenCrPs = acos(angleBetweenCrPs) * (180 / PI);

	if (angleBetweenCrPs < 90 && angleBetweenCrPs > -90) {
		return 0;
	}



	return 1;
}

void Cube::scale2DValues(double maxZValue, double minZValue) {
	double centerToTop = maxZValue - getCenterOfCubeTwoD()[1];
	double totalZLength = maxZValue - minZValue;
	//centerToTop tells us the proportion of length from center to maxZValue
	centerToTop = centerToTop / totalZLength;
	//the display is 40 height by 80 length, sizeChange is the 
	double sizeChange = 40 / totalZLength;

	//set the new center of cube
	double tempZ = getCenterOfCubeTwoD()[1];
	setCenter(20, 40 - (40 * centerToTop));

	//scale all 2D values of the cube
	for (int i = 0; i < 8; i++) {
		if (getXCoordinates2D()[i] != DBL_MAX && getZCoordinates2D()[i] != DBL_MAX) {
			setCoordinatesTwoDX(i, getXCoordinates2D()[i] * sizeChange);
			setCoordinatesTwoDZ(i, getZCoordinates2D()[i] * sizeChange);
		}
	}

	tempZ = tempZ * sizeChange;
	double offset[2] = { 0 - getCenterOfCubeTwoD()[0], tempZ - getCenterOfCubeTwoD()[1] };

	//move all 2D values of the cube
	for (int i = 0; i < 8; i++) {
		if (getXCoordinates2D()[i] != DBL_MAX) {
			setCoordinatesTwoDX(i, getXCoordinates2D()[i] - offset[0]);
			setCoordinatesTwoDZ(i, getZCoordinates2D()[i] - offset[1]);
		}
	}
}
#include "Display.h"

Display::Display() {
	// The default values for the camera are 0, -30, 25
	// for the purpose of this project, but it can be changed
	camPoint[0] = 0;
	camPoint[1] = -40;
	camPoint[2] = 25;
	// Default values for directionVector
	directionVector[0] = 0;
	directionVector[1] = 0;
	directionVector[2] = 0;
	//set tValue to 0
	tValue = 0;
	// Default planeConstant is 20 (plane is at y = -20)
	planeConstant = 25;
	// screen values are ' ' by default
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 40; j++) {
			screen[i][j] = ' ';
		}
	}
}

void Display::setTValue(double newTVal) {
	this->tValue = newTVal;
}

void Display::setCamPoints(double x, double y, double z) {
	camPoint[0] = x;
	camPoint[1] = y;
	camPoint[2] = z;
}

void Display::setPlaneConstant(double newPC) {
	this->planeConstant = newPC;
}

void Display::setDirectionVector(double x, double y, double z) {
	directionVector[0] = x;
	directionVector[1] = y;
	directionVector[2] = z;
}

double Display::getTValue() {
	return this->tValue;
}

double* Display::getCamPointArray() {
	return this->camPoint;
}

double Display::getPlaneConstant() {
	return this->planeConstant;
}

double* Display::getDirectionVector() {
	return this->directionVector;
}

//maps the 3D points to a 2D display
void Display::ThreeDToTwoD(Cube* changeCube) {
	double tempX = 0;
	double tempY = 0;
	double tempZ = 0;

	tempX = 0 - camPoint[0];
	tempY = 0 - camPoint[1];
	tempZ = 0 - camPoint[2];

	setDirectionVector(tempX, tempY, tempZ);

	tValue = -1 * (camPoint[1] + planeConstant) / tempY;

	changeCube->setCenter(camPoint, tValue, directionVector[0], directionVector[2]);


	for (int i = 0; i < 8; i++) {
		tempX = changeCube->getXCoordinates()[i] - camPoint[0];
		tempY = changeCube->getYCoordinates()[i] - camPoint[1];
		tempZ = changeCube->getZCoordinates()[i] - camPoint[2];

		setDirectionVector(tempX, tempY, tempZ);

		tValue = -1 * (camPoint[1] + planeConstant) / tempY;

		changeCube->setCoordinatesTwoDX(i, camPoint[0], tValue, directionVector[0]);
		changeCube->setCoordinatesTwoDZ(i, camPoint[2], tValue, directionVector[2]);
	}
}

void Display::displayCube(Cube cube) {
	char buf[1000] = "\r\n";

	double minXValue = 1000, minZValue = 1000, maxXValue = -1000, maxZValue = -1000;


	//find the maximum and minimum x and z values in a rotation
	for (int numRuns = 0; numRuns < 192; numRuns++) {

		if (cube.getRotationCalculation() >= 6.28316) {
			cube.setRotationCalculation(0);
		}

		cube.setCoordinates();
		ThreeDToTwoD(&cube);
		cube.findInvalidPoints(*this);

		//std::cout << newCube.getXCoordinates2D()[1];

		//get the smallest x and z value over the rotation using a bottom point
		if (minXValue > cube.getXCoordinates2D()[1] && cube.getXCoordinates2D()[1] != DBL_MAX) {
			minXValue = cube.getXCoordinates2D()[1];
		}
		if (minZValue > cube.getZCoordinates2D()[1] && cube.getZCoordinates2D()[1] != DBL_MAX) {
			minZValue = cube.getZCoordinates2D()[1];
		}
		//get the largest x and z value over the rotation using a top point
		if (maxXValue < cube.getXCoordinates2D()[0] && cube.getXCoordinates2D()[0] != DBL_MAX) {
			maxXValue = cube.getXCoordinates2D()[0];
		}
		if (maxZValue < cube.getZCoordinates2D()[0] && cube.getZCoordinates2D()[0] != DBL_MAX) {
			maxZValue = cube.getZCoordinates2D()[0];
		}

		cube.setRotationCalculation(cube.getRotationCalculation() + PI / 96);
	}


	for (int numRuns = 0; numRuns < 5000; numRuns++) {
		for (int i = 0; i < 1000; i++) {
			//buf[1000] = "\r\n";
			buf[i] = ' ';
		}
		for (int i = 0; i < 20; i++) {
			buf[i * 40 + 40 + i] = '\n';
		}
		buf[800 + 20] = '\r';
		buf[801 + 20] = '\n';

		//double rotationVal = -1;
		//cube.setRotationCalculation(PI / 4);

		if (cube.getRotationCalculation() >= 6.28316) {
			cube.setRotationCalculation(0);
		}

		cube.setCoordinates();
		ThreeDToTwoD(&cube);
		cube.findInvalidPoints(*this);


		//the allParallelograms are made so that indexes 0 and 2 are opposites to each other
		int allParallelograms[6][4] = { {0,1,3,2},{0,1,7,6},{4,5,3,2},{4,5,7,6},{0,2,4,6},{1,3,5,7} };

		cube.scale2DValues(maxZValue, minZValue);		

		//since the display is 40 by 40, and the array is 20 by 40 the midpoints are
		//1,3,5,...39 vertically and .5,1.5,2.5,...,39.5 horizontally

		//this loop makes it so that only valid parallelograms are in the allParallelograms array
		for (int i = 0; i < 6; i++) {
			bool isInvalidPoint = false;
			int invPointIter = 0;
			//for each invalid point, check for it in the array
			while (cube.getInvalidPoints()[invPointIter] != -1) {
				for (int j = 0; j < 4; j++) {
					if (allParallelograms[i][j] == cube.getInvalidPoints()[invPointIter]) {
						isInvalidPoint = true;
					}
				}
				invPointIter++;
			}
			//if the invalid point is contained in the array, disqualify whole sub-array
			if (isInvalidPoint == true) {
				for (int j = 0; j < 4; j++) {
					allParallelograms[i][j] = -1;
				}
			}
		}
		//using the allParallelograms array, make triangles that will be checked
		//there are a maximum of 3 points in a tri and 6 total tris, max
		std::pair<int, int> allTris[6][3];
		int curIndex = 0;
		for (int i = 0; i < 6; i++) {
			if (allParallelograms[i][0] != -1) {
				allTris[curIndex][0] = std::make_pair(allParallelograms[i][0], allParallelograms[i][1]);
				allTris[curIndex][1] = std::make_pair(allParallelograms[i][0], allParallelograms[i][2]);
				allTris[curIndex][2] = std::make_pair(allParallelograms[i][1], allParallelograms[i][2]);
				
				curIndex++;

				allTris[curIndex][0] = std::make_pair(allParallelograms[i][0], allParallelograms[i][2]);
				allTris[curIndex][1] = std::make_pair(allParallelograms[i][0], allParallelograms[i][3]);
				allTris[curIndex][2] = std::make_pair(allParallelograms[i][2], allParallelograms[i][3]);

				curIndex++;
			}
		}
		for (int i = curIndex; i < 6; i++) {
			allTris[curIndex][0] = std::make_pair(-1, -1);
			allTris[curIndex][1] = std::make_pair(-1, -1);
			allTris[curIndex][2] = std::make_pair(-1, -1);
		}

		//slope, z-intercept, beginning and end points of triEdges. The equations
		//will be 01, 02, and 12 in that order
		std::tuple<double, double, std::pair<double, double>> triEquations[6][3];

		for (int i = 0; i < 6; i++) {
			if (std::get<0>(allTris[i][0]) != -1) {
				for (int j = 0; j < 3; j++) {
					double bIntercept = -1;
					//std::get<1>(allTris[i][j]);
					double slope = (cube.getXCoordinates2D()[std::get<1>(allTris[i][j])] - cube.getXCoordinates2D()[std::get<0>(allTris[i][j])]) /
						(cube.getZCoordinates2D()[std::get<1>(allTris[i][j])] - cube.getZCoordinates2D()[std::get<0>(allTris[i][j])]);


					// x = mz + b
					//b = x-mz
					if (!std::isinf(slope)) {
						bIntercept = cube.getXCoordinates2D()[std::get<1>(allTris[i][j])] - slope * cube.getZCoordinates2D()[std::get<1>(allTris[i][j])];
					}
					else {
						bIntercept = cube.getXCoordinates2D()[std::get<0>(allTris[i][j])];
					}

					triEquations[i][j] = std::make_tuple(slope, bIntercept,
						std::make_pair(cube.getZCoordinates2D()[std::get<0>(allTris[i][j])], cube.getZCoordinates2D()[std::get<1>(allTris[i][j])]));
				}
			}
			else {
				for (int j = 0; j < 3; j++) {
					triEquations[i][j] = std::make_tuple(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
						std::make_pair(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
				}
			}
		}

		//this for loop loads characters into the array to make all of the cube's triangles
		for (int i = 0; i < 6; i++) {
			//if the current tri is valid, enter for loop. Otherwise, print nothing
			if (std::get<0>(allTris[i][0]) != -1) {
				//set the current triangle
				int edges[3][2] = {
					{std::get<0>(allTris[i][0]), std::get<1>(allTris[i][0])},
					{std::get<0>(allTris[i][1]), std::get<1>(allTris[i][1])},
					{std::get<0>(allTris[i][2]), std::get<1>(allTris[i][2])}
				};

				//find edge with most z range in the triangle
				double zRangeE1 = abs(cube.getZCoordinates2D()[edges[0][1]] - cube.getZCoordinates2D()[edges[0][0]]);
				double zRangeE2 = abs(cube.getZCoordinates2D()[edges[1][1]] - cube.getZCoordinates2D()[edges[1][0]]);
				double zRangeE3 = abs(cube.getZCoordinates2D()[edges[2][1]] - cube.getZCoordinates2D()[edges[2][0]]);

				int indexOfLargestRange = -1;
				if (zRangeE1 >= zRangeE2 && zRangeE1 >= zRangeE3) {
					//edge1 has largest range
					indexOfLargestRange = 0;
				}
				else if (zRangeE2 > zRangeE1 && zRangeE2 > zRangeE3) {
					//edge2 has largest range
					indexOfLargestRange = 1;
				}
				else {
					//edge3 has largest range
					indexOfLargestRange = 2;
				}
				//find the smaller and larger z values of LargestRange 
				double greaterPoint = -1, lesserpoint = -1;

				if (cube.getZCoordinates2D()[edges[indexOfLargestRange][1]] > cube.getZCoordinates2D()[edges[indexOfLargestRange][0]]) {
					greaterPoint = cube.getZCoordinates2D()[edges[indexOfLargestRange][1]];
					lesserpoint = cube.getZCoordinates2D()[edges[indexOfLargestRange][0]];
				}
				else {
					greaterPoint = cube.getZCoordinates2D()[edges[indexOfLargestRange][0]];
					lesserpoint = cube.getZCoordinates2D()[edges[indexOfLargestRange][1]];
				}

				//the third point of the triangle, the one not included in indexOfLargestRange
				int thirdPoint = -1;

				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 2; k++) {
						if (edges[j][k] != edges[indexOfLargestRange][0] && edges[j][k] != edges[indexOfLargestRange][1]) {
							thirdPoint = edges[j][k];
						}
					}
				}

				//find if the point is to the right or left of the longest range edge

				//0 is left, 1 is right
				int leftOrRightP1 = -1;
				int leftOrRightP2 = -1;

				//0 is left, 1 is right. it tells us if the point is to the left or right of the edge
				int leftOrRightGeneral = -1;

				if (cube.getXCoordinates2D()[edges[indexOfLargestRange][0]] <= cube.getXCoordinates2D()[thirdPoint]) {
					leftOrRightP1 = 1;
				}
				else {
					leftOrRightP1 = 0;
				}
				if (cube.getXCoordinates2D()[edges[indexOfLargestRange][1]] <= cube.getXCoordinates2D()[thirdPoint]) {
					leftOrRightP2 = 1;
				}
				else {
					leftOrRightP2 = 0;
				}

				if (leftOrRightP1 == 0 && leftOrRightP2 == 0) {
					leftOrRightGeneral = 0;
				}
				else if (leftOrRightP1 == 1 && leftOrRightP2 == 1) {
					leftOrRightGeneral = 1;
				}
				else {
					//find the value of the equation of the edge at the same z value as point
					//x = mz + b
					double xValue = std::get<0>(triEquations[i][indexOfLargestRange]) *
						cube.getZCoordinates2D()[thirdPoint] + std::get<1>(triEquations[i][indexOfLargestRange]);

					if (xValue <= cube.getXCoordinates2D()[thirdPoint]) {
						leftOrRightGeneral = 1;
					}
					else {
						leftOrRightGeneral = 0;
					}
				}
				//the algorithm will work from bottom up. Find the index of the bottom edge
				//that is connected to the longest z-range edge
				int indexOfSmallerEdge = -1;
				//edges[indexOfSmallerEdge][indexOfHigherPoint] is the stopping point of lower line
				int indexOfHigherPoint = -1;

				for (int j = 0; j < 3; j++) {
					if (indexOfLargestRange != j) {
						for (int k = 0; k < 2; k++) {
							if (cube.getZCoordinates2D()[edges[j][k]] == lesserpoint) {
								indexOfSmallerEdge = j;
								indexOfHigherPoint = (k + 1) % 2;
							}
						}
					}
				}

				//midpoints are 1, 3, 5, ..., 39

				char printChar = charType(edges);

				//this ensures the code doesn't break when there's a horizontal line in the triangle
				if (cube.getZCoordinates2D()[edges[indexOfSmallerEdge][0]] != cube.getZCoordinates2D()[edges[indexOfSmallerEdge][1]]) {
					//for each horizontal line in the triangle, load characters into the array
					for (int z = findNextOddIntUp(lesserpoint); z <= findNextOddIntDown(cube.getZCoordinates2D()[edges[indexOfSmallerEdge][indexOfHigherPoint]]); z += 2) {

						//x = mz + b
						double xValOfLargerEdge = std::get<0>(triEquations[i][indexOfLargestRange]) * z + std::get<1>(triEquations[i][indexOfLargestRange]);
						double xValOfSmallerEdge = std::get<0>(triEquations[i][indexOfSmallerEdge]) * z + std::get<1>(triEquations[i][indexOfSmallerEdge]);

						bool print = true;

						if (std::isinf(xValOfSmallerEdge) || std::isinf(xValOfLargerEdge)) {
							print = false;
						}

						if (abs(xValOfLargerEdge - xValOfSmallerEdge) >= .5 && print == true) {
							double x1 = findNextXMidpoint(xValOfLargerEdge);
							double x2 = findLastXMidpoint(xValOfSmallerEdge);
							bool keepRunning = true;
							//load array with characters in the current line
							while (keepRunning == true) {
								//generate the index of the array it will insert the char into
								int insertIndex = ((19 - detZIndex(z)) * 40) + (19 - detZIndex(z)) + detXIndex(x1);
								buf[insertIndex] = printChar;

								//if scanning to the right, add 1 to current x
								if (leftOrRightGeneral == 1) {
									x1 += 1;

									//simulates a for loop: if x1 > x2, stop running while loop. indicates when the
									//border of the triangle is reached
									if (x1 > x2) {
										keepRunning = false;
									}
								}
								//if scanning to the left, subtract 1 from current x
								else {
									x1 -= 1;

									//simulates a for loop: if x1 < x2, stop running while loop. indicates when the
									//border of the triangle is reached
									if (x1 < x2) {
										keepRunning = false;
									}
								}
							}
						}
					}
				}

				int indexOfSmallerEdge2 = -1;
				for (int j = 0; j < 3; j++) {
					if (j != indexOfLargestRange && j != indexOfSmallerEdge) {
						indexOfSmallerEdge2 = j;
						j = 3;
					}
				}
				//this ensures the code doesn't break when there's a horizontal line in the triangle
				if (cube.getZCoordinates2D()[edges[indexOfSmallerEdge2][0]] != cube.getZCoordinates2D()[edges[indexOfSmallerEdge2][1]]) {
					//for each horizontal line in the triangle, load characters into the array
					for (int z = findNextOddIntUp(cube.getZCoordinates2D()[edges[indexOfSmallerEdge][indexOfHigherPoint]]); z <= findNextOddIntDown(greaterPoint); z += 2) {

						//x = mz + b
						double xValOfLargerEdge = std::get<0>(triEquations[i][indexOfLargestRange]) * z + std::get<1>(triEquations[i][indexOfLargestRange]);
						double xValOfSmallerEdge = std::get<0>(triEquations[i][indexOfSmallerEdge2]) * z + std::get<1>(triEquations[i][indexOfSmallerEdge2]);

						bool print = true;
						if (std::isinf(xValOfSmallerEdge) || std::isinf(xValOfLargerEdge)) {
							print = false;
						}

						if (abs(xValOfLargerEdge - xValOfSmallerEdge) >= .5 && print == true) {
							double x1 = findNextXMidpoint(xValOfLargerEdge);
							double x2 = findLastXMidpoint(xValOfSmallerEdge);
							bool keepRunning = true;
							//load array with characters in the current line
							while (keepRunning == true) {
								//generate the index of the array it will insert the char into
								int insertIndex = ((19 - detZIndex(z)) * 40) + (19 - detZIndex(z)) + detXIndex(x1);
								buf[insertIndex] = printChar;

								//if scanning to the right, add 1 to current x
								if (leftOrRightGeneral == 1) {
									x1 += 1;

									//simulates a for loop: if x1 > x2, stop running while loop. indicates when the
									//border of the triangle is reached
									if (x1 > x2) {
										keepRunning = false;
									}
								}
								//if scanning to the left, subtract 1 from current x
								else {
									x1 -= 1;

									//simulates a for loop: if x1 < x2, stop running while loop. indicates when the
									//border of the triangle is reached
									if (x1 < x2) {
										keepRunning = false;
									}
								}
							}
						}
					}
				}
			}
		}

		std::system("clear");
		std::cout << std::endl;
		//write the array to the screen
		fwrite(buf, 1, sizeof(buf) - 1, stdout);

		cube.setRotationCalculation(cube.getRotationCalculation() + PI / 96);
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
	}
}

//finds the next odd integer in the positive direction
int Display::findNextOddIntUp(double number) {
	int nextNum = ceil(number);
	if (nextNum % 2 == 0) {
		nextNum++;
	}
	return nextNum;
}
//finds the next odd integer in the negative direction
int Display::findNextOddIntDown(double number) {
	int nextNum = floor(number);
	if (nextNum % 2 == 0) {
		nextNum--;
	}
	return nextNum;
}

//similar to findNextOddIntUp but it's in the x direction
double Display::findNextXMidpoint(double number) {
	//midpoints along x-axis are .5, 1.5, 2.5, ... 39.5

	int nextNum = ceil(number * 2);



	if (nextNum % 2 == 0) {
		return ((double)nextNum) / 2 + .5;
	}
	else {
		return ((double)nextNum) / 2;
	}
}

//similar to findNextOddIntUp but it's in the x direction
double Display::findLastXMidpoint(double number) {
	//midpoints along x-axis are .5, 1.5, 2.5, ... 39.5

	int nextNum = floor(number * 2);


	if (nextNum % 2 == 0) {
		return ((double)nextNum) / 2 - .5;
	}
	else {
		return ((double)nextNum) / 2;
	}
}

int Display::detXIndex(double number) {
	return (number - .5);
}

int Display::detZIndex(int number) {
	return (number - 1) / 2;
}

char Display::charType(int edges[3][2]) {
	int point1 = -1;
	int point2 = -1;
	int point3 = -1;

	point1 = edges[0][0];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			if (edges[i][j] != point1) {
				point2 = edges[i][j];
				i = 3;
				j = 2;
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			if (edges[i][j] != point1 && edges[i][j] != point2) {
				point3 = edges[i][j];
				i = 3;
				j = 2;
			}
		}
	}

	if ((point1 % 2) == 0 && (point2 % 2) == 0 && (point3 % 2) == 0) {
		return '*';
	}

	if (point1 % 2 == 1) {
		point1--;
	}
	if (point2 % 2 == 1) {
		point2--;
	}
	if (point3 % 2 == 1) {
		point3--;
	}

	point1 = point1 / 2;
	point2 = point2 / 2;
	point3 = point3 / 2;

	//find the two unique points
	int nonRepPoint1 = -1;
	int nonRepPoint2 = -1;

	nonRepPoint1 = point1;

	if (nonRepPoint1 == point2) {
		nonRepPoint2 = point3;
	}
	else {
		nonRepPoint2 = point2;
	}

	if ((nonRepPoint1 == 0 && nonRepPoint2 == 1) || (nonRepPoint1 == 1 && nonRepPoint2 == 0) ||
		(nonRepPoint1 == 2 && nonRepPoint2 == 3) || (nonRepPoint1 == 3 && nonRepPoint2 == 2)) {
		return 'l';
	}
	if ((nonRepPoint1 == 0 && nonRepPoint2 == 3) || (nonRepPoint1 == 3 && nonRepPoint2 == 1) ||
		(nonRepPoint1 == 1 && nonRepPoint2 == 2) || (nonRepPoint1 == 2 && nonRepPoint2 == 1)) {
		return '#';
	}

	return 'x';
}
#define _CRT_SECURE_NO_WARNINGS
#include "Display.h"

int main(void) {
	// z coordinate is always 10
	
	Cube newCube;
	Display newDisplay;

	/////////// TEMPORARY CODE
	//newCube.setRotationCalculation(PI / 4);
	newCube.setRotationCalculation(0);
	///////////
	
	/*begin = std::chrono::steady_clock::now();

	end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[ms]" << std::endl; */

	newDisplay.displayCube(newCube);

	std::cout << std::string(15, '0');
	std::cout << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	system("cls");


	std::cout << std::string(15, '1');
	std::cout << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	system("cls");

	return 0;
}
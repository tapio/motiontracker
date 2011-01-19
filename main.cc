#include <iostream>
#include "webcam.hh"

int main(int argc, char** argv)
{
	(void)argc; (void)argv; // Suppress warnings
	std::cout << "Webcam succesfully initialized: ";
	try {
		Webcam webcam;
		std::cout << "yes" << std::endl;
	} catch (std::exception& e) {
		std::cout << "no" << std::endl;
	}
	return 0;
}

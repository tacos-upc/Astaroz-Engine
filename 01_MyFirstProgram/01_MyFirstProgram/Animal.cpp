#include <iostream>
#include "Animal.h"

Animal::Animal(){
}


Animal::~Animal(){
}

void Animal::countLegs() {
	std::cout << "This animal has " << legs << " legs!";
}

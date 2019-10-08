
#include <iostream>
#include <string>
#include "Animal.h";


using namespace std;

int main() {
	std::cout << "Cout con STD!\n";									// --> 'std::cout' is mandatory to use COUT. We can also include the namespace STD to avoid doing that every time.
	cout << "Cout sin STD (usando el namespace)" << endl;			//for this to work, we need to include 'using namespace std;'.

	string a = "AVIRUUUUUUUUUUUU";									// --> to use strings, we need to include the library first.
	cout << a << endl;

	Animal dog;
	dog.legs = 4;
	dog.countLegs();

	string b;
	getline(cin, b);												// --> CIN is used to read the user's input in the next line.
	return 0;														// --> with this CIN structure, we will not need to pause or create a debugging point and we can break the pause with just 'ENTER'.
}
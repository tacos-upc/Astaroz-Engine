#include <iostream>

using namespace std;

int main() {

	int num1 = 10;
	int* p1 = &num1;

	cout << "num1 is: " << num1 << endl;		//la variable a secas, muestra el valor de la misma
	cout << "&num1 is: " << &num1 << endl;		//'&' en una variable, muestra la dirección de memoria que tiene
	cout << "p1 is: " << p1 << endl;			//el puntero a secas, muestra la dirección de memoria a la que apunta
	cout << "&p1 is: " << &p1 << endl;			//'&' en un puntero, apunta a la dirección de memoria del puntero
	cout << "*p1 is: " << *p1 << endl;			//'*' en un puntero, muestra el valor de la dirección de memoria a la que apunta

	cout << "*&num1 is: " << *&num1 << endl;

	num1 = *p1 + 2;
	cout << "num1 after suma is: " << num1 << endl;
	cout << "p1 is: " << p1 << endl;
	cout << "*p1 is: " << *p1 << endl;

	//arrays
	string texts[] = {"one", "two", "three"};
	cout << "sizeof(texts) is: " << sizeof(texts) << endl;
	cout << "sizeof(string) is: " << sizeof(string) << endl;
	cout << "sizeof(texts)/sizeof(string) is: " << sizeof(texts)/ sizeof(string) << endl;		//not sure why, but this division gives us the LENGTH of the array
	cout << "texts->length() is: " << texts->length();											//this also works for LENGTH


	system("pause");
	return 0;
}
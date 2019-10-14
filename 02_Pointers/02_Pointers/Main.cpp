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

	system("pause");
	return 0;
}
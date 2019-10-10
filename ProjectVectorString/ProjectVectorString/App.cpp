#include <iostream>
#include "Vec3.h"

using namespace std;

int main(){
	cout << endl << "VECTOR3 SECTION" << endl;
	Vec3<int> v1;
	v1.Display();

	Vec3<int> v2(1,2,3);
	v2.Display();

	Vec3<int> v3(v2);
	v3.Display();

	Vec3<int> v4 = v2.Sum(v3);
	v4.Display();

	v4.Normalize();
	v4.Display();
	v4.distance_to(v2);

	cout << endl << "STRING SECTION" << endl;

	system("pause");
	return 0;
}
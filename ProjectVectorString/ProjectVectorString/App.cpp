#include <iostream>
#include "Vec3.h"

using namespace std;

int main()
{
	Vec3 v1;
	v1.Display();
	Vec3 v2(1,2,3);
	v2.Display();
	//Vec3 v3(v2);

	system("pause");
	return 0;
}
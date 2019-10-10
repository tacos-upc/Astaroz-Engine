#pragma once
#include <iostream>

using namespace std;

class Vec3
{
public:
	float x;
	float y;
	float z;

	Vec3() {
		cout << "Called Vec3 constructor without params" << endl;
		x = 0;
		y = 0;
		z = 0;
	};
	Vec3(float x, float y, float z) {
		cout << "Called Vec3 constructor with 3 float params" << endl;
		this -> x = x;
		this -> y = y;
		this -> z = z;
	};
	void Display() {
		cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
	}
	void Normalize();
};

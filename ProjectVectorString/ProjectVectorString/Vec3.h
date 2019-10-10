#pragma once

#include <iostream>
#include <math.h>

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
	Vec3 Sum(Vec3 v) {
		Vec3 summation;
		summation.x = this->x + v.x;
		summation.y = this->y + v.y;
		summation.z = this->z + v.z;
		return summation;
	}
	void Normalize() {
		float mod = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
		if (mod < 0) {
			mod = mod * -1;
		}
		this->x = this->x / mod;
		this->y = this->y / mod;
		this->z = this->z / mod;
	};
	void distance_to(Vec3 v) {
		float distance = sqrt(pow(this->x - v.x, 2) + pow(this->y - v.y, 2) + pow(this->z - v.z, 2));
		cout << "The distance between the vectors is " << distance << endl;
	}
};

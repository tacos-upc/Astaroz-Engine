#pragma once
#include <string>

using namespace std;

class Object
{
public:
	Object();
	~Object();

	std::string id;

protected:

	void generateNewID();
};


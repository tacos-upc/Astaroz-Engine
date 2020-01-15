#ifndef __Object_H__
#define __Object_H__

#include <string>

class Object
{
public:
	Object();
	~Object();

	std::string id;

protected:
	void generateNewID();
};
#endif __Object_H__
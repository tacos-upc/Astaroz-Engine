#include "Object.h"

#include "Util/uuid/uuid.h"
#include "Util/uuid/uuid-io.h"

//using namespace stud; 
//using namespace std;
using stud::uuid;

Object::Object()
{
	generateNewID();
}

Object::~Object()
{
}

void Object::generateNewID()
{
	id = uuid::generate().string();
}
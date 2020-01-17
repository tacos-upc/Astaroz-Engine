#include "Component.h"

const char* Component::typeToString(int type)
{
	const char* name = "None";
	switch (type)
	{
		    case TRANSFORM:
				name = "Transform";
				break;
			case MESH:
				name = "Mesh";
				break;
			case MATERIAL:
				name = "Material";
				break;
			case CAMERA:
				name = "Camera";
				break;
			default:
				break;
	}
	return name;
}
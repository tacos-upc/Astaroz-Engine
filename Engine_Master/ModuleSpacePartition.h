#pragma once
#include "Module.h"
#include "Geometry/AABB.h"
#include "Math/MathAll.h"
#include "AABBTree.h"
#include "GameObject.h"

class ModuleSpacePartition : public Module
{
public:
	ModuleSpacePartition();
	~ModuleSpacePartition();

	bool Init() override;
	bool Start() override;

	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;

	bool CleanUp() override;
	void recalculateTree(GameObject*);

private:
	AABBTree* tree;
};


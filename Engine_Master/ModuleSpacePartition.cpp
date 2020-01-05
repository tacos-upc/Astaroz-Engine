#include "ModuleSpacePartition.h"

ModuleSpacePartition::~ModuleSpacePartition()
{
}

bool ModuleSpacePartition::Init()
{
	return true;
}

bool ModuleSpacePartition::Start()
{
	return true;
}

update_status ModuleSpacePartition::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleSpacePartition::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleSpacePartition::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleSpacePartition::CleanUp()
{
	return true;
}

#include <cassert>

#include "ECS/World/World_Fwd.h"

#include "Components.h"

int main(int argc, char* argv[])
{
	ECS::World w;
	ECS::Actor_t Actor = w.CreateActor();
	
	float Color1[3] = { 1, 2, 3 };
	float Brightness1 = 10.0f;
	PointLight* pl1 = w.AddComponent<PointLight>(Actor, Brightness1, Color1);
	assert(pl1 != nullptr);
	assert(pl1->Brightness == Brightness1);

	StaticMesh* pSMCube = w.AddComponent<StaticMesh>(Actor, "Cube.fbx");
	assert(pSMCube != nullptr);
	assert(pSMCube->Path == "Cube.fbx");

	StaticMesh* pSMPlane = w.AddComponent<StaticMesh>(Actor, "Plane.fbx");
	assert(pSMPlane != nullptr);
	assert(pSMPlane->Path == "Plane.fbx");

	ECS::ComponentUID_t SMPlaneId = pSMPlane->GetID();
	StaticMesh* pSMPlaneRefById = w.GetComponentById<StaticMesh>(Actor, SMPlaneId);
	assert(pSMPlaneRefById != nullptr);
	assert(pSMPlaneRefById->Path == "Plane.fbx");
	
	w.RemoveComponentById<StaticMesh>(Actor, SMPlaneId);
	StaticMesh* pSMPlaneRefById_REM = w.GetComponentById<StaticMesh>(Actor, SMPlaneId);
	assert(pSMPlaneRefById_REM == nullptr);

	return 0;
}

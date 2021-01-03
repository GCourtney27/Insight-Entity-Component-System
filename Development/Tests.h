#pragma once

#include "Components.h"
#include "ECS/ECS.h"

#include <cassert>


#define RUNTIME_ASSERT(expr) assert(expr);

namespace Debug
{

	int RunTests()
	{

		// Add components and remove them to a single actor.
		{
			ECS::World w;
			PointLight* pLight = 0;

			// Create an actor.
			ECS::Actor_t Actor = w.CreateActor();
			bool ValidActor = ECS::IsValidActor(Actor);
			RUNTIME_ASSERT(ValidActor);

			// Add a component to the actor.
			float Color1[3] = { 1, 2, 3 };
			float Brightness1 = 10.0f;
			PointLight* Light1 = w.AddComponent<PointLight>(Actor, Brightness1, Color1);
			assert(Light1->Brightness == Brightness1);

			// Get the component by its uid.
			pLight = w.GetComponentById<PointLight>(Light1->GetID());
			RUNTIME_ASSERT(pLight != nullptr);

			// Remove the component and verify it is destroyed.
			w.RemoveComponentById<PointLight>(Light1->GetID());
			pLight = w.GetComponentById<PointLight>(Light1->GetID());
			RUNTIME_ASSERT(pLight == nullptr);
		}

		// Destroy an actor.
		{
			ECS::World w;
			ECS::Actor_t Actor = w.CreateActor();

			// Add a light component to the actor.
			float Color1[3] = { 1, 2, 3 };
			float Brightness1 = 10.0f;
			PointLight* Light1 = w.AddComponent<PointLight>(Actor, Brightness1, Color1);
			ECS::ComponentUID_t LightID = Light1->GetID();
			assert(Light1->Brightness == Brightness1);

			// Add a mesh component to the actor.
			const char* c_Path = "Plane.fbx";
			StaticMesh* Mesh = w.AddComponent<StaticMesh>(Actor, c_Path);
			ECS::ComponentUID_t MeshID = Mesh->GetID();
			RUNTIME_ASSERT(Mesh->Path == c_Path);

			// Destroy the actor
			w.DestroyActor(Actor);

			// Verify the components no longer exist in the world.
			StaticMesh* pMesh = w.GetComponentById<StaticMesh>(MeshID);
			PointLight* pLight = w.GetComponentById<PointLight>(LightID);
			RUNTIME_ASSERT(!pMesh && !pLight);
		}

		// Add two actors with the same mesh type.
		// (Test if there are component clashes)
		{
			ECS::World w;
			ECS::Actor_t Actor1 = w.CreateActor();
			ECS::Actor_t Actor2 = w.CreateActor();

			const char* PlaneMesh = "Plane.fbx";
			StaticMesh* Mesh_Actor1 = w.AddComponent<StaticMesh>(Actor1, PlaneMesh);
			ECS::ComponentUID_t MeshID_Actor1 = Mesh_Actor1->GetID();

			const char* CubeMesh = "Cube.fbx";
			StaticMesh* Mesh_Actor2 = w.AddComponent<StaticMesh>(Actor2, CubeMesh);
			ECS::ComponentUID_t MeshID_Actor2 = Mesh_Actor2->GetID();

			StaticMesh* pMesh_Actor2Ref = w.GetComponentById<StaticMesh>(MeshID_Actor2);
			RUNTIME_ASSERT(pMesh_Actor2Ref->Path == CubeMesh);

			StaticMesh* pMesh_Actor1Ref = w.GetComponentById<StaticMesh>(MeshID_Actor1);
			RUNTIME_ASSERT(pMesh_Actor1Ref->Path == PlaneMesh);
		}

		printf("All Tests Passed!\n");
		return 0;
	}



}

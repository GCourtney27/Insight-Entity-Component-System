#pragma once

#include "ECS/ECS.h"
#include "Systems.h"
#include "Components.h"

#include "Timer.h"

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

		// Systems
		{
			ECS::World w;

		}

		printf("All Tests Passed!\n");
		return 0;
	}

	void RunPerfTests()
	{
		// Component iteration speed.
		// Loop over a lot of components and retrieve a value;
		{
			ECS::Actor_t DefaultActor;
			std::vector<PointLight*> LightPointers;
			ECS::GenericComponentMap<PointLight> LightMap;

			// Fill each container with instances.
			constexpr uint32_t InstCount = 10000;
			for (size_t i = 0; i < InstCount; ++i)
			{
				float Fac = float(i);
				float Color[3] = { Fac * 2, Fac * 4, Fac * 6 };
				float Brightness = Fac;

				LightPointers.push_back(new PointLight(Brightness, Color)); // Allocate on the heap. Immediate red flag.
				LightMap.AddComponent(DefaultActor, Brightness, Color);
			}

			// Test pointer version.
			Perf::ManualTimer PointerIterTimer("Pointer Iteration");
			{
				PointerIterTimer.Start();
				for (size_t i = 0; i < InstCount; ++i)
				{
					float Brightness = LightPointers[i]->Brightness;
				}
				PointerIterTimer.End();
			}

			// Test map version.
			Perf::ManualTimer MapIterTimer("Map Iteration");
			{
				MapIterTimer.Start();
				for (size_t i = 0; i < InstCount; ++i)
				{
					float Brightness = LightMap[uint32_t(i)].Brightness;
				}
				MapIterTimer.End();
			}

			// Make sure our optimized method is faster.
			RUNTIME_ASSERT(PointerIterTimer.GetElapsedNanos() > MapIterTimer.GetElapsedNanos());

			// Quick cleanup...
			for (size_t i = 0; i < InstCount; ++i)
				delete LightPointers[i];
		}

		printf("All performace tests passed!\n");
	}

}

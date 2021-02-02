#pragma once

#include "ECS/ECS.h"
#include "Systems.h"
#include "Components.h"

#include "Timer.h"



namespace Debug
{

	int RunTests()
	{
		// Add components and remove them for a single Entity.
		{
			ECS::EntityAdmin w;
			PointLight* pLight = 0;

			// Create an Entity.
			ECS::Entity_t Entity = w.CreateEntity();
			bool ValidEntity = ECS::IsValidEntity(Entity);
			RuntimeAssert(ValidEntity);

			// Add a component to the Entity.
			float Color1[3] = { 1, 2, 3 };
			float Brightness1 = 10.0f;
			PointLight* Light1 = w.AddComponent<PointLight>(Entity, Brightness1, Color1);
			assert(Light1->Brightness == Brightness1);

			// Get the component by its uid.
			pLight = w.GetComponentById<PointLight>(Light1->GetId());
			RuntimeAssert(pLight != nullptr);

			// Remove the component and verify it is destroyed.
			w.RemoveComponentById<PointLight>(Light1->GetId());
			pLight = w.GetComponentById<PointLight>(Light1->GetId());
			RuntimeAssert(pLight == nullptr);
		}

		// Destroy an Entity.
		{
			ECS::EntityAdmin w;
			ECS::Entity_t Entity = w.CreateEntity();

			// Add a light component to the Entity.
			float Color1[3] = { 1, 2, 3 };
			float Brightness1 = 10.0f;
			PointLight* Light1 = w.AddComponent<PointLight>(Entity, Brightness1, Color1);
			ECS::ComponentUID_t LightID = Light1->GetId();
			assert(Light1->Brightness == Brightness1);

			// Add a mesh component to the Entity.
			const char* c_Path = "Plane.fbx";
			StaticMesh* Mesh = w.AddComponent<StaticMesh>(Entity, c_Path);
			ECS::ComponentUID_t MeshID = Mesh->GetId();
			RuntimeAssert(Mesh->Path == c_Path);

			// Destroy the Entity
			w.DestroyEntity(Entity);

			// Verify the components no longer exist in the EntityAdmin.
			StaticMesh* pMesh = w.GetComponentById<StaticMesh>(MeshID);
			PointLight* pLight = w.GetComponentById<PointLight>(LightID);
			RuntimeAssert(!pMesh && !pLight);
		}

		// Add two Entitys with the same mesh type.
		// (Test if there are component clashes)
		{
			ECS::EntityAdmin w;
			ECS::Entity_t Entity1 = w.CreateEntity();
			ECS::Entity_t Entity2 = w.CreateEntity();

			const char* PlaneMesh = "Plane.fbx";
			StaticMesh* Mesh_Entity1 = w.AddComponent<StaticMesh>(Entity1, PlaneMesh);
			ECS::ComponentUID_t MeshID_Entity1 = Mesh_Entity1->GetId();

			const char* CubeMesh = "Cube.fbx";
			StaticMesh* Mesh_Entity2 = w.AddComponent<StaticMesh>(Entity2, CubeMesh);
			ECS::ComponentUID_t MeshID_Entity2 = Mesh_Entity2->GetId();

			StaticMesh* pMesh_Entity2Ref = w.GetComponentById<StaticMesh>(MeshID_Entity2);
			RuntimeAssert(pMesh_Entity2Ref->Path == CubeMesh);

			StaticMesh* pMesh_Entity1Ref = w.GetComponentById<StaticMesh>(MeshID_Entity1);
			RuntimeAssert(pMesh_Entity1Ref->Path == PlaneMesh);
		}

		// Remove Components
		{
			ECS::EntityAdmin w;
			ECS::Entity_t Entity = w.CreateEntity();

			float Color1[3] = { 1, 2, 3 };
			float Brightness1 = 1.0f;
			PointLight* pLight1 = w.AddComponent<PointLight>(Entity, Brightness1, Color1);

			float Color2[3] = { 4, 5, 6 };
			float Brightness2 = 2.0f;
			PointLight* pLight2 = w.AddComponent<PointLight>(Entity, Brightness2, Color2);
			ECS::ComponentUID_t Light2ID = pLight2->GetId();

			float Color3[3] = { 7, 8, 9 };
			float Brightness3 = 3.0f;
			PointLight* pLight3 = w.AddComponent<PointLight>(Entity, Brightness3, Color3);

			w.RemoveComponentById<PointLight>(Light2ID);

			RuntimeAssert(pLight3->Brightness == Brightness3);
		}

		DebugLog("[Test Suite] - All Tests Passed!\n");
		return 0;
	}

	void RunPerfTests()
	{
		// Component iteration speed.
		// Loop over a lot of components and retrieve a value;
		{
			ECS::Entity_t DefaultEntity;
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
				LightMap.AddComponent(DefaultEntity, Brightness, Color);
			}

			// Test pointer version.
			Perf::ManualTimer PointerIterTimer("Pointer Iteration");
			{
				PointerIterTimer.Start();
				for (size_t i = 0; i < InstCount; ++i)
				{
					float Brightness = LightPointers[i]->Brightness;
				}
				PointerIterTimer.Stop();
			}

			// Test map version.
			Perf::ManualTimer MapIterTimer("Map Iteration");
			{
				MapIterTimer.Start();
				for (size_t i = 0; i < InstCount; ++i)
				{
					float Brightness = LightMap[uint32_t(i)].Brightness;
				}
				MapIterTimer.Stop();
			}

			// Make sure our optimized method is faster.
			RuntimeAssert(PointerIterTimer.GetElapsedNanos() > MapIterTimer.GetElapsedNanos());

			// Quick cleanup...
			for (size_t i = 0; i < InstCount; ++i)
				delete LightPointers[i];
		}



		// Geometry Systems
		{
			ECS::EntityAdmin w;
			ECS::Entity_t Entity1 = w.CreateEntity();

			constexpr uint32_t InstCount = 10000;
			char Path[64];
			for (size_t i = 0; i < InstCount; ++i)
			{
				sprintf_s(Path, "Path: %zi", i);
				w.AddComponent<StaticMesh>(Entity1, Path);
			}

			GeometryProcessor GeomProcess(w);
			Perf::ManualTimer Timer("GeometryProcessor::Execute");
			Timer.Start();
			GeomProcess.Execute();
			Timer.Stop();
		}		

		// Light System
		{
			ECS::EntityAdmin w;
			ECS::Entity_t DefaultEntity = w.CreateEntity();

			constexpr uint32_t InstCount = 10000;
			for (size_t i = 0; i < InstCount; ++i)
			{
				float Fac = float(i);
				float Color[3] = { Fac * 2, Fac * 4, Fac * 6 };
				float Brightness = Fac;

				w.AddComponent<PointLight>(DefaultEntity, Brightness, Color);
			}

			LightProcessor LightProcessor(w);
			Perf::ManualTimer Timer("LightProcessor::Execute");
			Timer.Start();
			LightProcessor.Execute();
			Timer.Stop();
		}


		DebugLog("[Test Suite] - All performace tests passed!\n");
	}

}

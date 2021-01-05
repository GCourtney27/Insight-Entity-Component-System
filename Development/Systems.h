#pragma once

#include "ECS/World/System_Fwd.h"
#include "Components.h"
#include "Timer.h"


class GeometryProcessor : ECS::GenericSystem<StaticMesh>
{
public:
	GeometryProcessor(ECS::World& World) 
	: GenericSystem<StaticMesh>(World, "GeometryProcessor")
	{

	}
	virtual ~GeometryProcessor() {}


	virtual void Execute() override
	{
		/*size_t NumComponents = m_pComponentMapRef->GetNumComponents();
		for (uint32_t i = 0; i < NumComponents; ++i)
		{
			(*m_pComponentMapRef)[i].Path;
		}*/
		auto& Vec = GetRawComponentData();
		for (auto& Comp : Vec)
		{
			std::string& Path = Comp.Path;
		}
	}
};

class LightProcessor : ECS::GenericSystem<PointLight>
{
public:
	LightProcessor(ECS::World& World)
		: GenericSystem<PointLight>(World, "LightProcessor")
	{

	}
	virtual ~LightProcessor() {}


	virtual void Execute() override
	{
		// All stats captured on set iteration with 10,000 elements.
		//
		// For the best performance it is recomended iteration be done with rage based for loop as shown below.
		auto& Vec = GetRawComponentData();
		for (auto& Comp : Vec) 
		{
			float Brightness = Comp.Brightness; // 25,800 ns
		}

		// Other ways of accessing data, but not recomended as they provide vastly degraded performance.
		//
		//auto& Data = (*m_pComponentMapRef);
		//size_t NumComponents = Vec.size();
		//for (uint32_t i = 0; i < NumComponents; ++i)
		//{
		//	float Brightness = Vec[i].Brightness; // ~199,300 ns
		//	//float Brightness = Data.m_RawComponents[i].Brightness; // 196,100 ns
		//	//float Brightness = m_pComponentMapRef->m_RawComponents[i].Brightness; // 229,400 ns
		//	//float Brightness = GetRawMap()[i].Brightness; // 446,400 ns
		//	//float Brightness = (*m_pComponentMapRef)[i].Brightness; // 308,300 ns
		//	//float Brightness = Data[i].Brightness; // 320,300 ns
		//}
	}
};

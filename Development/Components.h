#pragma once

#include "ECS/Component/Component_Util.h"


struct PointLight : public ECS::ComponentBase<PointLight>
{
	PointLight() = default;
	virtual ~PointLight() = default;

	// TODO: Make sure the copy contructor isnt required for each component.
	PointLight operator=(const PointLight& rhs)
	{
		PointLight Result = {};
		::memcpy(&Result, static_cast<PointLight*>(this), sizeof(PointLight));
		return Result;
	}

	PointLight(const PointLight&& rhs) noexcept
	{
		Brightness = rhs.Brightness;
		constexpr unsigned int ColorSize = sizeof(Color) / sizeof(float);
		for (uint8_t i = 0; i < ColorSize; ++i)
			Color[i] = rhs.Color[i];
	}

	PointLight(const PointLight& rhs)
	{
		printf("WARNING: Copying component\n");
		Brightness = rhs.Brightness;
		constexpr unsigned int ColorSize = sizeof(Color) / sizeof(float);
		for (uint8_t i = 0; i < ColorSize; ++i)
			Color[i] = rhs.Color[i];
	}
	
	PointLight(float _Brightness, float _Color[3])
	{
		Brightness = _Brightness;
		constexpr unsigned int ColorSize = sizeof(Color) / sizeof(float);
		for(uint8_t i = 0; i < ColorSize; ++i)
			Color[i] = _Color[i];
	}

	float Color[3] = {};
	float Brightness = 0;
};

struct StaticMesh : public ECS::ComponentBase<StaticMesh>
{
	StaticMesh(const std::string& FilePath)
	{
		Path = FilePath;

		for (uint16_t i = 0; i < c_NumVerts; ++i)
			Vertices[i] = i* 3;
		
		for (uint16_t i = 0; i < c_NumIndices; ++i)
			Indices[i] = i;
	}
	virtual ~StaticMesh() = default;

	// TODO: Make sure the copy contructor isnt required for each component.
	StaticMesh operator=(const StaticMesh & rhs)
	{
		StaticMesh Result(rhs.Path);
		::memcpy(&Result.Vertices, static_cast<StaticMesh*>(this)->Vertices, sizeof(int) * c_NumVerts);
		::memcpy(&Result.Indices, static_cast<StaticMesh*>(this)->Indices, sizeof(int) * c_NumIndices);

		return Result;
	}

	std::string Path;
	
	static const int c_NumVerts = 20;
	static const int c_NumIndices = 5;
	int Vertices[c_NumVerts];
	int Indices[c_NumIndices];

};

struct TestComponent : public ECS::ComponentBase<TestComponent>
{
	TestComponent() = default;
	virtual ~TestComponent() = default;
	TestComponent(const TestComponent& rhs)
	{
		printf("WARNING: Copying component\n");
	}
	
	TestComponent(int _MyValue)
	{
		MyValue = _MyValue;
	}

	int MyValue = 0;
};

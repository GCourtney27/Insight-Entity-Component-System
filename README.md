# Insight-Entity-Component-System

A simple header only entity component system (ECS) written in C++17. Currently in use by the Insight Engine. <br/>
Designed to be cache friendly, easily consumable, and expandable using templates.

## Requirements: <br/>
Visual Studio 2019 <br/>
x64 compatible CPU <br/>

## Example Usage
ECS::World World; <br/>
Actor_t Actor = World.CreateActor(); <br/>
StaticMeshComponent* pMesh = World.AddComponent<StaticMeshComponent>(Actor, {}, {}); <br/>
ScriptComponent* pScript = World.AddComponent<ScriptComponent>(Actor, "Path/To/File.hi"); <br/>
GeometryProcessingSystem GeometrySystem(World); <br/>
  
while(AppIsRuning) <br/>
{ <br/>
  GeometrySystem.Execute(); <br/>
} <br/>

World.RemoveComponentById<StatiMeshComponent(pMesh->GetId()); <br/>
World.DestroyActor(Actor); <br/>

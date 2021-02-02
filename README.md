# Insight-Entity-Component-System

A simple header only entity component system (ECS) written in C++17. Currently in use by the Insight Engine. <br/>
Designed to be cache friendly, easily consumable, and expandable using templates.

## Requirements: <br/>
Visual Studio 2019 <br/>
x64 compatible CPU <br/>

## Example Usage
ECS::EntityAdmin EntityAdmin; <br/>
Entity_t Entity = EntityAdmin.CreateEntity(); <br/>
StaticMeshComponent* pMesh = EntityAdmin.AddComponent<StaticMeshComponent>(Entity, {}, {}); <br/>
ScriptComponent* pScript = EntityAdmin.AddComponent<ScriptComponent>(Entity, "Path/To/File.hi"); <br/>
GeometryProcessingSystem GeometrySystem(EntityAdmin); <br/>
  
while(AppIsRuning) <br/>
{ <br/>
  GeometrySystem.Execute(); <br/>
} <br/>

EntityAdmin.RemoveComponentById<StaticMeshComponent\(pMesh->GetId()); <br/>
EntityAdmin.DestroyEntity(Entity); <br/>

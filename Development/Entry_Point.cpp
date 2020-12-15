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

	float Color2[3] = { 4, 5, 6 };
	float Brightness2 = 20.0f;
	PointLight* pl2 = w.AddComponent<PointLight>(Actor, Brightness2, Color2);
	assert(pl2 != nullptr);

	ECS::ComponentUID_t Pl2ID = pl2->GetID();
	PointLight* Pl = w.GetComponentById<PointLight>(Actor, Pl2ID);
	assert(Pl->Brightness == pl2->Brightness);

	w.RemoveComponentById<PointLight>(Actor, Pl2ID);
	PointLight* PlNULL = w.GetComponentById<PointLight>(Actor, Pl2ID);
	assert(PlNULL == nullptr);


	/*PointLight* Pl = w.GetComponent<PointLight>(Actor);
	assert(Pl->Brightness == Brightness2);

	w.RemoveComponent<PointLight>(Actor);
	PointLight* comp = w.GetComponent<PointLight>(Actor);
	assert(comp == nullptr);

	TestComponent* TC = w.GetComponent<TestComponent>(Actor);
	assert(TC == nullptr);

	w.DestroyActor(Actor);
	PointLight* c = w.GetComponent<PointLight>(Actor);
	assert(c == nullptr);*/

	
	return 0;
}

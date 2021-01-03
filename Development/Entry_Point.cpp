#include "Tests.h"

typedef uint64_t QWORD;

typedef uint32_t Actor;
typedef uint32_t Index;

struct Pack
{
		uint32_t i;
		uint32_t j;
};

Actor ActorFromQWORD(const QWORD& Pack)
{
	Actor Result;
	uint64_t Mask = 0x00000000FFFFFFFF;
	Result = Actor(Pack | Mask);

	return Result;
}

int main(int argc, char* argv[])
{
	Debug::RunTests();
	Debug::RunPerfTests();

	return 0;
}


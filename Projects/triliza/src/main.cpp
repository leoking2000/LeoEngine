#include "triliza.h"
#include <LEO/EntryPoint.h>

LEO::ApplicationParameters GetUserApplicationParameters(int argc, char* argv[])
{
	LEO::ApplicationParameters p;
	p.createScene = []() { return std::make_unique<Triliza>(); };
	return p;
}

#include "triliza.h"
#include <LEO/EntryPoint.h>

LEO::ApplicationParameters GetUserApplicationParameters(int argc, char* argv[])
{
	LEO::ApplicationParameters p;
	p.startScene = std::make_shared<Triliza>();
	return p;
}

#include "Game.h"
#include <LEO/EntryPoint.h>

LEO::ApplicationParameters GetUserApplicationParameters(int argc, char* argv[])
{
	LOGINFO("R_path={} | Prod_build={}", RESOURCES_PATH, PRODUCTION_BUILD);

	LEO::ApplicationParameters p;
	p.width = 900;
	p.height = 600;
	p.isResizable = true;
	p.vsync = true;
	p.createScene = []() { return std::make_unique<Game>(); };

	return p;
}

#include "TestScene.h"
#include <LEO/EntryPoint.h>

LEO::ApplicationParameters GetUserApplicationParameters(int argc, char* argv[])
{
	LEO::ApplicationParameters p;
	p.isResizable = true;
	p.background_color = glm::vec3(0.0f);//glm::vec3(0.52f, 0.81f, 0.93f);

	p.startScene = std::make_unique<TestScene>();
	p.logLevel = LEO::LogLevel::VERBOSE;

	return p;
}


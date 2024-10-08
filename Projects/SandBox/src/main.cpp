#include "TestScene.h"
#define CLIENT_ENTRY_POINT
#include <LEO/EntryPoint.h>

LEO::ApplicationParameters GetUserApplicationParameters(int argc, char* argv[])
{
	LEO::ApplicationParameters p;
	p.isResizable = true;
	p.background_color = glm::vec3(0.0f);//glm::vec3(0.52f, 0.81f, 0.93f);

	p.createScene = []() { return std::make_unique<TestScene>(); };
	p.logLevel = LEO::LogLevel::VERBOSE;

	return p;
}

int LEO::Main(int argc, char* argv[])
{


	return 0;
}
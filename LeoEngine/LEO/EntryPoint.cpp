#include "EntryPoint.h"

namespace LEO
{
	int Main(int argc, char* argv[])
	{
		LEO::EngineInitialization();

		{
			ApplicationParameters p = GetUserApplicationParameters(argc, argv);
			Application app(p);
			app.Run();
		}

		LEO::EngineTerminate();
		return 0;
	}
}

// TODO: Have option to not have a console(have a WIN MAIN)
int main(int argc, char* argv[])
{
	return LEO::Main(argc, argv);
}

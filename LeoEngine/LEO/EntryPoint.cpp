#include "EntryPoint.h"

#ifndef CLIENT_ENTRY_POINT
namespace LEO
{
	int Main(int argc, char* argv[])
	{
		LEO::EngineInitialization();

		{
			ApplicationParameters p = GetUserApplicationParameters(argc, argv);
			Application app(p);
			app.Run(p.createScene);
		}

		LEO::EngineTerminate();
		return 0;
	}
}
#endif

// TODO: Have option to not have a console(have a WIN MAIN)
int main(int argc, char* argv[])
{
	return LEO::Main(argc, argv);
}

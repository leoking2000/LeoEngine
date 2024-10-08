#pragma once
#include "Application/Application.h"

// implemented by the client
extern LEO::ApplicationParameters GetUserApplicationParameters(int argc, char* argv[]);

namespace LEO
{
#ifdef CLIENT_ENTRY_POINT
	extern int Main(int argc, char* argv[]);
#else
	int Main(int argc, char* argv[]);
#endif // CLIENT_ENTRY_POINT
}

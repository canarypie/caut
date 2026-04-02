#include "renderer.h"
#include "arguments.h"
#include "utility.h"
#include "grid.h"
#include "system.h"

int main(int argc, char** argv)
{
	auto config = Config::handle_arguments(argc, argv);

	GOLSystem system(config);
	system.run();

	return 0;
}

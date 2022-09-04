//
// Print a greeting message on standard output and exit.
//
// On embedded platforms this might require semi-hosting or similar.
//
// For example, for toolchains derived from GNU Tools for Embedded,
// to enable semi-hosting, the following was added to the linker:
//
// --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group
// -T../debug-in-microsemi-smartfusion2-envm.ld
// -T../debug-in-microsemi-smartfusion2-envm.ld
// Adjust it for other toolchains.
// use define (__DEBUG_CONSOLE__)

#include "config.h"


int main()
{
	int nRet = 0;

	init_system();

	post_update_ready();

	while(1)
	{
		nRet = processFwUpdate();
	}

}


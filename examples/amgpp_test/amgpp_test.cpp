#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <AMG/AMGLib.h>
#include <AMG/AMGPPLib.h>

AMG_MODULE_INFO("AMG_Template", AMG_USERMODE, 1, 1);

using namespace amg;

int main(int argc, char **argv)
{

	AMG_SetupCallbacks();
	
	pspDebugScreenInit();
	
	pspDebugScreenPrintf("\n HelloWorld!!\n\n");
	pspDebugScreenPrintf(" Press HOME to exit");
	
	struct timer time;
	while(!AMG.Exit)
	{
		pspDebugScreenPrintf("%f\n", time.update());
		sceDisplayWaitVblankStart();
	}
	exit(AMG_ReturnXMB());
}

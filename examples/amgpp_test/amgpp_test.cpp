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
	mat::f32<4,4> A = { {0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15} };
	AMG_SetupCallbacks();
	
	pspDebugScreenInit();
	
	pspDebugScreenPrintf("\n HelloWorld!!\n\n");
	pspDebugScreenPrintf(" Press HOME to exit");
	
	struct timer time;
	while(!AMG.Exit)
	{
		for(size_t i = 0; i < 4; i++)
			pspDebugScreenPrintf("%f %f %f %f\n", A[i][0], A[i][1], A[i][2], A[i][3]);
		sceDisplayWaitVblankStart();
	}
	exit(AMG_ReturnXMB());
}

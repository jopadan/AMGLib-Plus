/*
 * AMGlib Multitexturing
 * Powered by Andresmargar
 * from SCENEBETA
 */

#include <AMG/AMGLib.h>

AMG_MODULE_INFO("AMG_Multitexturing", 0, 1, 1);

float Angle1 = 0;
float SCALING = 1;
float SCALING1 = 1;
int Ob = 0;
float ALT = -15.0;
float CAM = 28;
char text[256];

int main(int argc, char **argv){

	// Init PSP
	AMG_SetupCallbacks();
	AMG_Init3D(GU_PSM_5650 | AMG_DOUBLEBUFFER);
	AMG_InitTimeSystem();

	// Load a text font
	AMG_Texture *font = AMG_LoadTexture("assets/font.png", AMG_TEX_RAM);
	AMG_DeleteColor(font, 0xFF000000);
	AMG_Create2dObject(font, GU_PSM_5551, 1);
	AMG_SwizzleTexture(font);

	AMG_Model *Planet = AMG_LoadModel("assets/worms3d_planet.obj");
	AMG_Texture *Brillo = AMG_LoadTexture("assets/brillo.png", AMG_TEX_VRAM);
	AMG_SetTextureMapping(Brillo, GU_ENVIRONMENT_MAP, 1, 2);

	// Set second texture on top of the default to sub-objects
	AMG_SetObjectMultiTexture(&Planet->Object[2].Group[0], Brillo);
	AMG_SetObjectMultiTexture(&Planet->Object[11].Group[0], Brillo);
	AMG_SetObjectMultiTexture(&Planet->Object[16].Group[0], Brillo);
	AMG_SetObjectMultiTexture(&Planet->Object[20].Group[0], Brillo);
	AMG_SetObjectMultiTexture(&Planet->Object[23].Group[0], Brillo);
	AMG_SetObjectMultiTexture(&Planet->Object[25].Group[0], Brillo);
	AMG_SetObjectMultiTexture(&Planet->Object[26].Group[0], Brillo);
	AMG_SetObjectMultiTexture(&Planet->Object[31].Group[0], Brillo);

	// Background sphere
	AMG_Model *Space = AMG_LoadModel("assets/bkg.obj");
	// Disable Light
	Space->Object[0].Lighting = 0;

	/** CONFIGURATE LIGHT 0 **/

	// Tipo de luz	// Light type
	AMG_Light[0].Type = GU_DIRECTIONAL;
	// Light parameters
	AMG_Light[0].Diffuse = 0xFFFFFF;
	AMG_Light[0].Ambient = 0x777777;
	// Light position (direction vector)
	AMG_Light[0].Pos.x = 4.0f;
	AMG_Light[0].Pos.y = 4.0f;
	AMG_Light[0].Pos.z = 4.0f;
	// Set a camera
	AMG_Camera *camera = AMG_InitCamera();
	camera->Pos.z = 35.0f;
	camera->Pos.y = 0.0f;
	camera->Pos.x = 0.0f;
	camera->Rot.x = AMG_Deg2Rad(0.0f);

	// Fix camera projection
	AMG_SetPerspectiveFOV(36.0f);

	while(!AMG.Exit){
		AMG_Begin3D();
		AMG_ReadButtons();

		// Calculate rotation and scale
		SCALING += AMG_Sin(Angle1) * 0.004f;
		SCALING1 += AMG_Cos(Angle1) * 0.008f;
		Angle1 += 0.06f;

		// Scale rotate Planet
		for (Ob = 0; Ob < Planet->NObjects; Ob++){
			Planet->Object[Ob].Pos.y = 0.0;
			Planet->Object[Ob].Rot.y += 0.02;
			//Planet->Object[Ob].Scale.x = SCALING;
			Planet->Object[Ob].Scale.y = SCALING1;
		}

		// Rotate Skybox
		Space->Object[0].Rot.y += 0.006;

		// Set camera
		AMG_SetCamera(camera);

		AMG_EnableLight(0);

		AMG_RenderModel(Planet);
		AMG_RenderModel(Space);

		AMG_DisableLight(0);

		// 2D mode
		AMG_OrthoMode(1);
		sprintf(text, "~ MultiTexturing example ~\nBy Mills [%d FPS]\nModels from Worms 3D", AMG.FPS);
		AMG_Printf(font, 0, 0, 0xFFFFFFFF, text);
		AMG_OrthoMode(0);

		// Screenshot
		if(AMG_Button.Down &PSP_CTRL_CIRCLE) AMG_Screenshot("screenshot.png");

		AMG_Update3D();
		AMG_UpdateTime();
	}

	AMG_UnloadModel(Planet);
	AMG_UnloadModel(Space);
	AMG_UnloadTexture(font);
	AMG_Finish3D();
	return AMG_ReturnXMB();
}

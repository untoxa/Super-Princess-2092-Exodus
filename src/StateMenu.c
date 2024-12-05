#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gbdk/platform.h>

#include "SGB.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Music.h"
#include "Palette.h"

IMPORT_MAP(pressstarttilemap);
IMPORT_BORDER(border);

DECLARE_MUSIC(exo_start);

UINT8 sgb_initialized = FALSE;

void START(void) {
	SetBorderColor(RGB(0, 0, 0));
	
	if (!sgb_initialized) {
		sgb_initialized = TRUE;
		scroll_x_vblank = scroll_y_vblank = 0; 
		scroll_offset_x = scroll_offset_y = 0;
		LOAD_SGB_BORDER(border); 
	}

	InitScroll(BANK(pressstarttilemap), &pressstarttilemap, 0, 0);

	PlayMusic(exo_start, 0);
}

extern UINT8 stage_completion;
void UPDATE(void) {
	if(KEY_TICKED(J_START)) {
		stage_completion = 0;
		SetState(StateStageSelect);
//		SetState(StateEnding);	// for debugging purposes
	}
}
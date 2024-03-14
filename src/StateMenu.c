#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gbdk/platform.h>
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Music.h"

IMPORT_MAP(pressstarttilemap);

DECLARE_MUSIC(exo_start);

void START(void) {
#ifdef SEGA
	__WRITE_VDP_REG(VDP_R2, R2_MAP_0x3800);
	__WRITE_VDP_REG(VDP_R5, R5_SAT_0x3F00);
#endif

	InitScroll(BANK(pressstarttilemap), &pressstarttilemap, 0, 0);

	PlayMusic(exo_start, 0);
}

extern UINT8 stage_completion;
void UPDATE(void) {
	if(KEY_TICKED(J_START)) {
		stage_completion = 0;
		SetState(StateStageSelect);
	}
}
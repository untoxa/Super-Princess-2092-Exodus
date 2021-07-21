#include "Banks/SetBank2.h"
#include "main.h"

#include <gb/gb.h>
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Music.h"

#include "../res/src/pressstarttilemap.h"

DECLARE_MUSIC(exo_start);

void Start_StateMenu() {
	InitScroll(&pressstarttilemap, 0, 0);
	SHOW_BKG;

	PlayMusic(exo_start, 0);
}

extern UINT8 stage_completion;
void Update_StateMenu() {
	if(KEY_TICKED(J_START)) {
		stage_completion = 0;
		SetState(StateStageSelect);
	}
}
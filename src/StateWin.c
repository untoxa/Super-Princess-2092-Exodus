#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gbdk/platform.h>
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Music.h"

IMPORT_MAP(endingtilemap);
DECLARE_MUSIC(exo_gameover);

void START(void) {
	InitScroll(BANK(endingtilemap), &endingtilemap, 0, 0);

	PlayMusic(exo_gameover, 0);
}

void UPDATE(void) {
	if(KEY_TICKED(J_START)) {
		SetState(StateMenu);
	}
}
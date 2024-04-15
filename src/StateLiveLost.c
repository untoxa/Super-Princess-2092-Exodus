#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gbdk/platform.h>
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Music.h"
#include "Print.h"

IMPORT_MAP(menuBG);
IMPORT_FONT(font);

DECLARE_MUSIC(exo_stage);
extern UINT8 n_lives;

void START(void) {
	InitScroll(BANK(menuBG), &menuBG, 0, 0);

	INIT_FONT_EX(font, PRINT_BKG);
	PRINT(4, 8, "%d LIFES LEFT", (UINT16)(n_lives));

	PlayMusic(exo_stage, 0);
}

void UPDATE(void) {
	if(KEY_TICKED(J_START) | KEY_TICKED(J_A) | KEY_TICKED(J_B)) {
		SetState(StateGame);
	}
}
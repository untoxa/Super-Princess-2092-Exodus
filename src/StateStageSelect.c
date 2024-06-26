#include "Banks/SetAutoBank.h"
#include "main.h"

IMPORT_MAP(stageSelect);
IMPORT_FONT(font);

#include "ZGBMain.h"
#include "Print.h"
#include "Scroll.h"
#include "Keys.h"
#include "Math.h"
#include "Music.h"
#include "Sound.h"

#include "Sounds.h"

extern UINT8 current_stage;
extern UINT8 current_level;
extern UINT8 n_lives;
extern UINT8 stage_completion;

DECLARE_MUSIC(levelselect);

void SetStage(UINT8 stage) {
	current_stage = stage;

	PRINT(6, 13, "STAGE %d", (UINT16)stage + 1);
	ExecuteSFX(BANK(FX_STAGE), FX_STAGE, SFX_MUTE_MASK(FX_STAGE), SFX_PRIORITY_NORMAL);
}

void START(void) {
	StopMusic;
	if(stage_completion == 7) {
		SetState(StateEnding);
	} else {
		InitScroll(BANK(stageSelect), &stageSelect, 0, 0);
	
		INIT_FONT_EX(font, PRINT_BKG);
		PRINT(4, 11, "STAGE SELECT");

		for(current_stage = 0; current_stage != 3; current_stage += 1) {
			if(GET_BIT(stage_completion, current_stage) == 0) {
				SetStage(current_stage);
				break;
			}
		}

		PlayMusic(levelselect, 1);
	}
}

void UPDATE(void) {
	UINT8 tmp;
	if(KEY_TICKED(J_LEFT)) {
		for(tmp = current_stage - 1; tmp != 255; -- tmp) {
			if(GET_BIT(stage_completion, tmp) == 0) {
				SetStage(tmp);
				break;
			}
		}
	}

	if(KEY_TICKED(J_RIGHT)) {
		for(tmp = current_stage + 1; tmp != 3; ++ tmp) {
			if(GET_BIT(stage_completion, tmp) == 0) {
				SetStage(tmp);
				break;
			}
		}
	}


	if(KEY_TICKED(J_START) | KEY_TICKED(J_A) | KEY_TICKED(J_B)) {
		current_level = 0;
		n_lives = 3;

		SetState(StateStageIntro);
	}
}

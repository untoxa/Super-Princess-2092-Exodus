#include "Banks/SetAutoBank.h"
#include "main.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Math.h"
#include "Print.h"
#include "Music.h"
#include "Vector.h"

IMPORT_MAP(stageEndingWindow);
IMPORT_MAP(stageEnding);
IMPORT_FONT(font);

fixed scroll_p_x;
extern UINT8 n_sprite_types;
extern UINT8 spriteIdxs[];

Sprite* end_sprite_princess;
Sprite* end_sprite;

const UINT8 end_anim_walk[] = VECTOR(3, 4, 5, 4);

INT8 end_princess_screen_x;

typedef enum {
	AWAITING_PRINCESS_CENTER,
	SHOWING_ENEMIES,
	CREDITS,
	DONE
} END_STATE;
UINT8 end_state;

struct EndSpriteInfo {
	SPRITE_TYPE type;
	const char* name;
	UINT8 anim_data[2];
};


#if defined(SEGA)
#define TEXT_Y_OFS 12
#define TEXT_X_OFS -scroll_offset_x
#else
#define TEXT_Y_OFS 0
#define TEXT_X_OFS 0
#endif

const struct EndSpriteInfo endSpritesInfo[] = {
	{ SpriteMushroom, "Mushroom",              {1, 1}},
	{ SpriteShooter,  "Shooter",               {1, 1}},
	{ SpriteCeilingShooter, "Ceiling Shooter", {1, 1}},
	{ SpriteFly,      "Fly",                   {1, 1}},
	{ SpriteRoller,   "Roller",                {1, 1}},
	{ SpriteOvni,     "OVNI",                  {1, 0}},
	{ SpriteMissile,  "Missile",               {1, 0}},
	{ SpritePlatform, "Platform",              {1, 0}},
	{ SpritePrincess, "Super Princess",        {1, 0}},
};

UINT8 enemy_idx;
UINT16 text_wait;
INT16 end_enemy_x;

DECLARE_MUSIC(exo_ending);

void PrepareEnemy(void) {
	struct EndSpriteInfo* info;

	info = (struct EndSpriteInfo*)&endSpritesInfo[enemy_idx];
	InitSprite(end_sprite, info->type);
	SetSpriteAnim(end_sprite, info->anim_data, 1);
	end_enemy_x = 160 + 16;
	end_sprite->x = scroll_x + end_enemy_x;
	end_sprite->y = scroll_y + 100u;
	end_sprite->mirror = NO_MIRROR;
}

const char* Credits[] = {
	"    PROGRAMMING     ",
	"       ZAL0         ",

	"      GRAPHICS      ",
	"      SERGEEO       ",

	"    LEVEL DESIGN    ",
	"      SERGEEO       ",

	"EXTRA ART AND LEVELS",
	"       ZAL0         ",

	"       MUSIC        ",
	"      SERGEEO       ",

	"   CROSS PLATFORM   ",
	"       TOXA         ",

	"  GAME GEAR COLORS  ",
	"    BADCOMPUTER     ",

	"  GAME GEAR MUSIC   ",
	"      CHIRINEA      ",

	"   SPECIAL THANKS   ",
	"     ANTONIOND      ",

	"     THANK YOU      ",
	"    FOR PLAYING!    ",
	NULL
};

void PrepareCredits(void) {
	text_wait = 300;
        wait_vbl_done();
	PRINT(TEXT_X_OFS, TEXT_Y_OFS + 0, Credits[enemy_idx ++]);
	PRINT(TEXT_X_OFS, TEXT_Y_OFS + 2, Credits[enemy_idx ++]);
}

void SetEndState(UINT8 state) {
	end_state = state;
	switch(end_state) {
		case AWAITING_PRINCESS_CENTER:
			break;

		case SHOWING_ENEMIES:
			enemy_idx = 0;
			text_wait = 0;
			end_sprite = sprite_manager_sprites[StackPop(sprite_manager_sprites_pool)];
			PrepareEnemy();
			break;

		case CREDITS:
			enemy_idx = 0;
			PrepareCredits();
			break;
	}
}

UINT8 STRLEN(const UINT8* str) {
	UINT8 i;
	for(i = 0; *str != '\0'; ++i, ++str) {
	}
	return i;
}

#if defined(SEGA)
void LCD_noscroll(void) NONBANKED {
	if (_shadow_OAM_OFF) return;
	VDP_CMD = 0; VDP_CMD = VDP_RSCX;
}
#endif

void START(void) {
	UINT8 i;

	#if defined(NINTENDO)
	CRITICAL {
		remove_LCD(LCD_isr);
	}
	#elif defined(SEGA)
	CRITICAL {
		add_LCD(LCD_noscroll);
	}
	__WRITE_VDP_REG(VDP_R10, ((DEVICE_SCREEN_Y_OFFSET + TEXT_Y_OFS) << 3) - 1);
	set_interrupts(VBL_IFLAG | LCD_IFLAG);
	#endif
	SHOW_SPRITES;

	scroll_target = 0;
	InitScroll(BANK(stageEnding), &stageEnding, 0, 0);
	for(i = 21; i < 32; ++i) {
		ScrollUpdateColumn(i, 0);
	}
	scroll_p_x.w = 0;

	#if defined(NINTENDO)
	INIT_FONT_EX(font, PRINT_WIN);
	INIT_HUD(stageEndingWindow);
	SetWindowY(144 - (6 << 3));
	#elif defined(SEGA)
	INIT_FONT_EX(font, PRINT_BKG);
	#endif

	for(i = 0; i != N_SPRITE_TYPES; ++ i) {
		SpriteManagerLoad(i);
	}

	end_sprite_princess = sprite_manager_sprites[StackPop(sprite_manager_sprites_pool)];
	InitSprite(end_sprite_princess, SpritePrincess);
	SetSpriteAnim(end_sprite_princess, end_anim_walk, 32);
	end_princess_screen_x = -16;
	end_sprite_princess->x = scroll_x + end_princess_screen_x;
	end_sprite_princess->y = scroll_y + 66u;
	end_sprite_princess->mirror = NO_MIRROR;
	
	SetEndState(AWAITING_PRINCESS_CENTER);

	PlayMusic(exo_ending, 1);
}

extern UINT8 next_oam_idx;
void UPDATE(void) {
	const struct EndSpriteInfo* info;

	scroll_p_x.w += 32;
	scroll_x += scroll_p_x.b.h;

	end_sprite_princess->x = scroll_x + end_princess_screen_x;
	THIS = end_sprite_princess;
	DrawSprite();

	switch(end_state) {
		case AWAITING_PRINCESS_CENTER:
				if(scroll_p_x.b.h != 0)
					end_princess_screen_x ++;
				if(U_LESS_THAN((INT16)72, (INT16)end_princess_screen_x))
					SetEndState(SHOWING_ENEMIES);
			break;

		case SHOWING_ENEMIES:
			if(text_wait == 0) {
				if(end_enemy_x > 72) {
 					end_enemy_x -= (UINT16)1;
					if(end_enemy_x <= 72) {
						text_wait = 100;
						wait_vbl_done();
						PRINT(TEXT_X_OFS + ((20u - STRLEN(endSpritesInfo[enemy_idx].name)) >> 1), TEXT_Y_OFS + 4, endSpritesInfo[enemy_idx].name);
					}
				} else {
					end_enemy_x --;
					if(U_LESS_THAN(end_enemy_x + (UINT16)16, 0)) {
						info = &endSpritesInfo[enemy_idx];
						if(info->type == SpritePrincess) {
							SetEndState(CREDITS);
						} else {
							enemy_idx ++;
							PrepareEnemy();
						}
					}
				}
			} else {
				text_wait --;
				if(text_wait == 0) {
					wait_vbl_done();
					PRINT(TEXT_X_OFS, TEXT_Y_OFS + 4, "                    ");
				}
			}
			end_sprite->x = scroll_x + (UINT16)end_enemy_x;
			THIS = end_sprite;
			DrawSprite();
			break;

		case CREDITS:
			text_wait --;
			if(text_wait == 0) {
				if(Credits[enemy_idx]) {
					PrepareCredits();
				} else {
					SetEndState(DONE);
				}
			}
			break;
	}


	scroll_p_x.b.h = 0;
}
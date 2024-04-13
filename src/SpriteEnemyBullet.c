#include "Banks/SetAutoBank.h"
#include "main.h"

#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Sound.h"

#include "Sounds.h"

typedef struct {
	INT8 vx;
	INT8 vy;
} CUSTOM_DATA;

void CreateEnemyBullet(UINT16 x, UINT16 y, INT8 vx, INT8 vy) BANKED {
	Sprite* spr = SpriteManagerAdd(SpriteEnemyBullet, x, y);
	CUSTOM_DATA* data = (CUSTOM_DATA*)spr->custom_data;

	data->vx = vx;
	data->vy = vy;

	ExecuteSFX(BANK(FX_BULLET), FX_BULLET, SFX_MUTE_MASK(FX_BULLET), SFX_PRIORITY_NORMAL);
}

void START(void) {
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	THIS->x += (INT16)data->vx << delta_time;
	THIS->y += (INT16)data->vy << delta_time;
}

void DESTROY(void) {
}

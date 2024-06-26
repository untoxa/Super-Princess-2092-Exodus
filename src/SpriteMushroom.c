#include "Banks/SetAutoBank.h"
#include "main.h"

#include "SpriteManager.h"
#include "Vector.h"

#include "Math.h"

void CreateEnemyBullet(UINT16 x, UINT16 y, INT8 vx, INT8 vy) BANKED;

const UINT8 mushroom_anim_hide[] = VECTOR(0);
const UINT8 mushroom_anim_show[] = VECTOR(1);

typedef struct {
	UINT8 time_out;
} CUSTOM_DATA;

void START(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	SetSpriteAnim(THIS, mushroom_anim_hide, 3u);

	data->time_out = 200u;
}

extern Sprite* sprite_princess;

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	UINT8 x;
	
	if(data->time_out > 50) {
		if(sprite_princess && (DISTANCE(sprite_princess->x, THIS->x) < 60u)) {
			data->time_out = 0;
			x = ((sprite_princess->x - THIS->x) & 0xF000) ? -1 : 1;

			CreateEnemyBullet(THIS->x + 6, THIS->y + 6, 0, -1);
			CreateEnemyBullet(THIS->x + 6, THIS->y + 8, x,  0);
			
			//CreateEnemyBullet(THIS->x + 8, THIS->y, x, -1);
		}
	} else {
		data->time_out += 1;	
	}

	if(data->time_out < 20u) {
		SetSpriteAnim(THIS, mushroom_anim_show, 3);
	} else {
		SetSpriteAnim(THIS, mushroom_anim_hide, 3);
	}
}

void DESTROY(void) {
}
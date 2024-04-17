#include "Banks/SetAutoBank.h"
#include "main.h"

#include "SpriteManager.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Sound.h"

#include "Sounds.h"

extern const UINT8 mushroom_anim_show[];

void START(void) {
	ExecuteSFX(BANK(FX_BULLET), FX_BULLET, SFX_MUTE_MASK(FX_BULLET), SFX_PRIORITY_NORMAL + 1);
}

void UPDATE(void) {
	UINT8 i;
	Sprite* spr;
	Sprite* spr2;
	
	if(THIS->mirror ==  V_MIRROR) {
		THIS->x -= 3 << delta_time;
	} else {
		THIS->x += 3 << delta_time;
	}

	if(scroll_collisions[GetScrollTile((THIS->x + 4)>> 3, (THIS->y + 4) >> 3)]) {
		SpriteManagerRemove(THIS_IDX);
		return;
	}

	SPRITEMANAGER_ITERATE(i, spr) {
		if(spr->type == SpriteMushroom || spr->type == SpriteCeilingShooter || spr->type == SpriteShooter || spr->type == SpriteFly || 
			spr->type == SpriteRoller || spr->type == SpriteOvni || spr->type == SpriteMissile) {
			if(CheckCollision(THIS, spr)) {
				if((spr->type != SpriteMushroom) || (spr->anim_data == mushroom_anim_show)) { //Mushroom can only die on frame 1
					SpriteManagerRemove(i);
					ExecuteSFX(BANK(FX_EXPLOSION), FX_EXPLOSION, SFX_MUTE_MASK(FX_EXPLOSION), SFX_PRIORITY_HIGH);
				}
				spr2 = SpriteManagerAdd(SpriteEnemyParticle, spr->x, spr->y);
				SpriteManagerRemove(THIS_IDX);
				break;
			}
		}
	}
}

void DESTROY(void) {
}
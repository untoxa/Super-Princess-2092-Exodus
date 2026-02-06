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
	
	if(THIS->mirror ==  V_MIRROR) {
		THIS->x -= 3 << delta_time;
	} else {
		THIS->x += 3 << delta_time;
	}

	if(scroll_collisions[GetScrollTile((THIS->x + 4)>> 3, (THIS->y + 4) >> 3)] & THIS->coll_group) {
		SpriteManagerRemove(THIS_IDX);
		return;
	}

	SPRITEMANAGER_ITERATE(i, spr) {
		switch (spr->type) {
			case SpriteMushroom:
				if (CheckCollision(THIS, spr)) {
					if (spr->anim_data == mushroom_anim_show) {
						SpriteManagerRemoveSprite(spr);
						ExecuteSFX(BANK(FX_EXPLOSION), FX_EXPLOSION, SFX_MUTE_MASK(FX_EXPLOSION), SFX_PRIORITY_HIGH);
					}
					SpriteManagerAdd(SpriteEnemyParticle, spr->x, spr->y);
					SpriteManagerRemoveSprite(THIS);
				}
				break;
			case SpriteCeilingShooter:
			case SpriteShooter: 
			case SpriteFly:
			case SpriteRoller: 
			case SpriteOvni: 
			case SpriteMissile:
				if (CheckCollision(THIS, spr)) {
					SpriteManagerRemoveSprite(spr);
					ExecuteSFX(BANK(FX_EXPLOSION), FX_EXPLOSION, SFX_MUTE_MASK(FX_EXPLOSION), SFX_PRIORITY_HIGH);
					SpriteManagerAdd(SpriteEnemyParticle, spr->x, spr->y);
					SpriteManagerRemoveSprite(THIS);
				}
				break;
		}
	}
}

void DESTROY(void) {
}
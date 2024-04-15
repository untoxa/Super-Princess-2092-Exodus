#include "Banks/SetAutoBank.h"
#include "main.h"
#include "Vector.h"

#include "SpriteManager.h"

const UINT8 eparticle_anim[] = VECTOR(0, 1, 2, 3, 3);

void START(void) {
	SetSpriteAnim(THIS, eparticle_anim, 33);
}

void UPDATE(void) {
	if(THIS->anim_frame == 4) {
		SpriteManagerRemove(THIS_IDX);
	}
}

void DESTROY(void) {
}
#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gbdk/platform.h>

#include "ZGBMain.h"
#include "Sprite.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Math.h"
#include "Music.h"
#include "Sound.h"
#include "Vector.h"

#include "Sounds.h"

void CreatePParticle(UINT16 x, UINT16 y, INT8 vx, INT8 vy) BANKED;

//#define DEBUG_CONTROLS

//Princes anims
const UINT8 anim_walk[]          = VECTOR(3, 4, 5, 4);
const UINT8 anim_idle[]          = VECTOR(1, 2);
const UINT8 anim_idle_shoot[]    = VECTOR(0);
const UINT8 anim_jump[]          = VECTOR(3);
const UINT8 anim_ladder_idle_cooldown[]   = VECTOR(9);
const UINT8 anim_ladder_moving_cooldown[] = VECTOR(9, 10);
const UINT8 anim_ladder_idle[]   = VECTOR(7);
const UINT8 anim_ladder_moving[] = VECTOR(7, 8);
const UINT8 anim_hit[]           = VECTOR(1, 6, 1, 6, 1, 6);

typedef enum  {
	PRINCESS_STATE_NORMAL,
	PRINCESS_STATE_JUMPING,
	PRINCESS_STATE_LADDER,
	PRINCESS_STATE_HIT
}PRINCESS_STATE;
PRINCESS_STATE princes_state;
INT16 princess_accel_y;

extern UINT8 current_level;

INT8 shoot_cooldown = 0;

Sprite* sprite_princess = 0;

Sprite* princess_parent = 0;
UINT16 princess_old_x, princess_old_y;

UINT8 bg_hidden = 0;

void START(void) {
	SetSpriteAnim(THIS, anim_idle, 3u);

	princess_accel_y = 0;

	scroll_target = THIS;
	sprite_princess = THIS;

	bg_hidden = 0;

	if(GetScrollTile((THIS->x) >> 3, (THIS->y) >> 3) == 23u) {
		princes_state = PRINCESS_STATE_LADDER;
	} else {
		princes_state = PRINCESS_STATE_NORMAL;
	}
}

void Hit(void) {
	princes_state = PRINCESS_STATE_HIT;
	StopMusic;

	sfx_sound_init();
	ExecuteSFX(BANK(FX_HIT), FX_HIT, SFX_MUTE_MASK(FX_HIT), SFX_PRIORITY_HIGH);
}

UINT8 tile_collision;
extern INT8 load_next;

extern UINT8 stage_completion;
extern UINT8 current_stage;
void CheckCollisionTile(void) {
	switch(tile_collision) {
		case 1u:
			load_next = 1;
			break;

		case 2u:
			load_next = -1;
			break;

		case 33u:
		case 35u:
			Hit();
			break;

		case 27u:
		case 28u:
			SET_BIT(stage_completion, current_stage);
			SetState(StateStageSelect);
			break;
	}
}

void MovePrincess(void) {
	if(KEY_PRESSED(J_RIGHT)) {
		tile_collision = TranslateSprite(THIS, 1 << delta_time, 0);
		THIS->mirror = NO_MIRROR;
		CheckCollisionTile();
	} else if(KEY_PRESSED(J_LEFT)) {
		tile_collision = TranslateSprite(THIS, -1 << delta_time, 0);
		THIS->mirror = V_MIRROR;
		CheckCollisionTile();
	}

#ifndef DEBUG_CONTROLS
	if(KEY_PRESSED(J_UP)) {
		UINT8 tile = GetScrollTile((THIS->x) >> 3, (THIS ->y + 13u) >> 3);
		if(tile == 23u || tile == 59u)
		{
			THIS->x = (((THIS->x)>> 3) << 3) + 4;
			princess_accel_y = 0;
			princes_state = PRINCESS_STATE_LADDER;
		}
	}
	if(KEY_PRESSED(J_DOWN)) {
		UINT8 tile = GetScrollTile((THIS->x) >> 3, (THIS ->y + 14u) >> 3);
		if(tile == 23u || tile == 59u)
		{
			THIS->x = (((THIS->x)>> 3) << 3) + 4;
			THIS->y = THIS->y + 1u;
			princess_accel_y = 0;
			princes_state = PRINCESS_STATE_LADDER;
		}
	}
#endif
#ifdef DEBUG_CONTROLS
	if(KEY_PRESSED(J_UP)) {
		tile_collision = TranslateSprite(THIS, 0, -1 << delta_time);
		CheckCollisionTile();
	} else if(KEY_PRESSED(J_DOWN)) {
		tile_collision = TranslateSprite(THIS, 0, 1 << delta_time);
		CheckCollisionTile();
	}
#endif
}

void Shoot(void) {
	Sprite* bullet_sprite = SpriteManagerAdd(SpriteBullet, 0, 0);

	bullet_sprite->mirror = THIS->mirror;
	if(THIS->mirror) 
		bullet_sprite->x = THIS->x - 2u;
	else
		bullet_sprite->x = THIS->x + 7u; 
	bullet_sprite->y = THIS->y + 3u;
	shoot_cooldown = 10;
}

void Jump(void) {
	if(princes_state != PRINCESS_STATE_JUMPING) {
		princess_accel_y = -50;
		princes_state = PRINCESS_STATE_JUMPING;
		princess_parent = 0;

		ExecuteSFX(BANK(FX_JUMP), FX_JUMP, SFX_MUTE_MASK(FX_JUMP), SFX_PRIORITY_NORMAL);
	}
}

void UPDATE(void) {
	UINT8 i;
	Sprite* spr;

	princess_old_x = THIS->x;
	princess_old_y = THIS->y;

	switch(princes_state) {
		case PRINCESS_STATE_NORMAL:
			MovePrincess();
	
			//Choose idle anim or walk
			if(KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT) ) {
				SetSpriteAnim(THIS, anim_walk, 33u);
			} else {
				if(shoot_cooldown) {
					SetSpriteAnim(THIS, anim_idle_shoot, 3u);
				} else {
					SetSpriteAnim(THIS, anim_idle, 3u);
				}
			}

			//Check jumping
			if(KEY_TICKED(J_A)) {
				Jump();
			} 

			//Check falling
			if((princess_accel_y >> 4) > 1) {
				princes_state = PRINCESS_STATE_JUMPING;
			}
			break;

		case PRINCESS_STATE_LADDER:
			i = 23;
			if(KEY_PRESSED(J_UP)) {
				SetSpriteAnim(THIS, shoot_cooldown ? anim_ladder_moving_cooldown : anim_ladder_moving, 12u);
				tile_collision = TranslateSprite(THIS, 0, -1 << delta_time);
				CheckCollisionTile();
				i = GetScrollTile((THIS->x) >> 3, (THIS->y + 13u) >> 3);
			} else if(KEY_PRESSED(J_DOWN)) {
				SetSpriteAnim(THIS, shoot_cooldown ? anim_ladder_moving_cooldown : anim_ladder_moving, 12u);
				tile_collision = TranslateSprite(THIS, 0, 1 << delta_time);
				CheckCollisionTile();
				i = GetScrollTile((THIS->x) >> 3, (THIS->y + 14u) >> 3);
			} else {
				SetSpriteAnim(THIS, shoot_cooldown ? anim_ladder_idle_cooldown : anim_ladder_idle, 12u);
			}
			if(KEY_PRESSED(J_RIGHT)) {
				THIS->mirror = NO_MIRROR;
			} else if(KEY_PRESSED(J_LEFT)) {
				THIS->mirror = V_MIRROR;
			}

			//Check the end of the ladder
			if(i != 23u && i != 59 && i != 1u && i != 2u)
			{
				//TranslateSprite(THIS, 0, 1 << delta_time);
				princes_state = PRINCESS_STATE_NORMAL;
			}

			//Check jumping
			if(KEY_TICKED(J_A)) {
				Jump();
			}
			break;

		case PRINCESS_STATE_JUMPING:
			if((princess_accel_y < 0) && !KEY_PRESSED(J_A)) {
				princess_accel_y = 0;
			}

			SetSpriteAnim(THIS, anim_jump, 33u);
			MovePrincess();
			break;

		case PRINCESS_STATE_HIT:
			SetSpriteAnim(THIS, anim_hit, 15u);
			if((THIS->anim_frame + 1) % 2){
				if(!bg_hidden) {
					HIDE_BKG;
					bg_hidden = 1;
				}
			} else {
				if(bg_hidden) {
					SHOW_BKG;
					bg_hidden = 0;
					ExecuteSFX(BANK(FX_HIT), FX_HIT, SFX_MUTE_MASK(FX_HIT), SFX_PRIORITY_HIGH);
				}
			}

			if(THIS->anim_frame == 5) {
				SpriteManagerRemove(THIS_IDX);
				scroll_target = 0;

				CreatePParticle(THIS->x, THIS->y,  1,  1);
				CreatePParticle(THIS->x, THIS->y,  1, -1);
				CreatePParticle(THIS->x, THIS->y, -1,  1);
				CreatePParticle(THIS->x, THIS->y, -1, -1);
				SHOW_BKG;
			}
			break;
	}

#ifndef DEBUG_CONTROLS
	if(princess_parent == 0 && princes_state != PRINCESS_STATE_LADDER && princes_state != PRINCESS_STATE_HIT) {
		//Simple gravity physics 
		if(princess_accel_y < 60) {
			princess_accel_y += 2;
		}
		tile_collision = TranslateSprite(THIS, 0, (princess_accel_y >> 4));
		if(!tile_collision && delta_time != 0 && princess_accel_y < 40) { //Do another iteration if there is no collision
			princess_accel_y += 2;
			tile_collision = TranslateSprite(THIS, 0, (princess_accel_y >> 4));
		}
		if(tile_collision) {
			if(tile_collision != 1u && tile_collision != 2u) {
				if(princes_state == PRINCESS_STATE_JUMPING && princess_accel_y > 0) {
					princes_state = PRINCESS_STATE_NORMAL;
				}
				princess_accel_y = 0;
			}

			CheckCollisionTile();
		}
	}
	if(princess_parent && princes_state == PRINCESS_STATE_JUMPING) {
		princess_accel_y = 0;
		princes_state = PRINCESS_STATE_NORMAL;
	}
#endif

	if(princes_state != PRINCESS_STATE_HIT) {
		//Check enemy collision
		SPRITEMANAGER_ITERATE(i, spr) {
			if(spr->type == SpriteMushroom || spr->type == SpriteEnemyBullet || spr->type == SpriteCeilingShooter || spr->type == SpriteShooter ||
				 spr->type == SpriteFly || spr->type == SpriteRoller || spr->type == SpriteOvni || spr->type == SpriteMissile) {
				if(CheckCollision(THIS, spr)) {
					Hit();
				}
			}
		}


		if(shoot_cooldown) {
			shoot_cooldown -= 1u;
		} else {
			if(KEY_TICKED(J_B)) {
				Shoot();
			}
		}
	}
}

void DESTROY(void) {
	sprite_princess = 0;
}



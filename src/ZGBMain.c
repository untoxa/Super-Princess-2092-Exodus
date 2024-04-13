#include "ZGBMain.h"
#include "Math.h"

UINT8 next_state = StateMenu;

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if(current_state == StateGame) {
		switch(*tile_ptr) {
			case 1:
			case 2:
				*tile = (*(tile_ptr + 1) == 24) ? 23 : 0;
				return 255u;

			default:
				return (*tile_ptr > (255u - N_SPRITE_TYPES)) ? ((*tile = 0), (255u - *tile_ptr)) : ((*tile = *tile_ptr), 255);
		}
	}
	return 255u;
}
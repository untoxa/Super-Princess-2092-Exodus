#ifndef SOUNDS_H
#define SOUNDS_h

#include "Sound.h"

BANKREF_EXTERN(FX_BULLET)
extern const UINT8 FX_BULLET[];
extern void __mute_mask_FX_BULLET;

BANKREF_EXTERN(FX_EXPLOSION)
extern const UINT8 FX_EXPLOSION[];
extern void __mute_mask_FX_EXPLOSION;

BANKREF_EXTERN(FX_HIT)
extern const UINT8 FX_HIT[];
extern void __mute_mask_FX_HIT;

BANKREF_EXTERN(FX_JUMP)
extern const UINT8 FX_JUMP[];
extern void __mute_mask_FX_JUMP;

BANKREF_EXTERN(FX_STAGE)
extern const uint8_t FX_STAGE[];
extern void __mute_mask_FX_STAGE;

#endif
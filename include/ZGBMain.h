#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES            \
_STATE(StateMenu)         \
_STATE(StateGame)         \
_STATE(StateLiveLost)     \
_STATE(StateGameOver)     \
_STATE(StateStageIntro)   \
_STATE(StateStageSelect)  \
_STATE(StateEnding)       \
STATE_DEF_END

#define SPRITES \
_SPRITE_DMG_MIRROR(SpritePrincess,  princess,           V_MIRROR) \
_SPRITE_DMG_MIRROR(SpriteBullet,    princessbullet8x16, V_MIRROR) \
_SPRITE_DMG(SpriteMushroom,         mushroom        ) \
_SPRITE_DMG(SpriteEnemyBullet,      enemybullet8x16 ) \
_SPRITE_DMG(SpriteCeilingShooter,   ceilingshooter  ) \
_SPRITE_DMG_MIRROR(SpriteShooter,   wallshooter,        V_MIRROR) \
_SPRITE_DMG(SpriteEnemyParticle,    enemyexplosion  ) \
_SPRITE_DMG(SpritePrincessParticle, princessParticle) \
_SPRITE_DMG(SpritePlatform,         mobileplatform  ) \
_SPRITE_DMG_MIRROR(SpriteFly,       flyingbug,          V_MIRROR) \
_SPRITE_DMG(SpriteRoller,           weirdroller     ) \
_SPRITE_DMG(SpriteOvni,             ovni            ) \
_SPRITE_DMG(SpriteMissile,          missile         ) \
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif

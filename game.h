#ifndef __GAME_H__
#define __GAME_H__

#include <Seed.h>
using namespace Seed;

#include "player.h"
#include "modifier.h"

#include <Box2D/Box2D.h>

class Hud;
class Map;
class MapLayerTiled;
class MapLayerMetadata;
class MapLayerMosaic;
class CollisionObject;
class Baby;

#define GRID_ROW	17
#define GRID_COL	12
#define GRID_SIZE	(GRID_ROW * GRID_COL)

#define SPAWN_INTERVAL 5.0f
#define GLOBAL_PEACE_TIME 180.0f

class SpritePop;

class Game : public IEventInputKeyboardListener, public IEventInputJoystickListener, public IEventWidgetListener
{
	public:
		Game();
		virtual ~Game();

		void Update(f32 dt);
		Player *pPlayer1;

		BOOL IsFinished() const;
		u32 GetFinishType() const;

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);
		virtual void OnInputKeyboardPress(const EventInputKeyboard *ev);

		// IEventInputJoystickListener
		virtual void OnInputJoystickButtonPress(const EventInputJoystick *ev) const;
		virtual void OnInputJoystickButtonRelease(const EventInputJoystick *ev) const;
		virtual void OnInputJoystickDPadMove(const EventInputJoystick *ev) const;
		virtual void OnInputJoystickAxisMove(const EventInputJoystick *ev) const;

		// IEventWidgetListener
		virtual void OnWidgetPress(const EventWidget *ev);
		virtual void OnWidgetRelease(const EventWidget *ev);
		virtual void OnWidgetReleaseOut(const EventWidget *ev);

	protected:
		b2World *world;

		Hud *pHud;
		Map *pMap[2];
		MapLayerMetadata *pCollision;
		MapLayerMosaic *pBackground1;

		
		Player *pPlayerKeyboard1;
		Player *pPlayerJoystick1;

		Button btnLeftP1;

		f32 fElapsedTime;
		f32 fPowerupSpawnTimer;

		u32 iFinishType;
		mutable u32 iJoystickDpad1;

		BOOL bIsFinished;
		BOOL bPaused;

		SoundSource sfxGong;
		Array<Modifier *, 5> arPowerUps;

		SpritePop *pSpacePop;
		SpritePop *pInfoPop;

		BOOL bInfoPopLeaving;

	protected:
		void SpawnModifier();
		void CheckModifierCollision();
		void ApplyModifier(ModifierType type);

	private:
		CollisionObject *borderTop;
		CollisionObject *borderBottom;
		CollisionObject *borderLeft;
		CollisionObject *borderRight;
};

#endif

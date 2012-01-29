#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "main.h"
#include "actor.h"
#include "map.h"
#include "spritepop.h"
#include "modifier.h"

#define PLAYER_FOLLOWERS	20
#define PLAYER_LIFES	5
#define RESPAWN_TIME	2.5f

enum AnimationState
{
	ANIM_EGG = 0,
	ANIM_MOVING_MAGGOT,
	ANIM_STOPPED_MAGGOT,
	ANIM_HIT_MAGGOT,
	ANIM_COCOON,
	ANIM_MOVING_BUTTERFLY,
	ANIM_STOPPED_BUTTERFLY,
	ANIM_HIT_BUTTERFLY
};

enum PlayerState
{
	EGG = 0,
	MAGGOT,
	COCOON,
	BUTTERFLY
};

class MapLayerMetadata;

class Player : public Actor
{
	public:
				Player(b2World *world, Map* map[2]);
		virtual ~Player();

		virtual void Update(f32 dt, MapLayerMetadata *collision, Player *player);

		virtual void StartThrust();
		virtual void StopThrust();
		virtual void StartRight();
		virtual void StopRight();
		virtual void StartLeft();
		virtual void StopLeft();

		virtual void IncLife();
		virtual void DecLife();
		virtual u32 GetLife();

        virtual void Hit(Player *player, ModifierType eType);

		void  SetState(PlayerState state);
		PlayerState GetState();

		BOOL IsMoving();

		f32 GetSpeed();

		void Start();
		BOOL IsPlaying();

		u32 GetLayerCount();
		void SpawnFollower();

	public:
		AnimationState eAnimation;

		f32 fElapsedDeathTime;
		f32 fElapsedInvertTime;
		f32 fElapsedGravityTime;
		f32 fElapsedWeightTime;
		f32 fElapsedShieldTime;

		BOOL bReducedGravity;
		BOOL bLockControls;

	protected:
		virtual void ResolveCollision(MapLayerMetadata *pCollision, Player *player);
		virtual void ResolveAnimation();
		virtual void SetAnimation(AnimationState eAnimation);
		virtual void StopAllSounds();

	protected:
		u32 iLives;

		SoundSource sfxThrust;
		SoundSource sfxThrustFail;
		SoundSource sfxDeath;
		SoundSource sfxHit;
		SoundSource sfxSteps;
	private:
		Map* pMap1;
		Map* pMap2;

		PlayerState state;

		f32 speed;
		f32 stateTimer;
		f32 currentTimer;

		BOOL moving;
		BOOL started;
		BOOL hited;

		BOOL bHowtoFly;
		BOOL bHowtoSlow;
		BOOL bHowtoFlyEnd;
		BOOL bHowtoSlowEnd;
		SpritePop *sptHowtoFly;
		SpritePop *sptHowtoSlow;
		//Sprite arFollowers[PLAYER_FOLLOWERS];
		Array<Sprite, PLAYER_FOLLOWERS> arFollowers;
};

#endif // __PLAYER_H__

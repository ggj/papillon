#ifndef __MODIFIER_H__
#define __MODIFIER_H__

#include "app.h"
#include "actor.h"
#include "assets.h"

enum ModifierType
{
	ModNone = 0,
	ModGustSlowdown,
	ModGustBoost,
	ModSpider,
	ModSpiderWeb,
	ModBranch,
	ModWaterDrop,
	ModRock,
	ModTime,
	ModBoost,
	ModBee,
	ModBird
};

class MapLayerMetadata;
class Player;

class Modifier : public Actor
{
	public:
        Modifier(b2World *world, Player* player, ModifierType eType);
		virtual ~Modifier();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);

		ModifierType GetType() { return eType; }
        BOOL IsDone();
        BOOL IsHited();
        void hit();

	protected:
		ModifierType eType;
        Player* player;

        BOOL done;
        BOOL hited;
};

class ModifierSpider : public Modifier
{
	public:
        ModifierSpider(b2World *world, Player* player);
		virtual ~ModifierSpider();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);
};

class ModifierBird : public Modifier
{
	public:
        ModifierBird(b2World *world, Player* player);
		virtual ~ModifierBird();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);
};

class ModifierBee : public Modifier
{
	public:
        ModifierBee(b2World *world, Player* player);
		virtual ~ModifierBee();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);
};

class ModifierWaterDrop : public Modifier
{
	public:
        ModifierWaterDrop(b2World *world, Player* player);
		virtual ~ModifierWaterDrop();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);

		Sprite sptDrop;
		Sprite sptWater;

	BOOL bCreated;
};

#endif

#ifndef __MODIFIER_H__
#define __MODIFIER_H__

#include "app.h"
#include "actor.h"

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

class Modifier : public Actor
{
	public:
		Modifier(b2World *world, ModifierType eType);
		virtual ~Modifier();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);

		ModifierType GetType() { return eType; }

	protected:
		ModifierType eType;
};

class ModifierSpider : public Modifier
{
	public:
		ModifierSpider(b2World *world);
		virtual ~ModifierSpider();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);
};

class ModifierBird : public Modifier
{
	public:
		ModifierBird(b2World *world);
		virtual ~ModifierBird();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);
};

class ModifierBee : public Modifier
{
	public:
		ModifierBee(b2World *world);
		virtual ~ModifierBee();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);
};

class ModifierWaterDrop : public Modifier
{
	public:
		ModifierWaterDrop(b2World *world);
		virtual ~ModifierWaterDrop();

		virtual void Update(f32 dt, MapLayerMetadata *pCollision);
};

#endif
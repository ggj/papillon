#include "modifier.h"
#include "assets.h"
#include "app.h"
#include "maplayermetadata.h"

Modifier::Modifier(b2World *world, ModifierType type)
	: Actor(world)
	, eType(type)
{
	sptActor.Load(SPT_TREE);
	sptActor.SetAnimation((u32)eType);

	this->SetWidth(sptActor.GetWidth() * pScreen->GetWidth() - PLAYER_BORDER * 2.0f);
	this->SetHeight(sptActor.GetHeight() * pScreen->GetHeight() - PLAYER_BORDER * 2.0f);

	sptActor.SetPriority(PRIORITY_ENTITY);
	pScene->Add(&sptActor);
}

Modifier::~Modifier()
{
	pScene->Remove(&sptActor);
	sptActor.Unload();
}

void Modifier::Update(f32 dt, MapLayerMetadata *pCollision)
{
	Actor::Update(dt);

	if (body)
	{
		b2Vec2 pos = body->GetPosition();
		sptActor.SetPosition(pos.x * PIXEL2METER - sptActor.GetWidth() * 0.5f, pos.y * PIXEL2METER * -1.0f - sptActor.GetHeight() * 0.5f);
	}

	this->SetPosition(sptActor.GetX() * pScreen->GetWidth() + PLAYER_BORDER, sptActor.GetY() * pScreen->GetHeight() + PLAYER_BORDER);
}

ModifierSpider::ModifierSpider(b2World *world)
	: Modifier(world, ModSpider)
{

}

ModifierSpider::~ModifierSpider()
{

}

void ModifierSpider::Update(f32 dt, MapLayerMetadata *pCollision)
{

}

ModifierBird::ModifierBird(b2World *world)
	: Modifier(world, ModBird)
{

}

ModifierBird::~ModifierBird()
{

}

void ModifierBird::Update(f32 dt, MapLayerMetadata *pCollision)
{

}

ModifierBee::ModifierBee(b2World *world)
	: Modifier(world, ModBee)
{

}

ModifierBee::~ModifierBee()
{

}

void ModifierBee::Update(f32 dt, MapLayerMetadata *pCollision)
{

}

ModifierWaterDrop::ModifierWaterDrop(b2World *world)
	: Modifier(world, ModWaterDrop)
{

}

ModifierWaterDrop::~ModifierWaterDrop()
{

}

void ModifierWaterDrop::Update(f32 dt, MapLayerMetadata *pCollision)
{

}

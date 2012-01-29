#include "modifier.h"
#include "assets.h"
#include "app.h"
#include "maplayermetadata.h"
#include "game.h"

Modifier::Modifier(b2World *world, ModifierType type)
	: Actor(world)
	, eType(type)
{
	//sptActor.Load(SPT_TREE);
	sptActor.SetAnimation((u32)eType);

	this->SetWidth(sptActor.GetWidth() * pScreen->GetWidth() - PLAYER_BORDER * 2.0f);
	this->SetHeight(sptActor.GetHeight() * pScreen->GetHeight() - PLAYER_BORDER * 2.0f);

	sptActor.SetPriority(20000);
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
		sptDrop.Load(SPT_DROP);
		sptDrop.SetAnimation((u32)0);
		sptDrop.Play();	
		sptDrop.AddX(sptDrop.GetWidth() * 0.4 + 1.0f);

	sptDrop.SetPriority(799);
	pScene->Add(&sptDrop);

sptWater.Load(SPT_DROP);
		sptWater.SetAnimation((u32)1);
		sptWater.Play();	
		sptWater.AddX(sptDrop.GetWidth() * 0.85 + 1.0f);
sptWater.AddY(sptDrop.GetWidth() * 0.13);

	sptWater.SetPriority(799);
	pScene->Add(&sptWater);
}

ModifierWaterDrop::~ModifierWaterDrop()
{

}

void ModifierWaterDrop::Update(f32 dt, MapLayerMetadata *pCollision)
{
	sptWater.SetX(sptWater.GetX() - pPlayer->
	
	if (sptWater.GetCurrentFrame() == sptWater.GetNumFrames() - 1)
	{
		sptActor.Load(SPT_DROP);
		sptActor.SetAnimation((u32)3);
		sptActor.SetX(sptWater.GetX() + sptActor.GetWidth() * 3.3);
		sptActor.SetY(sptWater.GetY() + sptActor.GetHeight() * 0.5);
		this->SetWidth(sptActor.GetWidth() * pScreen->GetWidth() - PLAYER_BORDER * 2.0f);
		this->SetHeight(sptActor.GetHeight() * pScreen->GetHeight() - PLAYER_BORDER * 2.0f);
		
		sptActor.SetPriority(799);
		pScene->Add(&sptActor);
		
		CreateDinamycBody(sptActor.GetX(), sptActor.GetY(), sptActor.GetWidth(), sptActor.GetHeight(), COLLISION_PLAYER, COLLISION_GROUND);
		SetGravityModifier(10.0f);
		//sptWater.Unload();
	}
	sptActor.SetY(sptActor.GetY() + 0.005f);


	if (sptWater.GetCurrentFrame() == sptWater.GetNumFrames() - 1)
	{


	}
}

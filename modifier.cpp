#include "modifier.h"
#include "assets.h"
#include "app.h"
#include "maplayermetadata.h"
#include "game.h"

Modifier::Modifier(b2World *world, Player* player, ModifierType type)
	: Actor(world)
	, player(player)
	, eType(type)
	, done(FALSE)
	, hited(FALSE)
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

BOOL Modifier::IsDone()
{
	return done;
}

BOOL Modifier::IsHited()
{
	return hited;
}

void Modifier::hit()
{
	player->Hit(NULL, eType);
	hited = TRUE;
}

ModifierSpider::ModifierSpider(b2World *world, Player* player)
	: Modifier(world, player, ModSpider)
{
	sptActor.Load(SPT_FOLHA);
	sptActor.SetAnimation((u32)0);

	sptActor.SetPriority(699);
	pScene->Add(&sptActor);

	sptActor.SetX(1.0f);
	sptActor.SetY(pRand->Get(0.1f, 0.9f));

	this->SetPosition(sptActor.GetX() * pScreen->GetWidth() + PLAYER_BORDER, sptActor.GetY() * pScreen->GetHeight() + PLAYER_BORDER);
	this->SetWidth(sptActor.GetWidth() * pScreen->GetWidth() - PLAYER_BORDER * 2.0f);
	this->SetHeight(sptActor.GetHeight() * pScreen->GetHeight() - PLAYER_BORDER * 2.0f);

	//CreateDinamycBody(sptActor.GetX(), sptActor.GetY(), sptActor.GetWidth(), sptActor.GetHeight(), COLLISION_OBJECT, COLLISION_GROUND);
}

ModifierSpider::~ModifierSpider()
{

}

void ModifierSpider::Update(f32 dt, MapLayerMetadata *pCollision)
{
	Modifier::Update(dt, pCollision);

	float move = dt * player->GetSpeed() * (f32)(player->GetLayerCount() - 1);

	if (player->IsMoving())
	{
		sptActor.AddX(-move);
	}

	if (hited)
	{
		done = TRUE;
	}

	if (sptActor.GetX() + sptActor.GetWidth() < 0.0f)
	{
		done = true;
	}
}

ModifierBird::ModifierBird(b2World *world, Player* player)
	: Modifier(world, player, ModBird)
{

}

ModifierBird::~ModifierBird()
{

}

void ModifierBird::Update(f32 dt, MapLayerMetadata *pCollision)
{

}

ModifierBee::ModifierBee(b2World *world, Player* player)
	: Modifier(world, player, ModBee)
{

}

ModifierBee::~ModifierBee()
{

}

void ModifierBee::Update(f32 dt, MapLayerMetadata *pCollision)
{

}

ModifierWaterDrop::ModifierWaterDrop(b2World *world, Player* player)
	: Modifier(world, player, ModWaterDrop)
{

		bCreated = false;

		sptDrop.Load(SPT_DROP);
		sptDrop.SetAnimation((u32)0);
		sptDrop.Play();
		sptDrop.SetX(1.0f);

		sptDrop.SetPriority(799);
		pScene->Add(&sptDrop);

		sptWater.Load(SPT_DROP);
		sptWater.SetAnimation((u32)1);
		sptWater.Play();
		sptWater.SetX(1.38f);
		sptWater.AddY(sptDrop.GetWidth() * 0.13);

		sptWater.SetPriority(799);
		pScene->Add(&sptWater);

		sptActor.Load(SPT_DROP);
		sptActor.SetAnimation((u32)3);

		this->SetWidth(sptActor.GetWidth() * pScreen->GetWidth() - PLAYER_BORDER * 2.0f);
		this->SetHeight(sptActor.GetHeight() * pScreen->GetHeight() - PLAYER_BORDER * 2.0f);

		sptActor.SetVisible(FALSE);
		sptActor.SetPriority(699);
		pScene->Add(&sptActor);
}

ModifierWaterDrop::~ModifierWaterDrop()
{
	pScene->Remove(&sptWater);
	pScene->Remove(&sptDrop);
}

void ModifierWaterDrop::Update(f32 dt, MapLayerMetadata *pCollision)
{
	Actor::Update(dt);

	if (bCreated)
	{
		if (sptActor.GetAnimation() != 2u)
		{
			if (body)
			{
				b2Vec2 pos = body->GetPosition();
				sptActor.SetPosition(0.05f + pos.x * PIXEL2METER - sptActor.GetWidth() * 0.5f, pos.y * PIXEL2METER * -1.0f - sptActor.GetHeight() * 0.5f);
			}

			this->SetPosition(sptActor.GetX() * pScreen->GetWidth() + PLAYER_BORDER, sptActor.GetY() * pScreen->GetHeight() + PLAYER_BORDER);
		}
	}

	float move = dt * player->GetSpeed() * (f32)(player->GetLayerCount() - 1);
	//sptActor.AddX(-move);
	if (player->IsMoving())
	{
		sptWater.AddX(-move);
		sptDrop.AddX(-move);
	}

	if (sptDrop.GetX() + sptDrop.GetWidth() < 0.0f)
	{
		done = true;
	}

	if (!bCreated && (sptDrop.GetX() + 0.3 < (player->GetX() + player->GetWidth())))
	{
		sptActor.SetX(sptWater.GetX() + sptActor.GetWidth() * 2.0f);
		sptActor.SetY(sptWater.GetY() + sptActor.GetHeight() * 0.65);
		CreateDinamycBody(sptActor.GetX(), sptActor.GetY(), sptActor.GetWidth(), sptActor.GetHeight(), COLLISION_OBJECT, COLLISION_GROUND);
		sptActor.SetVisible(true);
		bCreated = true;
	}

	if (bCreated)
	{
		if (sptActor.GetAnimation() != 2u)
		{
			if (player->IsMoving())
			{
				body->ApplyLinearImpulse(b2Vec2(-0.04f, 0.0f), b2Vec2(1.0f, 0.0f));
			}
			for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
			{
				b2Contact* c = ce->contact;

				CollisionObject* collision = (CollisionObject*)c->GetFixtureA()->GetBody()->GetUserData();
				if (collision && collision->GetName() && strcmp(collision->GetName(), "ground") == 0)
				{
					sptActor.SetAnimation(2u);
					body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				}
				else
				{
					collision = (CollisionObject*)c->GetFixtureB()->GetBody()->GetUserData();
					if (collision && collision->GetName() && strcmp(collision->GetName(), "ground") == 0)
					{
						sptActor.SetAnimation(2u);
						body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					}
				}
			}
		}
		else if (player->IsMoving())
		{
			sptActor.AddX(-move);
		}
	}
}

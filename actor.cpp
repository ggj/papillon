#include "actor.h"

Actor::Actor(b2World *world)
	: CollisionObject(world)
	, sptActor()
	, fLastX(0.0f)
	, fLastY(0.0f)
	, fGravityModifier(0.0f)
	, bInvertAxis(FALSE)
	, vDir()
	, vController()
{
	pScene->Add(this);
}

Actor::~Actor()
{
	pScene->Remove(this);
}

void Actor::Update(f32 dt)
{
    Point2f vThrust;
	vThrust.x = bInvertAxis ? -vController.x : vController.x;

	vThrust.y = vController.y;

	f32 fThrustX = (THRUST_POWER_X + fGravityModifier) * dt;
	f32 fThrustY = (THRUST_POWER_Y + fGravityModifier) * dt;

	vThrust.x *= fThrustX;
	vThrust.y *= fThrustY;

	if (body)
	{
		vThrust.x /= PIXEL2METER;
		vThrust.y /= PIXEL2METER;
		body->ApplyLinearImpulse(b2Vec2(vThrust.x, vThrust.y), b2Vec2(0.0f, 0.0f));
    }
}

Sprite &Actor::GetSprite()
{
	return sptActor;
}

void Actor::SetInvertedAxis(BOOL b)
{
	bInvertAxis = b;
}

void Actor::SetGravityModifier(f32 mod)
{
	fGravityModifier = mod;
}


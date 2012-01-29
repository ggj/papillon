#include "player.h"
#include "maplayermetadata.h"
#include "assets.h"

Player::Player(b2World *world, Map* map[2])
	: Actor(world)
	, eAnimation(FALL)
	, fElapsedDeathTime(0.0f)
	, fElapsedInvertTime(0.0f)
	, fElapsedGravityTime(0.0f)
	, fElapsedWeightTime(0.0f)
	, fElapsedShieldTime(0.0f)
	, bReducedGravity(FALSE)
	, bEnableShield(FALSE)
	, bLockControls(FALSE)
	, iLives(5)
{
	pMap1 = map[0];
	pMap2 = map[1];

	this->SetX(0.1f);
	this->SetY(0.3f);
	this->SetWidth(sptActor.GetWidth() * pScreen->GetWidth() - PLAYER_BORDER * 2.0f);
	this->SetHeight(sptActor.GetHeight() * pScreen->GetHeight() - PLAYER_BORDER * 2.0f);

	CreateDinamycBody(GetX(), GetY(), GetWidth(), GetHeight(), COLLISION_PLAYER, COLLISION_GROUND);

	sptActor.Load(SPT_PAPILLON);
	sptActor.SetColor(255u, 90u, 20u, 255u);
	sptActor.SetBlending(Seed::BlendModulate);
	sptActor.SetPriority(20000);
	pScene->Add(&sptActor);

//	sptShield.Load(SPT_TREE);
//	sptShield.SetPriority(PRIORITY_POWERUP);
//	sptShield.SetVisible(FALSE);
//	pScene->Add(&sptShield);

//	sptWeight.Load(SPT_TREE);
//	sptWeight.SetPriority(PRIORITY_POWERUP);
//	sptWeight.SetVisible(FALSE);
//	pScene->Add(&sptWeight);

//	sfxThrust.Load(nTeam == TeamPanda ? SFX_WINGS : SFX_JETPACK);
//	sfxThrust.SetVolume(nTeam == TeamPanda ? 0.3f : 0.05f);
//	pSoundSystem->Add(&sfxThrust);

//	sfxThrustFail.Load(nTeam == TeamPanda ? SFX_WINGS_FAIL : SFX_JETPACK_FAIL);
//	sfxThrustFail.SetVolume(0.1f);
//	pSoundSystem->Add(&sfxThrustFail);

//	sfxDeath.Load(nTeam == TeamPanda ? SFX_BEAR: SFX_MAIA);
//	sfxDeath.SetVolume(0.1f);
//	pSoundSystem->Add(&sfxDeath);

//	sfxHit.Load(SFX_HIT);
//	sfxHit.SetVolume(0.1f);
//	pSoundSystem->Add(&sfxHit);

//	sfxSteps.Load(SFX_STEPS);
//	sfxSteps.SetVolume(0.15f);
//	pSoundSystem->Add(&sfxSteps);
}

Player::~Player()
{
	pScene->Remove(&sptActor);
	sptActor.Unload();

	pSoundSystem->Remove(&sfxThrust);
	sfxThrust.Unload();

	pSoundSystem->Remove(&sfxThrustFail);
	sfxThrustFail.Unload();

	pSoundSystem->Remove(&sfxDeath);
	sfxDeath.Unload();

	pSoundSystem->Remove(&sfxHit);
	sfxHit.Unload();

	pSoundSystem->Remove(&sfxSteps);
	sfxSteps.Unload();
}

void Player::StartThrust()
{
	if (bLockControls)
		return;

	vController.y = 1;
}

void Player::StopThrust()
{
	if (bLockControls)
		return;

	vController.y = 0;
}

void Player::StartRight()
{
	if (bLockControls)
		return;

	vController.x = 1;
}

void Player::StopRight()
{
	if (bLockControls)
		return;

	vController.x = 0;
}

void Player::StartLeft()
{
	if (bLockControls)
		return;

	vController.x = -1;
}

void Player::StopLeft()
{
	if (bLockControls)
		return;

	vController.x = 0;
}

void Player::Update(f32 dt, MapLayerMetadata *collision, Player *player)
{
	if (bInvertAxis)
	{
		fElapsedInvertTime += dt;
		if (fElapsedInvertTime > 5.0f)
		{
			bInvertAxis = FALSE;
		}
	}

	if (bReducedGravity)
	{
		fElapsedGravityTime += dt;
		if (fElapsedGravityTime > 5.0f)
		{
			fGravityModifier = 0.0f;
			bReducedGravity = FALSE;
		}
	}

	if (bEnableShield)
	{
		fElapsedShieldTime += dt;
		if (fElapsedShieldTime >= 5.0f)
		{
			bEnableShield = FALSE;
			sptShield.SetVisible(FALSE);
		}
	}

	if (bEnableWeight)
	{
		fElapsedWeightTime += dt;
		if (fElapsedWeightTime >= 2.0f)
		{
			bEnableWeight = FALSE;
			fGravityModifier = 0.0f;
			sptWeight.SetVisible(FALSE);
		}
	}

	Actor::Update(dt);

	bool paralax = false;
	if (GetX() + GetWidth() > 0.75f)
	{
			SetX(0.75f - GetWidth());
			paralax = true;
	}

	if (paralax)
	{
		f32 sizeX = pScreen->GetWidth() * PIXEL2METER;
		f32 sizeY = pScreen->GetHeight() * PIXEL2METER;

		f32 b2x = GetX() * pScreen->GetWidth() / sizeX;
		f32 b2y = -(GetY() * pScreen->GetHeight() / sizeY);

		f32 b2w = GetWidth() * pScreen->GetWidth() / sizeX;
		f32 b2h = GetHeight() * pScreen->GetHeight() / sizeY;

		f32 x = b2x + b2w * 0.5f;
		f32 y = body->GetPosition().y;

		body->SetTransform(b2Vec2(x, y), body->GetAngle());
		body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));

		float speed = dt * 0.5f;

		f32 sw = (f32)pScreen->GetWidth();
		f32 size = 1.0f / sw * 1670;

		for (int i = 1; i < pMap1->GetLayerCount(); i++)
		{
			if (pMap1->GetLayerAt(i)->GetPosition().x <= 0.0f)
			{
				pMap1->GetLayerAt(i)->AddPosition(Point2f(-speed, 0.0f));
				pMap2->GetLayerAt(i)->SetPosition(Point2f(pMap1->GetLayerAt(i)->GetPosition().x + size + size, pMap1->GetLayerAt(i)->GetPosition().y));

				if (pMap2->GetLayerAt(i)->GetPosition().x <= 0.0f)
				{
					pMap1->GetLayerAt(i)->SetPosition(Point2f(1.0f, 0.0f));
				}
			}
			else
			{
				pMap2->GetLayerAt(i)->AddPosition(Point2f(-speed, 0.0f));
				pMap1->GetLayerAt(i)->SetPosition(Point2f(pMap2->GetLayerAt(i)->GetPosition().x + size + size, pMap2->GetLayerAt(i)->GetPosition().y));
			}

			speed += dt * 0.5f;
		}

		printf("%f - %f\n", pMap1->GetLayerAt(1)->GetPosition().x * sw, pMap2->GetLayerAt(1)->GetPosition().x * sw);
	}

	this->ResolveCollision(collision, player);
	this->ResolveAnimation();

	if (eAnimation == DEATH)
	{
		fElapsedDeathTime += dt;
		if (fElapsedDeathTime >= RESPAWN_TIME)
		{
					sptActor.SetPosition(-1.0f, -1.0f);
					body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					this->SetAnimation(FALL);
					bLockControls = FALSE;
		}
	}

	sptShield.SetPosition((sptActor.GetX() + sptActor.GetWidth() / 2.0f) - (sptShield.GetWidth() / 2.0f),
						  (sptActor.GetY() + sptActor.GetHeight() / 2.0f) - (sptShield.GetHeight() / 2.0f));

	sptWeight.SetPosition((sptActor.GetX() + sptActor.GetWidth() / 2.0f) - (sptWeight.GetWidth() / 2.0f),
						  sptActor.GetY() - sptWeight.GetHeight());
}

void Player::ResolveAnimation()
{
	//horizontal swap
		/*if (vDir.x > 0)
				sptActor.SetScaleX(1.0f);
		else
				sptActor.SetScaleX(-1.0f);*/

	if (eAnimation == DEATH)
		return;

	else if (eAnimation == ATTACK)
	{
		if (!sptActor.IsFinished())
			return;
	}

	if (vController.y > 0)
	{
		this->SetAnimation(FLY_UP);
	}
	else
	{
		if (vDir.y > 0)
			this->SetAnimation(FALL);
		else if (vDir.y == 0 && vDir.x)
			this->SetAnimation(RUN);
		else if (vDir.y == 0 && vDir.x == 0)
			this->SetAnimation(IDLE);
	}
}

void Player::ResolveCollision(MapLayerMetadata *collision, Player *player)
{
	CollisionDataArray col;

	this->SetPosition(sptActor.GetX() * pScreen->GetWidth() + PLAYER_BORDER, sptActor.GetY() * pScreen->GetHeight() + PLAYER_BORDER);
	if (player)
	{
		player->SetPosition(player->sptActor.GetX() * pScreen->GetWidth() + PLAYER_BORDER, player->sptActor.GetY() * pScreen->GetHeight() + PLAYER_BORDER);
	}

	Rect4f playerCollision;

	if (eAnimation != DEATH && (player && player->eAnimation != DEATH) && player && CheckHit(player->GetBoundingBox(), playerCollision))
	{
		if (this->GetBoundingBox().y < player->GetBoundingBox().y)
		{
			this->SetAnimation(ATTACK);
			if (!player->bEnableShield)
				player->Hit(this);
		}
	}

	if (body)
	{
		fLastX = sptActor.GetX();
		fLastY = sptActor.GetY();

		b2Vec2 pos = body->GetPosition();
		sptActor.SetPosition(pos.x * PIXEL2METER - sptActor.GetWidth() * 0.5f, pos.y * PIXEL2METER * -1.0f - sptActor.GetHeight() * 0.5f);

		vDir.x = sptActor.GetX() - fLastX;
		vDir.y = sptActor.GetY() - fLastY;
		vDir.Normalize();
	}

	this->SetPosition(sptActor.GetX() * pScreen->GetWidth() + PLAYER_BORDER, sptActor.GetY() * pScreen->GetHeight() + PLAYER_BORDER);
}

void Player::StopAllSounds()
{
	sfxThrust.Stop();
	sfxThrustFail.Stop();
	sfxDeath.Stop();
	sfxHit.Stop();
	sfxSteps.Stop();
}

void Player::SetAnimation(AnimationState animation)
{
	if (eAnimation == animation)
		return;

	eAnimation = animation;

	switch (eAnimation)
	{
		case FLY_UP:
		{
			this->StopAllSounds();
			sfxThrust.Play();
		}
		break;

		case DEATH:
		{
			this->StopAllSounds();
			sfxDeath.Play();
		}
		break;

		case ATTACK:
		{
			this->StopAllSounds();
			sfxHit.Play();
		}
		break;

		case FALL:
		{
			this->StopAllSounds();
			sfxThrustFail.Play();
		}
		break;

		case RUN:
		{
			this->StopAllSounds();
			sfxSteps.Play();
		}
		break;

		default:
		{
			this->StopAllSounds();
		}
		break;
	}

	sptActor.SetAnimation((u32)animation);
}

void Player::Hit(Player *player)
{
	if (eAnimation == DEATH)
		return;

	this->SetAnimation(DEATH);
	bLockControls = TRUE;
	fElapsedDeathTime = 0.0f;
	vController.x = vController.y = 0;
}

void Player::IncLife()
{
	if (iLives < PLAYER_LIFES * 2)
		iLives++;
}

void Player::DecLife()
{
	if (iLives > 0)
		iLives--;
}

u32 Player::GetLife()
{
	return iLives;
}
